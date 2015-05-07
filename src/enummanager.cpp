/****************************************************************************
**
** This file is part of the CAMP library.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
** 
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
** 
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
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
    std::shared_ptr<Enum> newEnum(new Enum(name));

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
