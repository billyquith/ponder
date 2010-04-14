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



#include <camp/value.hpp>
#include <iostream>


namespace camp
{
const Value Value::nothing;

//-------------------------------------------------------------------------------------------------
Value::Value()
    : m_value(NoType())
    , m_type(noType)
{
}

//-------------------------------------------------------------------------------------------------
Value::Value(const Value& other)
    : m_value(other.m_value)
    , m_type(other.m_type)
{
}

//-------------------------------------------------------------------------------------------------
Type Value::type() const
{
    return m_type;
}

//-------------------------------------------------------------------------------------------------
bool Value::operator==(const Value& other) const
{
    return visit(detail::EqualVisitor(), other);
}

//-------------------------------------------------------------------------------------------------
bool Value::operator<(const Value& other) const
{
    return visit(detail::LessThanVisitor(), other);
}

//-------------------------------------------------------------------------------------------------
CAMP_API std::ostream& operator<<(std::ostream& stream, const Value& value)
{
    value.visit(detail::PrintVisitor(stream));
    return stream;
}

} // namespace camp
