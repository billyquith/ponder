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


#include <camp/detail/classmanager.hpp>
#include <camp/class.hpp>
#include <camp/classnotfound.hpp>
#include <camp/classalreadycreated.hpp>
#include <camp/outofrange.hpp>
#include <camp/observer.hpp>
#include <cassert>


namespace camp
{
namespace detail
{
//-------------------------------------------------------------------------------------------------
ClassManager& ClassManager::instance()
{
    static ClassManager manager;
    return manager;
}

//-------------------------------------------------------------------------------------------------
Class& ClassManager::registerNew(const std::string& name, const std::string& id)
{
    // First make sure that the class doesn't already exist
    if ((m_byName.find(name) != m_byName.end()) || (m_byId.find(id) != m_byId.end()))
        CAMP_ERROR(ClassAlreadyCreated(name, id));

    // Create the new class and insert it into the lookup tables
    ClassPtr newClass = ClassPtr(new Class(name));
    m_byName[name] = newClass;
    m_byId[id] = newClass;

    // Notify observers
    ObserverIterator end = observersEnd();
    for (ObserverIterator it = observersBegin(); it != end; ++it)
    {
        (*it)->classAdded(*newClass);
    }

    return *newClass;
}

//-------------------------------------------------------------------------------------------------
std::size_t ClassManager::count() const
{
    return m_byName.size();
}

//-------------------------------------------------------------------------------------------------
const Class& ClassManager::getByIndex(std::size_t index) const
{
    // Make sure that the index is not out of range
    if (index >= m_byName.size())
        CAMP_ERROR(OutOfRange(index, m_byName.size()));

    ClassByNameTable::const_iterator it = m_byName.begin();
    std::advance(it, index);

    return *it->second;
}

//-------------------------------------------------------------------------------------------------
const Class& ClassManager::getByName(const std::string& name) const
{
    ClassByNameTable::const_iterator it = m_byName.find(name);
    if (it == m_byName.end())
        CAMP_ERROR(ClassNotFound(name));

    return *it->second;
}

//-------------------------------------------------------------------------------------------------
const Class& ClassManager::getById(const std::string& id) const
{
    ClassByIdTable::const_iterator it = m_byId.find(id);
    if (it == m_byId.end())
        CAMP_ERROR(ClassNotFound(id));

    return *it->second;
}

//-------------------------------------------------------------------------------------------------
const Class* ClassManager::getByIdSafe(const std::string& id) const
{
    ClassByIdTable::const_iterator it = m_byId.find(id);
    if (it == m_byId.end())
        return 0;

    return &*it->second;
}

//-------------------------------------------------------------------------------------------------
bool ClassManager::classExists(const std::string& id) const
{
    return m_byId.find(id) != m_byId.end();
}

//-------------------------------------------------------------------------------------------------
ClassManager::ClassManager()
{
}

//-------------------------------------------------------------------------------------------------
ClassManager::~ClassManager()
{
    // Notify observers of classes destruction
    ClassByNameTable::const_iterator endClass = m_byName.end();
    ObserverIterator endObs = observersEnd();
    for (ClassByNameTable::const_iterator itClass = m_byName.begin(); itClass != endClass; ++itClass)
    {
        for (ObserverIterator itObs = observersBegin(); itObs != endObs; ++itObs)
        {
            (*itObs)->classRemoved(*itClass->second);
        }
    }
}

} // namespace detail

} // namespace camp
