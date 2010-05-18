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


#include <camp/property.hpp>
#include <camp/errors.hpp>
#include <camp/classvisitor.hpp>
#include <camp/userobject.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
Property::~Property()
{
}

//-------------------------------------------------------------------------------------------------
const std::string& Property::name() const
{
    return m_name;
}

//-------------------------------------------------------------------------------------------------
Type Property::type() const
{
    return m_type;
}

//-------------------------------------------------------------------------------------------------
bool Property::readable(const UserObject& object) const
{
    return isReadable() && m_readable.get(object);
}

//-------------------------------------------------------------------------------------------------
bool Property::writable(const UserObject& object) const
{
    return isWritable() && m_writable.get(object);
}

//-------------------------------------------------------------------------------------------------
Value Property::get(const UserObject& object) const
{
    // Check if the property is readable
    if (!readable(object))
        CAMP_ERROR(ForbiddenRead(name()));

    return getValue(object);
}

//-------------------------------------------------------------------------------------------------
void Property::set(const UserObject& object, const Value& value) const
{
    // Check if the property is writable
    if (!writable(object))
        CAMP_ERROR(ForbiddenWrite(name()));

    // Here we don't call setValue directly, we rather let the user object do it
    // and add any processing needed for proper propagation of the modification
    object.set(*this, value);
}

//-------------------------------------------------------------------------------------------------
void Property::accept(ClassVisitor& visitor) const
{
    visitor.visit(*this);
}

//-------------------------------------------------------------------------------------------------
bool Property::isReadable() const
{
    return true;
}

//-------------------------------------------------------------------------------------------------
bool Property::isWritable() const
{
    return true;
}

//-------------------------------------------------------------------------------------------------
Property::Property(const std::string& name, Type type)
    : m_name(name)
    , m_type(type)
    , m_readable(true)
    , m_writable(true)
{
}

} // namespace camp
