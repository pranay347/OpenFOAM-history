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

#include "CloudToVTK.H"
#include "vtkTools.H"
//#include "OFstream.H"
#include "floatScalar.H"

// * * * * * * * * * * * * * Protectd Member Functions * * * * * * * * * * * //

template<class CloudType>
void Foam::CloudToVTK<CloudType>::writeData
(
    OFstream& os,
    const bool binary,
    const List<floatScalar>& data
) const
{
    const label procI = Pstream::myProcNo();

    List<List<floatScalar> > allProcData(Pstream::nProcs());
    allProcData[procI] = data;
    Pstream::gatherList(allProcData);
    List<floatScalar> allData =
        ListListOps::combine<List<floatScalar> >
        (
            allProcData,
            accessOp<List<floatScalar> >()
        );

    vtkTools::write(os, binary, allData);
}


template<class CloudType>
template<class Type>
void Foam::CloudToVTK<CloudType>::writeFieldData
(
    OFstream& os,
    const bool binary,
    const List<floatScalar>& data,
    const word& title,
    const label nParcels
) const
{
    os  << title << ' ' << pTraits<Type>::nComponents << ' '
        << nParcels << " float" << endl;
    writeData(os, binary, data);
}


template<class CloudType>
void Foam::CloudToVTK<CloudType>::write()
{
    label nParcels = this->owner().size();
    DynamicList<floatScalar> position(3*nParcels);
    DynamicList<floatScalar> U(3*nParcels);
    DynamicList<floatScalar> d(nParcels);
    DynamicList<floatScalar> age(nParcels);
    DynamicList<floatScalar> rho(nParcels);

    forAllConstIter(typename CloudType, this->owner(), iter)
    {
        vtkTools::insert(iter().position(), position);
        vtkTools::insert(iter().U(), U);
        vtkTools::insert(iter().d(), d);
        vtkTools::insert(iter().age(), age);
        vtkTools::insert(iter().rho(), rho);
    }

    reduce(nParcels, sumOp<label>());


binary_ = false;
    if (Pstream::master())
    {
        // Create directory if does not exist
        mkDir(this->outputTimeDir());

        // Open new file at start up

        const fileName fName = this->outputTimeDir()/(type() + ".vtk");
        this->setModelProperty("file", fName);

        OFstream os(fName, binary_ ? IOstream::BINARY : IOstream::ASCII);

        vtkTools::writeHeader(os, binary_, this->modelName().c_str());
        os  << "DATASET POLYDATA" << endl
            << "POINTS " << nParcels << " float" << endl;

        writeData(os, binary_, position);

        os  << "POINT_DATA " << nParcels << endl
            << "FIELD attributes " << 4
            << endl;

        writeFieldData<vector>(os, binary_, U, "U", nParcels);
        writeFieldData<scalar>(os, binary_, d, "d", nParcels);
        writeFieldData<scalar>(os, binary_, age, "age", nParcels);
        writeFieldData<scalar>(os, binary_, rho, "rho", nParcels);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
Foam::CloudToVTK<CloudType>::CloudToVTK
(
    const dictionary& dict,
    CloudType& owner,
    const word& modelName
)
:
    CloudFunctionObject<CloudType>(dict, owner, modelName, typeName),
    binary_(dict.lookupOrDefault<bool>("binary", true))
{}


template<class CloudType>
Foam::CloudToVTK<CloudType>::CloudToVTK
(
    const CloudToVTK<CloudType>& c
)
:
    CloudFunctionObject<CloudType>(c),
    binary_(c.binary_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
Foam::CloudToVTK<CloudType>::~CloudToVTK()
{}


// ************************************************************************* //
