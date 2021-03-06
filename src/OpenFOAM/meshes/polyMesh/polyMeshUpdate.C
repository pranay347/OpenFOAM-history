/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2014 OpenFOAM Foundation
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

Description
    Update the polyMesh corresponding to the given map.

\*---------------------------------------------------------------------------*/

#include "polyMesh.H"
#include "mapPolyMesh.H"
#include "Time.H"
#include "globalMeshData.H"
#include "pointMesh.H"
#include "indexedOctree.H"
#include "treeDataCell.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::polyMesh::updateMesh(const mapPolyMesh& mpm)
{
    if (debug)
    {
        Info<< "void polyMesh::updateMesh(const mapPolyMesh&) : "
            << "updating addressing and (optional) pointMesh/pointFields"
            << endl;
    }

    // Update boundaryMesh (note that patches themselves already ok)
    boundary_.updateMesh();

    // Update zones
    pointZones_.clearAddressing();
    faceZones_.clearAddressing();
    cellZones_.clearAddressing();

    // Remove the stored tet base points
    tetBasePtIsPtr_.clear();
    // Remove the cell tree
    cellTreePtr_.clear();

    // Update parallel data
    if (globalMeshDataPtr_.valid())
    {
        globalMeshDataPtr_->updateMesh();
    }

    setInstance(time().timeName());

    // Map the old motion points if present
    if (oldPointsPtr_.valid())
    {
        // Make a copy of the original points
        pointField oldMotionPoints = oldPointsPtr_();

        pointField& newMotionPoints = oldPointsPtr_();

        // Resize the list to new size
        newMotionPoints.setSize(points_.size());

        // Map the list
        if (mpm.hasMotionPoints())
        {
            newMotionPoints.map(oldMotionPoints, mpm.pointMap());

            // Any points created out-of-nothing get set to the current
            // coordinate for lack of anything better.
            forAll(mpm.pointMap(), newPointI)
            {
                if (mpm.pointMap()[newPointI] == -1)
                {
                    newMotionPoints[newPointI] = points_[newPointI];
                }
            }
        }
        else
        {
            const labelList& pointMap = mpm.pointMap();
            const labelList& revPointMap = mpm.reversePointMap();

            forAll(pointMap, newPointI)
            {
                label oldPointI = pointMap[newPointI];
                if (oldPointI >= 0)
                {
                    if (revPointMap[oldPointI] == newPointI) // master point
                    {
                        newMotionPoints[newPointI] = oldMotionPoints[oldPointI];
                    }
                    else
                    {
                        newMotionPoints[newPointI] = points_[newPointI];
                    }
                }
                else
                {
                    newMotionPoints[newPointI] = points_[newPointI];
                }
            }
        }
    }

    meshObject::updateMesh<polyMesh>(*this, mpm);
    meshObject::updateMesh<pointMesh>(*this, mpm);

    // Reset valid directions (could change by faces put into empty patches)
    geometricD_ = Vector<label>::zero;
    solutionD_ = Vector<label>::zero;

    const_cast<Time&>(time()).functionObjects().updateMesh(mpm);
}


// ************************************************************************* //
