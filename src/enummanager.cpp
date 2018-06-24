/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2018 Nick Trout.
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

Enum& EnumManager::addClass(IdRef id)
{
    // First make sure that the enum doesn't already exist
    if (enumExists(id))
    {
        PONDER_ERROR(EnumAlreadyCreated(id));
    }

    // Create the new class
    Enum* newEnum = new Enum(id);

    // Insert it into the table
    m_enums.insert(id, newEnum);

    // Notify observers
    notifyEnumAdded(*newEnum);

    // Done
    return *newEnum;
}
    
void EnumManager::removeClass(IdRef id)
{
    auto it = m_enums.findKey(id);
    if (it == m_enums.end())
        PONDER_ERROR(EnumNotFound(id));
    
    Enum *en = (*it).value();
    
    // Notify observers
    notifyEnumRemoved(*en);

    delete en;
    m_enums.erase(id);
}

std::size_t EnumManager::count() const
{
    return m_enums.size();
}

const Enum& EnumManager::getByIndex(std::size_t index) const
{
    // Make sure that the index is not out of range
    if (index >= m_enums.size())
        PONDER_ERROR(OutOfRange(index, m_enums.size()));

    EnumTable::const_iterator it = m_enums.begin();
    std::advance(it, index);

    return *it->second;
}

const Enum& EnumManager::getById(IdRef id) const
{
    auto it = m_enums.findKey(id);
    if (it == m_enums.end())
        PONDER_ERROR(EnumNotFound(id));

    return *it->second;
}

const Enum* EnumManager::getByIdSafe(IdRef id) const
{
    auto it = m_enums.findKey(id);
    return it == m_enums.end() ? nullptr : it->second;
}

bool EnumManager::enumExists(IdRef id) const
{
    return m_enums.findKey(id) != m_enums.end();
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
