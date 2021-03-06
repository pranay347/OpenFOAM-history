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

#include "directionalPressureGradientExplicitSource.H"
#include "fvMatrices.H"
#include "DimensionedField.H"
#include "IFstream.H"
#include "addToRunTimeSelectionTable.H"
#include "transform.H"
#include "surfaceInterpolate.H"
#include "turbulenceModel.H"
#include "turbulentTransportModel.H"
#include "turbulentFluidThermoModel.H"

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

namespace Foam
{
namespace fv
{
    defineTypeNameAndDebug(directionalPressureGradientExplicitSource, 0);

    addToRunTimeSelectionTable
    (
        option,
        directionalPressureGradientExplicitSource,
        dictionary
    );
}
}


// * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * * * //

namespace Foam
{
    template<>
    const char* Foam::NamedEnum
    <
        Foam::fv::
        directionalPressureGradientExplicitSource::
        pressureDropModel,
        3
    >::names[] =
    {
        "volumetricFlowRateTable",
        "constant",
        "DarcyForchheimer"
    };
}

const Foam::NamedEnum
<
    Foam::fv::directionalPressureGradientExplicitSource::pressureDropModel,
    3
> Foam::fv::directionalPressureGradientExplicitSource::PressureDropModelNames_;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::fv::directionalPressureGradientExplicitSource::initialise()
{
    const faceZone& fZone = mesh_.faceZones()[zoneID_];

    faceId_.setSize(fZone.size());
    facePatchId_.setSize(fZone.size());

    label count = 0;
    forAll(cellFaceMap_, i)
    {
        label faceI = cellFaceMap_[i];

        label faceId = -1;
        label facePatchId = -1;
        if (mesh_.isInternalFace(faceI))
        {
            faceId = faceI;
            facePatchId = -1;
        }
        else
        {
            facePatchId = mesh_.boundaryMesh().whichPatch(faceI);
            const polyPatch& pp = mesh_.boundaryMesh()[facePatchId];
            if (isA<coupledPolyPatch>(pp))
            {
                if (refCast<const coupledPolyPatch>(pp).owner())
                {
                    faceId = pp.whichFace(faceI);
                }
                else
                {
                    faceId = -1;
                }
            }
            else if (!isA<emptyPolyPatch>(pp))
            {
                faceId = faceI - pp.start();
            }
            else
            {
                faceId = -1;
                facePatchId = -1;
            }
        }

        if (faceId >= 0)
        {
            facePatchId_[count] = facePatchId;
            faceId_[count] = faceId;
            count++;
        }
    }
    faceId_.setSize(count);
    facePatchId_.setSize(count);
}


