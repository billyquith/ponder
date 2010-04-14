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


#include <camp/enumobject.hpp>
#include <camp/enum.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
long EnumObject::value() const
{
    return m_value;
}

//-------------------------------------------------------------------------------------------------
const std::string& EnumObject::name() const
{
    return m_enum->name(m_value);
}

//-------------------------------------------------------------------------------------------------
const Enum& EnumObject::getEnum() const
{
    return *m_enum;
}

//-------------------------------------------------------------------------------------------------
bool EnumObject::operator==(const EnumObject& other) const
{
    return (m_enum == other.m_enum) && (m_value == other.m_value);
}

//-------------------------------------------------------------------------------------------------
bool EnumObject::operator<(const EnumObject& other) const
{
    if (m_enum != other.m_enum)
    {
        return m_enum < other.m_enum;
    }
    else
    {
        return m_value < other.m_value;
    }
}

} // namespace camp
