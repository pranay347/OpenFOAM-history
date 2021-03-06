/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2015 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    globalMeshDataTest

Description
    Test global point communication

\*---------------------------------------------------------------------------*/

#include "globalMeshData.H"
#include "argList.H"
#include "polyMesh.H"
#include "Time.H"
#include "mapDistribute.H"
#include "IOmapDistribute.H"

using namespace Foam;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Main program:

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createPolyMesh.H"

    {
        // Test a bit of copying
        const mapDistribute& globalPointSlavesMap =
            mesh.globalData().globalPointSlavesMap();

        mapDistribute cp(globalPointSlavesMap);
        Debug(cp.constructSize());
    }

    const mapDistribute& someMap = mesh.globalData().globalPointSlavesMap();
    {
        // Test writing
        IOmapDistribute map
        (
            IOobject
            (
                "myMap",
                runTime.timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            someMap
        );

        map.write();
    }
    {
        // Test Reading
        IOmapDistribute map
        (
            IOobject
            (
                "myMap",
                runTime.timeName(),
                mesh,
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            )
        );

        if
        (
            map.subMap() != someMap.subMap()
         && map.constructMap() != someMap.constructMap()
         && map.constructSize() != someMap.constructSize()
        )
        {
            FatalErrorIn(args.executable())
                << "map:" << map << exit(FatalError);
        }


        // Test Transfer
        IOmapDistribute map2
        (
            IOobject
            (
                "myMap",
                runTime.timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            map.xfer()
        );

        if
        (
            map2.subMap() != someMap.subMap()
         && map2.constructMap() != someMap.constructMap()
         && map2.constructSize() != someMap.constructSize()
        )
        {
            FatalErrorIn(args.executable())
                << "map2:" << map2 << exit(FatalError);
        }
    }

    const globalMeshData& globalData = mesh.globalData();
    const indirectPrimitivePatch& coupledPatch = globalData.coupledPatch();
    const globalIndexAndTransform& transforms = globalData.globalTransforms();


    // Test:print shared points
    {
        const mapDistribute& globalPointSlavesMap =
            globalData.globalPointSlavesMap();
        const labelListList& slaves =
            globalData.globalPointSlaves();
        const labelListList& transformedSlaves =
            globalData.globalPointTransformedSlaves();

        // Create field with my local data
        pointField coords(globalPointSlavesMap.constructSize());
        SubList<point>(coords, coupledPatch.nPoints()).assign
        (
            coupledPatch.localPoints()
        );

        // Exchange data. Apply positional transforms.
        globalPointSlavesMap.distribute
        (
            transforms,
            coords,
            mapDistribute::transformPosition()
        );

        // Print
        forAll(slaves, pointI)
        {
            const labelList& slavePoints = slaves[pointI];

            if (slavePoints.size() > 0)
            {
                Pout<< "Master point:" << pointI
                    << " coord:" << coords[pointI]
                    << " connected to untransformed slave points:" << endl;

                forAll(slavePoints, i)
                {
                    Pout<< "    " << coords[slavePoints[i]] << endl;
                }
            }

            const labelList& transformedSlavePoints = transformedSlaves[pointI];

            if (transformedSlavePoints.size() > 0)
            {
                Pout<< "Master point:" << pointI
                    << " coord:" << coords[pointI]
                    << " connected to transformed slave points:" << endl;

                forAll(transformedSlavePoints, i)
                {
                    Pout<< "    " << coords[transformedSlavePoints[i]]
                        << endl;
                }
            }
        }
    }


    // Test:print shared edges
    {
        const mapDistribute& globalEdgeSlavesMap =
            globalData.globalEdgeSlavesMap();
        const labelListList& slaves =
            globalData.globalEdgeSlaves();
        const labelListList& transformedSlaves =
            globalData.globalEdgeTransformedSlaves();

        // Test: distribute edge centres
        pointField ec(globalEdgeSlavesMap.constructSize());
        forAll(coupledPatch.edges(), edgeI)
        {
            ec[edgeI] = coupledPatch.edges()[edgeI].centre
            (
                coupledPatch.localPoints()
            );
        }

        // Exchange data Apply positional transforms.
        globalEdgeSlavesMap.distribute
        (
            transforms,
            ec,
            mapDistribute::transformPosition()
        );

        // Print
        forAll(slaves, edgeI)
        {
            const labelList& slaveEdges = slaves[edgeI];

            if (slaveEdges.size() > 0)
            {
                Pout<< "Master edge:" << edgeI
                    << " centre:" << ec[edgeI]
                    << " connected to slave edges:" << endl;

                forAll(slaveEdges, i)
                {
                    Pout<< "    " << ec[slaveEdges[i]] << endl;
                }
            }
            const labelList& transformedSlaveEdges = transformedSlaves[edgeI];

            if (transformedSlaveEdges.size() > 0)
            {
                Pout<< "Master edge:" << edgeI
                    << " centre:" << ec[edgeI]
                    << " connected to transformed slave edges:" << endl;

                forAll(transformedSlaveEdges, i)
                {
                    Pout<< "    " << ec[transformedSlaveEdges[i]]
                        << endl;
                }
            }
        }
    }


    // Test: point to faces addressing
    {
        const mapDistribute& globalPointBoundaryFacesMap =
            globalData.globalPointBoundaryFacesMap();
        const labelListList& slaves =
            globalData.globalPointBoundaryFaces();
        const labelListList& transformedSlaves =
            globalData.globalPointTransformedBoundaryFaces();

        label nBnd = mesh.nFaces()-mesh.nInternalFaces();

        pointField fc(globalPointBoundaryFacesMap.constructSize());
        SubList<point>(fc, nBnd).assign
        (
            primitivePatch
            (
                SubList<face>
                (
                    mesh.faces(),
                    nBnd,
                    mesh.nInternalFaces()
                ),
                mesh.points()
            ).faceCentres()
        );

        // Exchange data
        globalPointBoundaryFacesMap.distribute
        (
            transforms,
            fc,
            mapDistribute::transformPosition()
        );

        // Print
        forAll(slaves, pointI)
        {
            const labelList& slaveFaces = slaves[pointI];

            if (slaveFaces.size() > 0)
            {
                Pout<< "Master point:" << pointI
                    << " at:" << coupledPatch.localPoints()[pointI]
                    << " connected to " << slaveFaces.size()
                    << " untransformed faces:" << endl;

                forAll(slaveFaces, i)
                {
                    Pout<< "    " << fc[slaveFaces[i]] << endl;
                }
            }

            const labelList& transformedSlaveFaces = transformedSlaves[pointI];

            if (transformedSlaveFaces.size() > 0)
            {
                Pout<< "Master point:" << pointI
                    << " connected to " << transformedSlaveFaces.size()
                    << " transformed faces:" << endl;

                forAll(transformedSlaveFaces, i)
                {
                    Pout<< "    " << fc[transformedSlaveFaces[i]] << endl;
                }
            }
        }
    }


    // Test: point to cells addressing
    {
        const labelList& boundaryCells = globalData.boundaryCells();
        const mapDistribute& globalPointBoundaryCellsMap =
            globalData.globalPointBoundaryCellsMap();
        const labelListList& slaves = globalData.globalPointBoundaryCells();
        const labelListList& transformedSlaves =
            globalData.globalPointTransformedBoundaryCells();

        pointField cc(globalPointBoundaryCellsMap.constructSize());
        forAll(boundaryCells, i)
        {
            cc[i] = mesh.cellCentres()[boundaryCells[i]];
        }

        // Exchange data
        globalPointBoundaryCellsMap.distribute
        (
            transforms,
            cc,
            mapDistribute::transformPosition()
        );

        // Print
        forAll(slaves, pointI)
        {
            const labelList& pointCells = slaves[pointI];

            if (pointCells.size() > 0)
            {
                Pout<< "Master point:" << pointI
                    << " at:" << coupledPatch.localPoints()[pointI]
                    << " connected to " << pointCells.size()
                    << " untransformed boundaryCells:" << endl;

                forAll(pointCells, i)
                {
                    Pout<< "    " << cc[pointCells[i]] << endl;
                }
            }

            const labelList& transformPointCells = transformedSlaves[pointI];

            if (transformPointCells.size() > 0)
            {
                Pout<< "Master point:" << pointI
                    << " connected to " << transformPointCells.size()
                    << " transformed boundaryCells:" << endl;

                forAll(transformPointCells, i)
                {
                    Pout<< "    " << cc[transformPointCells[i]] << endl;
                }
            }
        }
    }


    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
