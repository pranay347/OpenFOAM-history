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

\*---------------------------------------------------------------------------*/

#include "fieldAverage.H"
#include "volFields.H"
#include "Time.H"

#include "fieldAverageItem.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
defineTypeNameAndDebug(fieldAverage, 0);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::fieldAverage::resetFields()
{
    forAll(faItems_, i)
    {
        if (faItems_[i].mean())
        {
            if (obr_.found(faItems_[i].meanFieldName()))
            {
                obr_.checkOut(*obr_[faItems_[i].meanFieldName()]);
            }
        }
        if (faItems_[i].prime2Mean())
        {
            if (obr_.found(faItems_[i].prime2MeanFieldName()))
            {
                obr_.checkOut(*obr_[faItems_[i].prime2MeanFieldName()]);
            }
        }
    }
}


void Foam::fieldAverage::initialize()
{
    resetFields();

    Info(log_)<< type() << " " << name_ << ":" << nl;


    // Add mean fields to the field lists
    forAll(faItems_, fieldI)
    {
        addMeanField<scalar>(fieldI);
        addMeanField<vector>(fieldI);
        addMeanField<sphericalTensor>(fieldI);
        addMeanField<symmTensor>(fieldI);
        addMeanField<tensor>(fieldI);
    }

    // Add prime-squared mean fields to the field lists
    forAll(faItems_, fieldI)
    {
        addPrime2MeanField<scalar, scalar>(fieldI);
        addPrime2MeanField<vector, symmTensor>(fieldI);
    }

    forAll(faItems_, fieldI)
    {
        if (!faItems_[fieldI].active())
        {
            WarningIn("void Foam::fieldAverage::initialize()")
                << "Field " << faItems_[fieldI].fieldName()
                << " not found in database for averaging";
        }
    }

    // ensure first averaging works unconditionally
    prevTimeIndex_ = -1;

    Info(log_)<< endl;

    initialised_ = true;
}


void Foam::fieldAverage::calcAverages()
{
    if (!initialised_)
    {
        initialize();
    }

    const label currentTimeIndex =
        static_cast<const fvMesh&>(obr_).time().timeIndex();

    if (prevTimeIndex_ == currentTimeIndex)
    {
        return;
    }
    else
    {
        prevTimeIndex_ = currentTimeIndex;
    }

    Info(log_)<< type() << " " << name_ << " output:" << nl;

    Info(log_)<< "    Calculating averages" << nl;

    addMeanSqrToPrime2Mean<scalar, scalar>();
    addMeanSqrToPrime2Mean<vector, symmTensor>();

    calculateMeanFields<scalar>();
    calculateMeanFields<vector>();
    calculateMeanFields<sphericalTensor>();
    calculateMeanFields<symmTensor>();
    calculateMeanFields<tensor>();

    calculatePrime2MeanFields<scalar, scalar>();
    calculatePrime2MeanFields<vector, symmTensor>();

    forAll(faItems_, fieldI)
    {
        totalIter_[fieldI]++;
        totalTime_[fieldI] += obr_.time().deltaTValue();
    }
}


void Foam::fieldAverage::writeAverages() const
{
    Info(log_)<< "    Writing average fields" << endl;

    writeFields<scalar>();
    writeFields<vector>();
    writeFields<sphericalTensor>();
    writeFields<symmTensor>();
    writeFields<tensor>();
}


void Foam::fieldAverage::writeAveragingProperties()
{
    forAll(faItems_, fieldI)
    {
        const word& fieldName = faItems_[fieldI].fieldName();

        dictionary propsDict;
        propsDict.add("totalIter", totalIter_[fieldI]);
        propsDict.add("totalTime", totalTime_[fieldI]);
        setProperty(fieldName, propsDict);
    }
}


void Foam::fieldAverage::readAveragingProperties()
{
    totalIter_.clear();
    totalIter_.setSize(faItems_.size(), 1);

    totalTime_.clear();
    totalTime_.setSize(faItems_.size(), obr_.time().deltaTValue());

    if (resetOnRestart_)
    {
        Info(log_)
            << "    Starting averaging at time " << obr_.time().timeName()
            << nl;
    }
    else
    {
        Info(log_)<< "    Restarting averaging for fields:" << nl;
        forAll(faItems_, fieldI)
        {
            const word& fieldName = faItems_[fieldI].fieldName();
            if (foundProperty(fieldName))
            {
                dictionary fieldDict;
                getProperty(fieldName, fieldDict);

                totalIter_[fieldI] = readLabel(fieldDict.lookup("totalIter"));
                totalTime_[fieldI] = readScalar(fieldDict.lookup("totalTime"));
                Info(log_)
                    << "        " << fieldName
                    << " iters = " << totalIter_[fieldI]
                    << " time = " << totalTime_[fieldI] << nl;
            }
            else
            {
                Info(log_)
                    << "        " << fieldName
                    << ": starting averaging at time "
                    << obr_.time().timeName() << endl;
            }
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::fieldAverage::fieldAverage
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
:
    functionObjectState(obr, name),
    obr_(obr),
    prevTimeIndex_(-1),
    resetOnRestart_(false),
    resetOnOutput_(false),
    log_(true),
    initialised_(false),
    faItems_(),
    totalIter_(),
    totalTime_()
{
    // Only active if a fvMesh is available
    if (setActive<fvMesh>())
    {
        read(dict);
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::fieldAverage::~fieldAverage()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::fieldAverage::read(const dictionary& dict)
{
    if (active_)
    {
        initialised_ = false;

        log_.readIfPresent("log", dict);

        Info(log_)<< type() << " " << name_ << ":" << nl;

        dict.readIfPresent("resetOnRestart", resetOnRestart_);
        dict.readIfPresent("resetOnOutput", resetOnOutput_);
        dict.lookup("fields") >> faItems_;

        readAveragingProperties();

        Info(log_)<< endl;
    }
}


void Foam::fieldAverage::execute()
{
    if (active_)
    {
        calcAverages();
        Info(log_)<< endl;
    }
}


void Foam::fieldAverage::end()
{
    if (active_)
    {
        execute();
    }
}


void Foam::fieldAverage::timeSet()
{}


void Foam::fieldAverage::write()
{
    if (active_)
    {
        writeAverages();
        writeAveragingProperties();

        if (resetOnOutput_)
        {
            Info(log_)
                << "    Restarting averaging at time " << obr_.time().timeName()
                << nl << endl;

            initialize();

            // ensure first averaging works unconditionally
            prevTimeIndex_ = -1;
        }

        Info(log_)<< endl;
    }
}


void Foam::fieldAverage::updateMesh(const mapPolyMesh&)
{
    // Do nothing
}


void Foam::fieldAverage::movePoints(const polyMesh&)
{
    // Do nothing
}


// ************************************************************************* //
