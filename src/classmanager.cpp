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
#include <camp/invalidclass.hpp>
#include <camp/invalidindex.hpp>
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
    // Create the new class and insert it into the main table
    ClassPtr newClass = ClassPtr(new Class(name));
    std::pair<ClassByNameTable::iterator, bool> result = m_byName.insert(std::make_pair(name, newClass));

    // If this name already exists in the table, report an error (duplicate classes are not allowed)
    if (!result.second)
        CAMP_ERROR(InvalidClass(name.c_str()));

    // Insert a pointer to the new class in the id table
    m_byId[id].push_back(newClass);

    // Notify observers
    ObserverIterator end = observersEnd();
    for (ObserverIterator it = observersBegin(); it != end; ++it)
    {
        (*it)->classAdded(*newClass);
    }

    return *newClass;
}

//-------------------------------------------------------------------------------------------------
const Class& ClassManager::getByName(const std::string& name) const
{
    ClassByNameTable::const_iterator it = m_byName.find(name);
    if (it == m_byName.end())
        CAMP_ERROR(InvalidClass(name.c_str()));

    return *it->second;
}

//-------------------------------------------------------------------------------------------------
std::size_t ClassManager::count(const std::string& id) const
{
    ClassByIdTable::const_iterator it = m_byId.find(id);
    if (it == m_byId.end())
        return 0;

    return it->second.size();
}

//-------------------------------------------------------------------------------------------------
const Class& ClassManager::getById(const std::string& id, std::size_t index) const
{
    // First retrieve the array of classes associated to the given identifier
    ClassByIdTable::const_iterator it = m_byId.find(id);
    if (it == m_byId.end())
        CAMP_ERROR(InvalidClass(id.c_str()));

    // Make sure the index is valid
    if (index >= it->second.size())
        CAMP_ERROR(InvalidIndex(index, it->second.size()));

    return *it->second[index];
}

//-------------------------------------------------------------------------------------------------
std::size_t ClassManager::count() const
{
    return m_byName.size();
}

//-------------------------------------------------------------------------------------------------
const Class& ClassManager::getByIndex(std::size_t index) const
{
    // Make sure the index is valid
    if (index >= m_byName.size())
        CAMP_ERROR(InvalidIndex(index, m_byName.size()));

    ClassByNameTable::const_iterator it = m_byName.begin();
    std::advance(it, index);

    return *it->second;
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
