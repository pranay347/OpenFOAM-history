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

\*---------------------------------------------------------------------------*/

#include "GradientDispersionRAS.H"
#include "demandDrivenData.H"
#include "fvcGrad.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
Foam::GradientDispersionRAS<CloudType>::GradientDispersionRAS
(
    const dictionary& dict,
    CloudType& owner
)
:
    DispersionRASModel<CloudType>(dict, owner),
    gradkPtr_(NULL),
    ownGradK_(false)
{}


template<class CloudType>
Foam::GradientDispersionRAS<CloudType>::GradientDispersionRAS
(
    const GradientDispersionRAS<CloudType>& dm
)
:
    DispersionRASModel<CloudType>(dm),
    gradkPtr_(dm.gradkPtr_),
    ownGradK_(dm.ownGradK_)
{
    dm.ownGradK_ = false;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
Foam::GradientDispersionRAS<CloudType>::~GradientDispersionRAS()
{
    cacheFields(false);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void Foam::GradientDispersionRAS<CloudType>::cacheFields(const bool store)
{
    DispersionRASModel<CloudType>::cacheFields(store);

    if (store)
    {
        gradkPtr_ = fvc::grad(*this->kPtr_).ptr();
        ownGradK_ = true;
    }
    else
    {
        if (ownGradK_)
        {
            deleteDemandDrivenData(gradkPtr_);
            gradkPtr_ = NULL;
            ownGradK_ = false;
        }
    }
}


template<class CloudType>
Foam::vector Foam::GradientDispersionRAS<CloudType>::update
(
    const scalar dt,
    const label cellI,
    const vector& U,
    const vector& Uc,
    vector& UTurb,
    scalar& tTurb
)
{
    cachedRandom& rnd = this->owner().rndGen();

    const scalar cps = 0.16432;

    const scalar k = this->kPtr_->internalField()[cellI];
    const scalar epsilon =
        this->epsilonPtr_->internalField()[cellI] + ROOTVSMALL;
    const vector& gradk = this->gradkPtr_->internalField()[cellI];

    const scalar UrelMag = mag(U - Uc - UTurb);

    const scalar tTurbLoc =
        min(k/epsilon, cps*pow(k, 1.5)/epsilon/(UrelMag + SMALL));


    // Parcel is perturbed by the turbulence
    if (dt < tTurbLoc)
    {
        tTurb += dt;

        if (tTurb > tTurbLoc)
        {
            tTurb = 0.0;

            scalar sigma = sqrt(2.0*k/3.0);
            vector dir = -gradk/(mag(gradk) + SMALL);

            // Numerical Recipes... Ch. 7. Random Numbers...
            scalar x1 = 0.0;
            scalar x2 = 0.0;
            scalar rsq = 10.0;
            while ((rsq > 1.0) || (rsq == 0.0))
            {
                x1 = 2.0*rnd.sample01<scalar>() - 1.0;
                x2 = 2.0*rnd.sample01<scalar>() - 1.0;
                rsq = x1*x1 + x2*x2;
            }

            scalar fac = sqrt(-2.0*log(rsq)/rsq);

            // In 2D calculations the -grad(k) is always
            // away from the axis of symmetry
            // This creates a 'hole' in the spray and to
            // prevent this we let x1 be both negative/positive
            if (this->owner().mesh().nSolutionD() == 2)
            {
                fac *= x1;
            }
            else
            {
                fac *= mag(x1);
            }

            UTurb = sigma*fac*dir;
        }
    }
    else
    {
        tTurb = GREAT;
        UTurb = vector::zero;
    }

    return Uc + UTurb;
}


// ************************************************************************* //
