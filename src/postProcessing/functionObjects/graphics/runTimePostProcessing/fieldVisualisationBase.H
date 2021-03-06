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
    Foam::fieldVisualisationBase

Description

SourceFiles
    fieldVisualisationBase.C

\*---------------------------------------------------------------------------*/

#ifndef fieldVisualisationBase_H
#define fieldVisualisationBase_H

#include "dictionary.H"
#include "Tuple2.H"
#include "NamedEnum.H"
#include "vector.H"
#include "HashPtrTable.H"
#include "DataEntry.H"

#include "vtkSmartPointer.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

class vtkActor;
class vtkLookupTable;
class vtkPolyData;
class vtkPolyDataMapper;
class vtkRenderer;


class vtkMapper;

namespace Foam
{

class runTimePostProcessing;

/*---------------------------------------------------------------------------*\
                   Class fieldVisualisationBase Declaration
\*---------------------------------------------------------------------------*/

class fieldVisualisationBase
{
public:

    // Public enumerations

        enum colourByType
        {
            cbColour,
            cbField
        };

        static const NamedEnum<colourByType, 2> colourByTypeNames;

        enum colourMapType
        {
            cmRainbow,
            cmBlueWhiteRed,
            cmFire,
            cmGreyscale
        };

        static const NamedEnum<colourMapType, 4> colourMapTypeNames;


private:

    // Private data

        //- Reference to the parent function object
        const runTimePostProcessing& parent_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        fieldVisualisationBase(const fieldVisualisationBase&);

        //- Disallow default bitwise assignment
        void operator=(const fieldVisualisationBase&);


protected:

    // Protected data

        struct scalarBar
        {
            bool visible_;
            bool vertical_;
            Tuple2<scalar, scalar> position_;
            string title_;
            label fontSize_;
            string labelFormat_;
            label numberOfLabels_;
        };

        //- Colours
        const HashPtrTable<DataEntry<vector>, word>& colours_;

        //- Field name
        word fieldName_;

        //- Colour by type
        colourByType colourBy_;

        //- Colour map type
        colourMapType colourMap_;

        //- Range of values
        Tuple2<scalar, scalar> range_;

        //- Scalar bar
        scalarBar scalarBar_;


    // Protected Member Functions

        //- Set the colour map
        void setColourMap(vtkLookupTable* lut) const;

        //- Add scalar bar to renderer
        void addScalarBar
        (
            const scalar position,
            vtkRenderer* renderer,
            vtkLookupTable* lut
        ) const;

        //- Set field/configure mapper, add scalar bar
        void setField
        (
            const scalar position,
            const word& colourFieldName,
            vtkPolyDataMapper* mapper,
            vtkRenderer* renderer
        ) const;

        //- Add glyphs
        void addGlyphs
        (
            const scalar position,
            const word& scaleFieldName,
            const word& colourFieldName,
            const scalar maxGlyphLength,
            vtkPolyData* data,
            vtkActor* actor,
            vtkRenderer* renderer
        ) const;


public:

    // Constructors

        //- Construct from dictionary
        fieldVisualisationBase
        (
            const runTimePostProcessing& parent,
            const dictionary& dict,
            const HashPtrTable<DataEntry<vector>, word>& colours
        );


    //- Destructor
    virtual ~fieldVisualisationBase();


    // Member Functions

        // Access

            //- Return the colours
            const HashPtrTable<DataEntry<vector>, word>& colours() const;

            //- Return the field name
            const word& fieldName() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
