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

// OpenFOAM includes
#include "functionObjectCloud.H"
#include "runTimePostProcessing.H"
#include "addToRunTimeSelectionTable.H"

// VTK includes
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataReader.h"
#include "vtkProperty.h"

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(functionObjectCloud, 0);
    addToRunTimeSelectionTable(pointData, functionObjectCloud, dictionary);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObjectCloud::functionObjectCloud
(
    const runTimePostProcessing& parent,
    const dictionary& dict,
    const HashPtrTable<DataEntry<vector>, word>& colours
)
:
    pointData(parent, dict, colours),
    fieldVisualisationBase(parent, dict, colours),
    cloudName_(dict.lookup("cloudName")),
    functionObject_(dict.lookup("functionObject")),
    colourFieldName_(dict.lookup("colourFieldName")),
    actor_()
{
    actor_ = vtkSmartPointer<vtkActor>::New();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::functionObjectCloud::~functionObjectCloud()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void Foam::functionObjectCloud::addGeometryToScene
(
    const scalar position,
    vtkRenderer* renderer
)
{
    if (!visible_)
    {
        return;
    }

    const dictionary& cloudDict =
        geometryBase::parent_.obr().lookupObject<IOdictionary>
        (
            cloudName_ + "OutputProperties"
        );

    fileName fName;
    if (cloudDict.found("cloudFunctionObject"))
    {
        const dictionary& foDict = cloudDict.subDict("cloudFunctionObject");
        if (foDict.found(functionObject_))
        {
            foDict.subDict(functionObject_).readIfPresent("file", fName);
        }
    }

    if (fName.empty())
    {
        WarningIn
        (
            "void Foam::functionObjectCloud::addToScene"
            "("
                "const scalar, "
                "vtkRenderer*"
            ")"
        )
            << "Unable to find function object " << functionObject_
            << " output for field " << fieldName_
            << ". Line will not be processed"
            << endl;
        return;
    }

    if (fName.ext() == "vtk")
    {
        vtkSmartPointer<vtkPolyDataReader> points =
            vtkSmartPointer<vtkPolyDataReader>::New();
        points->SetFileName(fName.c_str());
        points->Update();

        vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();

        actor_->SetMapper(mapper);

        addGlyphs
        (
            position,
            fieldName_,
            colourFieldName_,
            maxGlyphLength_,
            points->GetOutput(),
            actor_,
            renderer
        );

        renderer->AddActor(actor_);
    }
}


void Foam::functionObjectCloud::updateActors(const scalar position)
{
    actor_->GetProperty()->SetOpacity(opacity(position));

    vector pc = pointColour_->value(position);
    actor_->GetProperty()->SetColor(pc[0], pc[1], pc[2]);
}


// ************************************************************************* //
