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


#include <camp/detail/enummanager.hpp>
#include <camp/enum.hpp>
#include <camp/invalidenum.hpp>
#include <camp/invalidindex.hpp>
#include <camp/observer.hpp>
#include <cassert>


namespace camp
{
namespace detail
{
//-------------------------------------------------------------------------------------------------
EnumManager& EnumManager::instance()
{
    static EnumManager manager;
    return manager;
}

//-------------------------------------------------------------------------------------------------
Enum& EnumManager::registerNew(const std::string& name, const std::string& id)
{
    // Create the new enum and insert it into the main table
    EnumPtr newEnum = EnumPtr(new Enum(name));
    std::pair<EnumByNameTable::iterator, bool> result = m_byName.insert(std::make_pair(name, newEnum));

    // If this name already exists in the table, report an error (duplicate enums are not allowed)
    if (!result.second)
        CAMP_ERROR(InvalidEnum(name.c_str()));

    // Insert a pointer to the new enum in the id table
    m_byId[id].push_back(newEnum);

    // Notify observers
    ObserverIterator end = observersEnd();
    for (ObserverIterator it = observersBegin(); it != end; ++it)
    {
        (*it)->enumAdded(*newEnum);
    }

    return *newEnum;
}

//-------------------------------------------------------------------------------------------------
const Enum& EnumManager::getByName(const std::string& name) const
{
    EnumByNameTable::const_iterator it = m_byName.find(name);
    if (it == m_byName.end())
        CAMP_ERROR(InvalidEnum(name.c_str()));

    return *it->second;
}

//-------------------------------------------------------------------------------------------------
std::size_t EnumManager::count(const std::string& id) const
{
    EnumByIdTable::const_iterator it = m_byId.find(id);
    if (it == m_byId.end())
        return 0;

    return it->second.size();
}

//-------------------------------------------------------------------------------------------------
const Enum& EnumManager::getById(const std::string& id, std::size_t index) const
{
    // First retrieve the array of enums associated to the given identifier
    EnumByIdTable::const_iterator it = m_byId.find(id);
    if (it == m_byId.end())
        CAMP_ERROR(InvalidEnum(id.c_str()));

    // Make sure the index is valid
    if (index >= it->second.size())
        CAMP_ERROR(InvalidIndex(index, it->second.size()));

    return *it->second[index];
}

//-------------------------------------------------------------------------------------------------
const Enum* EnumManager::getByIdSafe(const std::string& id, std::size_t index) const
{
    // First retrieve the array of enums associated to the given identifier
    EnumByIdTable::const_iterator it = m_byId.find(id);
    if (it == m_byId.end())
        return 0;

    // Make sure the index is valid
    if (index >= it->second.size())
        return 0;

    return &*it->second[index];
}

//-------------------------------------------------------------------------------------------------
std::size_t EnumManager::count() const
{
    return m_byName.size();
}

//-------------------------------------------------------------------------------------------------
const Enum& EnumManager::getByIndex(std::size_t index) const
{
    // Make sure the index is valid
    if (index >= m_byName.size())
        CAMP_ERROR(InvalidIndex(index, m_byName.size()));

    EnumByNameTable::const_iterator it = m_byName.begin();
    std::advance(it, index);

    return *it->second;
}

//-------------------------------------------------------------------------------------------------
bool EnumManager::enumExists(const std::string& id) const
{
    return m_byId.find(id) != m_byId.end();
}

//-------------------------------------------------------------------------------------------------
EnumManager::EnumManager()
{
}

//-------------------------------------------------------------------------------------------------
EnumManager::~EnumManager()
{
    // Notify observers of enums destruction
    EnumByNameTable::const_iterator endEnum = m_byName.end();
    ObserverIterator endObs = observersEnd();
    for (EnumByNameTable::const_iterator itEnum = m_byName.begin(); itEnum != endEnum; ++itEnum)
    {
        for (ObserverIterator itObs = observersBegin(); itObs != endObs; ++itObs)
        {
            (*itObs)->enumRemoved(*itEnum->second);
        }
    }
}

} // namespace detail

} // namespace camp
