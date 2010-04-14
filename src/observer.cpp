/****************************************************************************
**
** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** CAMP is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** 
** CAMP is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public License
** along with CAMP.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/


#include <camp/observer.hpp>
#include <camp/detail/classmanager.hpp>
#include <camp/detail/enummanager.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
Observer::~Observer()
{
    // Nothing to do
}

//-------------------------------------------------------------------------------------------------
void Observer::classAdded(const Class&)
{
    // Default implementation does nothing
}

//-------------------------------------------------------------------------------------------------
void Observer::classRemoved(const Class&)
{
    // Default implementation does nothing
}

//-------------------------------------------------------------------------------------------------
void Observer::enumAdded(const Enum&)
{
    // Default implementation does nothing
}

//-------------------------------------------------------------------------------------------------
void Observer::enumRemoved(const Enum&)
{
    // Default implementation does nothing
}

//-------------------------------------------------------------------------------------------------
Observer::Observer()
{
    // Nothing to do
}

//-------------------------------------------------------------------------------------------------
CAMP_API void addObserver(Observer* observer)
{
    detail::ClassManager::instance().addObserver(observer);
    detail::EnumManager::instance().addObserver(observer);
}

//-------------------------------------------------------------------------------------------------
CAMP_API void removeObserver(Observer* observer)
{
    detail::ClassManager::instance().removeObserver(observer);
    detail::EnumManager::instance().removeObserver(observer);
}

} // namespace camp
