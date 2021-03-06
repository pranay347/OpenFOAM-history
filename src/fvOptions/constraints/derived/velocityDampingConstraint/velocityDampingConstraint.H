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

Class
    Foam::fv::velocityDampingConstraint

Description
    Constraint for velocity to dampen velocity fluctuations in
    steady simulations. In case of velocity exceeding limit applies
    a source sink to remove the excess velocity by adding source term dU/dt
    where dt is a local time scale (velocity / cell size).

    Constraint described by:

        velocityDampingConstraintCoeffs
        {
            UMax        100;

            // Optional: name of velocity field (default: U)
            UName       U;
        }


SourceFiles
    velocityDampingConstraint.C

\*---------------------------------------------------------------------------*/

#ifndef velocityDampingConstraint_H
#define velocityDampingConstraint_H

#include "fvOption.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace fv
{

/*---------------------------------------------------------------------------*\
                 Class velocityDampingConstraint Declaration
\*---------------------------------------------------------------------------*/

class velocityDampingConstraint
:
    public option
{

protected:

    // Protected data

        //- Maximum velocity
        scalar UMax_;


    // Protected Member Functions

        void addDamping(fvMatrix<vector>& eqn);


private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        velocityDampingConstraint(const velocityDampingConstraint&);

        //- Disallow default bitwise assignment
        void operator=(const velocityDampingConstraint&);


public:

    //- Runtime type information
    TypeName("velocityDampingConstraint");


    // Constructors

        //- Construct from components
        velocityDampingConstraint
        (
            const word& name,
            const word& modelType,
            const dictionary& dict,
            const fvMesh& mesh
        );


    //- Destructor
    virtual ~velocityDampingConstraint()
    {}


    // Member Functions

        virtual bool alwaysApply() const;


        // Set values directly

            //- Constrain vector matrix
            virtual void setValue
            (
                fvMatrix<vector>& eqn,
                const label fieldI
            );


        // I-O

            //- Write data
            virtual void writeData(Ostream&) const;

            //- Read dictionary
            virtual bool read(const dictionary& dict);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
