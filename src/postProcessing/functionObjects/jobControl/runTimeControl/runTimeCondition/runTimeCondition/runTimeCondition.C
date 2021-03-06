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

#include "runTimeCondition.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(runTimeCondition, 0);
    defineRunTimeSelectionTable(runTimeCondition, dictionary);
}


// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

Foam::dictionary& Foam::runTimeCondition::setConditionDict()
{
    dictionary& propertyDict = state_.propertyDict();

    if (!propertyDict.found(name_))
    {
        propertyDict.add(name_, dictionary());
    }

    return propertyDict.subDict(name_);
}


const Foam::dictionary& Foam::runTimeCondition::conditionDict() const
{
    return conditionDict_;
}


Foam::dictionary& Foam::runTimeCondition::conditionDict()
{
    return conditionDict_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::runTimeCondition::runTimeCondition
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    functionObjectState& state
)
:
    name_(name),
    obr_(obr),
    state_(state),
    active_(dict.lookupOrDefault<bool>("active", true)),
    conditionDict_(setConditionDict()),
    log_(dict.lookupOrDefault("log", true)),
    groupID_(dict.lookupOrDefault("groupID", -1))
{}


// * * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * //

Foam::runTimeCondition::~runTimeCondition()
{}


// * * * * * * * * * * * * * * Public Member Functions * * * * * * * * * * * //

const Foam::word& Foam::runTimeCondition::name() const
{
    return name_;
}


bool Foam::runTimeCondition::active() const
{
    return active_;
}


Foam::label Foam::runTimeCondition::groupID() const
{
    return groupID_;
}


// ************************************************************************* //
