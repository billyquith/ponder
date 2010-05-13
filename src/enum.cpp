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


#include <camp/enum.hpp>
#include <camp/outofrange.hpp>
#include <camp/enumnamenotfound.hpp>
#include <camp/enumvaluenotfound.hpp>
#include <camp/detail/enummanager.hpp>
#include <algorithm>


namespace camp
{
//-------------------------------------------------------------------------------------------------
void Enum::undeclare(const std::string& name)
{
    detail::EnumManager::instance().removeClass(name);
}

//-------------------------------------------------------------------------------------------------
const std::string& Enum::name() const
{
    return m_name;
}

//-------------------------------------------------------------------------------------------------
std::size_t Enum::size() const
{
    return m_pairs.size();
}

//-------------------------------------------------------------------------------------------------
const Enum::Pair& Enum::pair(std::size_t index) const
{
    // Make sure that the index is not out of range
    if (index >= m_pairs.size())
        CAMP_ERROR(OutOfRange(index, m_pairs.size()));

    return m_pairs[index];
}

//-------------------------------------------------------------------------------------------------
bool Enum::hasName(const std::string& name) const
{
    const NameIndex& names = m_pairs.get<Name>();

    return names.find(name) != names.end();
}

//-------------------------------------------------------------------------------------------------
bool Enum::hasValue(long value) const
{
    const ValueIndex& values = m_pairs.get<Val>();

    return values.find(value) != values.end();
}

//-------------------------------------------------------------------------------------------------
const std::string& Enum::name(long value) const
{
    const ValueIndex& values = m_pairs.get<Val>();

    ValueIndex::const_iterator it = values.find(value);
    if (it == values.end())
        CAMP_ERROR(EnumValueNotFound(value, name()));

    return it->name;
}

//-------------------------------------------------------------------------------------------------
long Enum::value(const std::string& name) const
{
    const NameIndex& names = m_pairs.get<Name>();

    NameIndex::const_iterator it = names.find(name);
    if (it == names.end())
        CAMP_ERROR(EnumNameNotFound(name, m_name));

    return it->value;
}

//-------------------------------------------------------------------------------------------------
bool Enum::operator==(const Enum& other) const
{
    return m_name == other.m_name;
}

//-------------------------------------------------------------------------------------------------
bool Enum::operator!=(const Enum& other) const
{
    return m_name != other.m_name;
}

//-------------------------------------------------------------------------------------------------
Enum::Enum(const std::string& name)
    : m_name(name)
{
}

} // namespace camp