void Foam::fv::directionalPressureGradientExplicitSource::writeProps
(
    const vectorField& gradP
) const
{
    // Only write on output time
    if (mesh_.time().outputTime())
    {
        IOdictionary propsDict
        (
            IOobject
            (
                name_ + "Properties",
                mesh_.time().timeName(),
                "uniform",
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            )
        );
        propsDict.add("gradient", gradP);
        propsDict.regIOobject::write();
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::fv::directionalPressureGradientExplicitSource::
directionalPressureGradientExplicitSource
(
    const word& sourceName,
    const word& modelType,
    const dictionary& dict,
    const fvMesh& mesh
)
:
    option(sourceName, modelType, dict, mesh),
    model_(PressureDropModelNames_.read(coeffs_.lookup("model"))),
    gradP0_(cells_.size(), vector::zero),
    dGradP_(cells_.size(), vector::zero),
    gradPporous_(cells_.size(), vector::zero),
    flowDir_(coeffs_.lookup("flowDir")),
    invAPtr_(NULL),
    D_(0),
    I_(0),
    length_(0),
    pressureDrop_(0),
    flowRate_(),
    faceZoneName_(coeffs_.lookup("faceZone")),
    zoneID_(mesh_.faceZones().findZoneID(faceZoneName_)),
    faceId_(),
    facePatchId_(),
    relaxationFactor_(coeffs_.lookupOrDefault<scalar>("relaxationFactor",0.3)),
    cellFaceMap_(cells_.size(), -1)
{
    coeffs_.lookup("fieldNames") >> fieldNames_;

    flowDir_ /= mag(flowDir_);

    if (fieldNames_.size() != 1)
    {
        FatalErrorIn
        (
            "Foam::fv::directionalPressureGradientExplicitSource::"
            "directionalPressureGradientExplicitSource"
            "("
                "const word&, "
                "const word&, "
                "const dictionary&, "
                "const fvMesh&"
            ")"
        )   << "Source can only be applied to a single field.  Current "
            << "settings are:" << fieldNames_ << exit(FatalError);
    }

    if (zoneID_ < 0)
    {
        FatalErrorIn
        (
            "directionalPressureGradientExplicitSource::"
            "directionalPressureGradientExplicitSource\n"
            "(\n"
                "const word&,\n "
                "const word&,\n "
                "const dictionary&, \n"
                "const fvMesh& \n"
            ")\n"
        )
            << type() << " " << this->name() << ": "
            << "    Unknown face zone name: " << faceZoneName_
            << ". Valid face zones are: " << mesh_.faceZones().names()
            << nl << exit(FatalError);
    }

    label fZoneSize = mesh_.faceZones()[zoneID_].size();

    if (mesh_.faceZones()[zoneID_].size() != cells_.size())
    {
        FatalErrorIn
        (
            "directionalPressureGradientExplicitSource::"
            "directionalPressureGradientExplicitSource\n"
            "(\n"
                "const word&,\n "
                "const word&,\n "
                "const dictionary&,\n "
                "const fvMesh&\n"
            ")\n"
        )
            << " face zone size :" << fZoneSize
            << " different from cell zone size : " << cells_.size() << nl
            << " This momemtum source requires to be equal. \n"
            << " It should be created with the faces upstream the cell zone"
            << nl << exit(FatalError);
    }

    if (model_ == pVolumetricFlowRateTable)
    {
        flowRate_ = interpolationTable<scalar>(coeffs_);
    }
    else if (model_ == pConstant)
    {
        coeffs_.lookup("pressureDrop") >> pressureDrop_;
    }
    else if (model_ == pDarcyForchheimer)
    {
        coeffs_.lookup("D") >> D_;
        coeffs_.lookup("I") >> I_;
        coeffs_.lookup("length") >> length_;
    }
    else
    {
        FatalErrorIn
        (
            "directionalPressureGradientExplicitSource::"
            "directionalPressureGradientExplicitSource\n"
            "(\n"
                "const word&, \n"
                "const word&, \n"
                "const dictionary&, \n"
                "const fvMesh& \n"
            ") \n"
        )
            << "Did not find mode " << model_
            << nl
            << "Please set 'model' to one of "
            << PressureDropModelNames_.toc()
            << exit(FatalError);
    }

    applied_.setSize(fieldNames_.size(), false);

    const faceZone& fZone = mesh_.faceZones()[zoneID_];
    const cellList& cells = mesh_.cells();

    forAll (cells_, i)
    {
        label faceId = -1;
        const cell& cellI = cells[cells_[i]];
        forAll (cellI, j)
        {
            label faceI = cellI[j];
            if (findIndex(fZone, faceI) != -1)
            {
                faceId = faceI;
                break;
            }
        }
        cellFaceMap_[i] = faceId;
    }

    // Read the initial pressure gradient from file if it exists
    IFstream propsFile
    (
        mesh_.time().timePath()/"uniform"/(name_ + "Properties")
    );

    if (propsFile.good())
    {
        Info<< "    Reading pressure gradient from file" << endl;
        dictionary propsDict(dictionary::null, propsFile);
        propsDict.lookup("gradient") >> gradP0_;
    }

    Info<< "    Initial pressure gradient = " << gradP0_ << nl << endl;

    initialise();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::fv::directionalPressureGradientExplicitSource::correct
(
    volVectorField& U
)
{
    const scalarField& rAU = invAPtr_().internalField();

    const scalarField magUn(mag(U), cells_);

    const surfaceScalarField& phi =
            mesh().lookupObject<surfaceScalarField>("phi");

    switch (model_)
    {
        case pDarcyForchheimer:
        {
            if (phi.dimensions() == dimVelocity*dimArea)
            {
                const incompressible::turbulenceModel& turbModel =
                    mesh().lookupObject<incompressible::turbulenceModel>
                    (
                        turbulenceModel::propertiesName
                    );

                const scalarField nu(turbModel.nu(), cells_);

                gradPporous_ = -flowDir_*(D_*nu + I_*0.5*magUn)*magUn*length_;
                break;
            }
            else
            {
                const compressible::turbulenceModel& turbModel =
                    mesh().lookupObject<compressible::turbulenceModel>
                    (
                        turbulenceModel::propertiesName
                    );

                const scalarField mu(turbModel.mu(),cells_);

                const scalarField rho(turbModel.rho(),cells_);

                gradPporous_ =
                    - flowDir_*(D_*mu + I_*0.5*rho*magUn)*magUn*length_;
            }
        }
        case pConstant:
        {
            gradPporous_ = -flowDir_*pressureDrop_;
            break;
        }

        case pVolumetricFlowRateTable:
        {
            scalar volFlowRate = 0;
            scalar totalphi = 0;

            forAll(faceId_, i)
            {
                label faceI = faceId_[i];
                if (facePatchId_[i] != -1)
                {
                    label patchI = facePatchId_[i];
                    totalphi += phi.boundaryField()[patchI][faceI];
                }
                else
                {
                    totalphi += phi[faceI];
                }
            }
            reduce(totalphi, sumOp<scalar>());

            if (phi.dimensions() == dimVelocity*dimArea)
            {
                volFlowRate = mag(totalphi);
            }
            else
            {
                const compressible::turbulenceModel& turbModel =
                    mesh().lookupObject<compressible::turbulenceModel>
                    (
                        turbulenceModel::propertiesName
                    );
                const scalarField rho(turbModel.rho(),cells_);
                const scalarField cv(mesh_.V(), cells_);
                scalar rhoAve = gSumProd(rho, cv)/gSum(cv);
                volFlowRate = mag(totalphi)/rhoAve;
            }

            gradPporous_ = -flowDir_*flowRate_(volFlowRate);
            break;
        }
    }

    const surfaceVectorField Uf
    (
        fvc::interpolate(U, "directionalPressureGradient::Uf")
    );

    vectorField UfCells(faceId_.size(), vector::zero);

    forAll(faceId_, i)
    {
        label faceI = faceId_[i];
        if (facePatchId_[i] != -1)
        {
            label patchI = facePatchId_[i];
            UfCells[i] = U.boundaryField()[patchI][faceI];
        }
        else
        {
            UfCells[i] = Uf[faceI];
        }
    }

    forAll(cells_, i)
    {
        label cellI = cells_[i];

        const vector Ufnorm = UfCells[i]/mag(UfCells[i]);
        const tensor D = rotationTensor(Ufnorm, flowDir_);

        dGradP_[i] +=
            relaxationFactor_*((D & UfCells[i]) - U[cellI])/rAU[cellI];

        if (debug)
        {
            Info<< "Flow deflection parameter = "
                << (D & UfCells[i]) - U[cellI] << endl;
            Info<< "Difference mag(U) = " << mag(UfCells[i]) - mag(U[cellI])
                << endl;
            Info<< "Pressure drop in flowDir direction : "
                << gradPporous_[i] << endl;
            Info<< "UfCell:= " << UfCells[i] << "U : " << U[cellI] << endl;
        }
    }

    writeProps(gradP0_ + dGradP_);
}


void Foam::fv::directionalPressureGradientExplicitSource::addSup
(
    fvMatrix<vector>& eqn,
    const label fieldI
)
{
    DimensionedField<vector, volMesh> Su
    (
        IOobject
        (
            name_ + fieldNames_[fieldI] + "Sup",
            mesh_.time().timeName(),
            mesh_,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh_,
        dimensionedVector("zero", eqn.dimensions()/dimVolume, vector::zero)
    );

    UIndirectList<vector>(Su, cells_) = gradP0_ + dGradP_ + gradPporous_;

    eqn += Su;
}


void Foam::fv::directionalPressureGradientExplicitSource::addSup
(
    const volScalarField& rho,
    fvMatrix<vector>& eqn,
    const label fieldI
)
{
    this->addSup(eqn, fieldI);
}


void Foam::fv::directionalPressureGradientExplicitSource::setValue
(
    fvMatrix<vector>& eqn,
    const label
)
{
    if (invAPtr_.empty())
    {
        invAPtr_.reset
        (
            new volScalarField
            (
                IOobject
                (
                    name_ + ":invA",
                    mesh_.time().timeName(),
                    mesh_,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                1.0/eqn.A()
            )
        );
    }
    else
    {
        invAPtr_() = 1.0/eqn.A();
    }

    gradP0_ += dGradP_;
    dGradP_ = vector::zero;
}


// ************************************************************************* //
