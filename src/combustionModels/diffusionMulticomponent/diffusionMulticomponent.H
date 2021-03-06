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
    Foam::combustionModels::diffusionMulticomponent

Description

    Diffusion based turbulent combustion model for multicomponent species.

    The model calculates the laminar finite rate source terms based on
    the kinetic for each reaction in order to begin the combustion and
    evaluates the minimum between this and the cross diffusion rate term
    defined as D*prob*muEff*mag(grad(Yi)*grad(Yj)).

    where:

    D :     is a model constant.
    muEff : is the effective turbulent diffusivity
    prob :  is a Gaussian shaped distribution around the stoichiometric value
            of each reaction. The distribtion has the input parameter 'sigma'
            for standard deviation.

    The variable prob is multiplied by the factor: 1  + pow3(O2/oxydantRes),
    where oxydantRes is the residual oxydant specified for each reaction.

    In the combustion properties dictionary:

    diffusionMulticomponentCoeffs
    {
        Ci          (1.0 1.0);      // Default to 1
        fuels       (CH4 CO);
        oxydants    (O2 O2);
        YoxStream   (0.23 0.23);    // Default to 0.23
        YfStream    (1.0 1.0);      // Default to 1.0
        sigma       (0.02 0.02);    // Default to 0.02
        oxydantRes  (0.025 0.005);
        ftCorr      (0.0 0.0);      // Default to 0.0
    }

    ftCorr is used to shift the location of the flame and corrects the
    stochimetric mixture value when the mesh resolution is not enough
    to resolve the combustion zone.

    NOTE: Optionally a switch can be used to ignite the mixture using laminar
    combustion.


SourceFiles
    diffusionMulticomponent.C

\*---------------------------------------------------------------------------*/

#ifndef diffusionMulticomponent_H
#define diffusionMulticomponent_H

#include "scalarList.H"
#include "tmp.H"
#include "Reaction.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace combustionModels
{

/*---------------------------------------------------------------------------*\
                    Class diffusionMulticomponent Declaration
\*---------------------------------------------------------------------------*/

template<class CombThermoType, class ThermoType>
class diffusionMulticomponent
:
    public CombThermoType
{
    // Private data

        //- Reactions
        const PtrList<Reaction<ThermoType> >& reactions_;

        //- Thermodynamic data of the species
        const PtrList<ThermoType>& specieThermo_;

        //- Pointer list of source terms
        PtrList<volScalarField> RijPtr_;

        //- Model constants
        scalarList Ci_;

        //- List of fuels for each reaction
        wordList fuelNames_;

        //- List of oxydants for each reaction
        wordList oxydantNames_;

        //- Heat of combustion [J/Kg]
        scalarList qFuel_;

        //- Stoichiometric air-fuel mass ratio
        scalarList stoicRatio_;

        //- Stoichiometric oxygen-fuel mass ratio
        scalarList s_;

        //- Oxydaser sream mass concentrations
        scalarList YoxStream_;

        //- Fuel stream mass concentrations
        scalarList YfStream_;

        //- Mean distribution for gaussian probabililty
        scalarList sigma_;

        //- Residual oxydaser
        scalarList oxydantRes_;

        //- ft stochiometric correction
        scalarList ftCorr_;

        //- Relaxatnio factor on total source
        scalar alpha_;

        //- Switch on to laminar combustion for ignition
        bool laminarIgn_;


    // Private Member Functions

        //- Return the chemical time scale
        tmp<volScalarField> tc() const;

        //-init
        void init();

        //- Disallow copy construct
        diffusionMulticomponent(const diffusionMulticomponent&);

        //- Disallow default bitwise assignment
        void operator=(const diffusionMulticomponent&);


public:

    //- Runtime type information
    TypeName("diffusionMulticomponent");


    // Constructors

        //- Construct from components
        diffusionMulticomponent
        (
            const word& modelType,
            const fvMesh& mesh,
            const word& phaseName
        );


    //- Destructor
    virtual ~diffusionMulticomponent();


    // Member Functions

        // Evolution

            //- Correct combustion rate
            virtual void correct();

            //- Fuel consumption rate matrix.
            virtual tmp<fvScalarMatrix> R(volScalarField& Y) const;

            //- Heat release rate calculated from fuel consumption rate matrix
            virtual tmp<volScalarField> dQ() const;

            //-  Return source for enthalpy equation [kg/m/s3]
            virtual tmp<volScalarField> Sh() const;


    // I-O

            //- Update properties from given dictionary
            virtual bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace combustionModels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "diffusionMulticomponent.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
