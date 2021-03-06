/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2015 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "equipotentialPointSmoother.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace pointSmoothers
{
    defineTypeNameAndDebug(equipotentialPointSmoother, 0);
    addToRunTimeSelectionTable
    (
        pointSmoother,
        equipotentialPointSmoother,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::pointSmoothers::equipotentialPointSmoother::equipotentialPointSmoother
(
    const dictionary& dict,
    pointVectorField& pointDisplacement
)
:
    pointSmoother(dict, pointDisplacement)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::pointSmoothers::equipotentialPointSmoother::~equipotentialPointSmoother()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void Foam::pointSmoothers::equipotentialPointSmoother::calculate
(
    const labelList& facesToMove,
    const pointField& oldPoints,
    const pointField& currentPoints,
    polyMeshGeometry& meshGeometry
)
{
    // Number of points used in each average
    scalarField weights(mesh().nPoints(), 0);

    // Reset the displacements which are about to be calculated
    reset(facesToMove, weights);

    // Sum the non-internal face displacements
    forAll(facesToMove, faceToMoveI)
    {
        const label faceI(facesToMove[faceToMoveI]);

        if (!isInternalOrProcessorFace(faceI))
        {
            const face& fPoints(mesh().faces()[faceI]);

            const scalar area(mag(mesh().faceAreas()[faceI]));

            forAll(fPoints, fPointI)
            {
                const label pointI(fPoints[fPointI]);

                pointDisplacement()[pointI] +=
                    area
                   *(
                        meshGeometry.faceCentres()[faceI]
                      - oldPoints[pointI]
                    );

                weights[pointI] += area;
            }
        }
    }

    // Sum the internal face displacements
    forAll(facesToMove, faceToMoveI)
    {
        const label faceI(facesToMove[faceToMoveI]);

        if (isInternalOrProcessorFace(faceI))
        {
            const face& fPoints(mesh().faces()[faceI]);

            forAll(fPoints, fPointI)
            {
                const label pointI(fPoints[fPointI]);

                if (weights[pointI] < SMALL)
                {
                    const labelList& pCells(mesh().pointCells()[pointI]);

                    forAll(pCells, pCellI)
                    {
                        const label cellI(pCells[pCellI]);

                        const scalar volume(mesh().cellVolumes()[cellI]);

                        pointDisplacement()[pointI] +=
                            volume
                           *(
                                meshGeometry.cellCentres()[cellI]
                              - oldPoints[pointI]
                            );

                        weights[pointI] += volume;
                    }
                }
            }
        }
    }

    // Average
    average(facesToMove, weights);
}


// ************************************************************************* //
