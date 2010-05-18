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
#include <camp/errors.hpp>
#include <camp/observer.hpp>


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
Enum& EnumManager::addClass(const std::string& name, const std::string& id)
{
    const IdIndex&   ids   = m_enums.get<Id>();
    const NameIndex& names = m_enums.get<Name>();

    // First make sure that the enum doesn't already exist
    if ((ids.find(id) != ids.end()) || (names.find(name) != names.end()))
        CAMP_ERROR(EnumAlreadyCreated(name, id));

    // Create the new class
    boost::shared_ptr<Enum> newEnum(new Enum(name));

    // Insert it into the table
    EnumInfo info;
    info.id = id;
    info.name = name;
    info.enumPtr = newEnum;
    m_enums.insert(info);

    // Notify observers
    notifyEnumAdded(*newEnum);

    return *newEnum;
}

//-------------------------------------------------------------------------------------------------
void EnumManager::removeClass(const std::string& name)
{
    NameIndex& names = m_enums.get<Name>();

    // Make sure that the metaclass exists
    NameIndex::iterator it = names.find(name);
    if (it == names.end())
        CAMP_ERROR(EnumNotFound(name));

    // Notify observers
    notifyEnumRemoved(*it->enumPtr);

    // Remove it from the table
    names.erase(it);
}

//-------------------------------------------------------------------------------------------------
std::size_t EnumManager::count() const
{
    return m_enums.size();
}

//-------------------------------------------------------------------------------------------------
const Enum& EnumManager::getByIndex(std::size_t index) const
{
    // Make sure that the index is not out of range
    if (index >= m_enums.size())
        CAMP_ERROR(OutOfRange(index, m_enums.size()));

    EnumTable::const_iterator it = m_enums.begin();
    std::advance(it, index);

    return *it->enumPtr;
}

//-------------------------------------------------------------------------------------------------
const Enum& EnumManager::getByName(const std::string& name) const
{
    const NameIndex& names = m_enums.get<Name>();

    NameIndex::const_iterator it = names.find(name);
    if (it == names.end())
        CAMP_ERROR(EnumNotFound(name));

    return *it->enumPtr;
}

//-------------------------------------------------------------------------------------------------
const Enum& EnumManager::getById(const std::string& id) const
{
    const IdIndex& ids = m_enums.get<Id>();

    IdIndex::const_iterator it = ids.find(id);
    if (it == ids.end())
        CAMP_ERROR(EnumNotFound(id));

    return *it->enumPtr;
}

//-------------------------------------------------------------------------------------------------
const Enum* EnumManager::getByIdSafe(const std::string& id) const
{
    const IdIndex& ids = m_enums.get<Id>();

    IdIndex::const_iterator it = ids.find(id);
    if (it == ids.end())
        return 0;

    return &*it->enumPtr;
}

//-------------------------------------------------------------------------------------------------
bool EnumManager::enumExists(const std::string& id) const
{
    const IdIndex& ids = m_enums.get<Id>();

    return ids.find(id) != ids.end();
}

//-------------------------------------------------------------------------------------------------
EnumManager::EnumManager()
{
}

//-------------------------------------------------------------------------------------------------
EnumManager::~EnumManager()
{
    // Notify observers
    for (EnumTable::const_iterator it = m_enums.begin(); it != m_enums.end(); ++it)
    {
        notifyEnumRemoved(*it->enumPtr);
    }
}

} // namespace detail

} // namespace camp
