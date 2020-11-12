/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2020 Nick Trout.
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

#include <ponder/detail/enummanager.hpp>
#include <ponder/enum.hpp>
#include <ponder/errors.hpp>

namespace ponder {
namespace detail {
    
EnumManager& EnumManager::instance()
{
    static EnumManager manager;
    return manager;
}

Enum& EnumManager::addClass(TypeId const& id, IdRef name)
{
    // First make sure that the enum doesn't already exist
    if (enumExists(id) || (!name.empty() && getByNameSafe(name) != nullptr))
    {
        PONDER_ERROR(EnumAlreadyCreated(name));
    }

    // Create the new class
    Enum* newEnum = new Enum(name);

    // Insert it into the table
    m_enums.insert(std::make_pair(id, newEnum));
    m_names.insert(std::make_pair(name, newEnum));

    // Notify observers
    notifyEnumAdded(*newEnum);

    // Done
    return *newEnum;
}
    
void EnumManager::removeClass(TypeId const& id)
{
    auto it = m_enums.find(id);
    if (it == m_enums.end())
        return; //PONDER_ERROR(EnumNotFound(id));
    
    Enum *en{ it->second };
    
    // Notify observers
    notifyEnumRemoved(*en);

    m_names.erase(en->name());
    delete en;
    m_enums.erase(id);
}

size_t EnumManager::count() const
{
    return m_enums.size();
}

const Enum* EnumManager::getByIdSafe(TypeId const& id) const
{
    const auto it = m_enums.find(id);
    return it == m_enums.end() ? nullptr : it->second;
}

const Enum& EnumManager::getById(TypeId const& id) const
{
    const Enum* e{ getByIdSafe(id) };
    if (!e)
        PONDER_ERROR(EnumNotFound("?"));

    return *e;
}

const Enum* EnumManager::getByNameSafe(const IdRef name) const 
{
    const auto it{ std::find_if(m_names.begin(), m_names.end(),
        [&name](const auto& a) { return a.first.compare(name.data()) == 0; } ) };

    return it == m_names.end() ? nullptr : it->second;
}

const Enum& EnumManager::getByName(const IdRef name) const
{
    const Enum* e{ getByNameSafe(name) };
    if (!e)
        PONDER_ERROR(EnumNotFound(name));

    return *e;
}

bool EnumManager::enumExists(TypeId const& id) const
{
    return m_enums.find(id) != m_enums.end();
}

EnumManager::EnumManager()
{
}

EnumManager::~EnumManager()
{
    // Notify observers
    for (EnumTable::const_iterator it = m_enums.begin(); it != m_enums.end(); ++it)
    {
        Enum* enumPtr = it->second;
        notifyEnumRemoved(*enumPtr);
        delete enumPtr;
    }
}

} // namespace detail
} // namespace ponder
