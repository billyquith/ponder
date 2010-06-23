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


#include <camp/arrayproperty.hpp>
#include <camp/classvisitor.hpp>
#include <camp/invalidaccess.hpp>
#include <camp/invalidobject.hpp>
#include <camp/invalidvalue.hpp>
#include <camp/invalidindex.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
ArrayProperty::ArrayProperty(const std::string& name, Type elementType, bool dynamic)
    : Property(name, arrayType)
    , m_elementType(elementType)
    , m_dynamic(dynamic)
{
}

//-------------------------------------------------------------------------------------------------
ArrayProperty::~ArrayProperty()
{
}

//-------------------------------------------------------------------------------------------------
Type ArrayProperty::elementType() const
{
    return m_elementType;
}

//-------------------------------------------------------------------------------------------------
bool ArrayProperty::dynamic() const
{
    return m_dynamic;
}

//-------------------------------------------------------------------------------------------------
std::size_t ArrayProperty::size(const UserObject& object) const
{
    // Check if the property is readable
    if (!readable(object))
        CAMP_ERROR(InvalidAccess(name().c_str(), InvalidAccess::Read));

    return getSize(object);
}

//-------------------------------------------------------------------------------------------------
void ArrayProperty::resize(const UserObject& object, std::size_t newSize) const
{
    // Check if the array is dynamic
    if (!dynamic())
        CAMP_ERROR(InvalidAccess(name().c_str(), InvalidAccess::Write));

    // Check if the property is writable
    if (!writable(object))
        CAMP_ERROR(InvalidAccess(name().c_str(), InvalidAccess::Write));

    std::size_t currentSize = size(object);
    if (newSize > currentSize)
    {
        // Add elements to reach the new size
        while (currentSize < newSize)
            insertElement(object, currentSize++);
    }
    else if (newSize < currentSize)
    {
        // Remove elements to reach the new size
        while (currentSize > newSize)
            removeElement(object, --currentSize);
    }
}

//-------------------------------------------------------------------------------------------------
Value ArrayProperty::get(const UserObject& object, std::size_t index) const
{
    // Check if the property is readable
    if (!readable(object))
        CAMP_ERROR(InvalidAccess(name().c_str(), InvalidAccess::Read));

    // Check if the index is in range
    std::size_t range = size(object);
    if (index >= range)
        CAMP_ERROR(InvalidIndex(index, range));

    return getElement(object, index);
}

//-------------------------------------------------------------------------------------------------
void ArrayProperty::set(const UserObject& object, std::size_t index, const Value& value) const
{
    // Check if the property is writable
    if (!writable(object))
        CAMP_ERROR(InvalidAccess(name().c_str(), InvalidAccess::Write));

    // Check if the index is in range
    std::size_t range = size(object);
    if (index >= range)
        CAMP_ERROR(InvalidIndex(index, range));

    return setElement(object, index, value);
}

//-------------------------------------------------------------------------------------------------
void ArrayProperty::insert(const UserObject& object, std::size_t before, const Value& value) const
{
    // Check if the array is dynamic
    if (!dynamic())
        CAMP_ERROR(InvalidAccess(name().c_str(), InvalidAccess::Write));

    // Check if the property is writable
    if (!writable(object))
        CAMP_ERROR(InvalidAccess(name().c_str(), InvalidAccess::Write));

    // Check if the index is in range
    std::size_t range = size(object) + 1;
    if (before >= range)
        CAMP_ERROR(InvalidIndex(before, range));

    return insertElement(object, before, value);
}

//-------------------------------------------------------------------------------------------------
void ArrayProperty::remove(const UserObject& object, std::size_t index) const
{
    // Check if the array is dynamic
    if (!dynamic())
        CAMP_ERROR(InvalidAccess(name().c_str(), InvalidAccess::Write));

    // Check if the property is writable
    if (!writable(object))
        CAMP_ERROR(InvalidAccess(name().c_str(), InvalidAccess::Write));

    // Check if the index is in range
    std::size_t range = size(object);
    if (index >= range)
        CAMP_ERROR(InvalidIndex(index, range));

    return removeElement(object, index);
}

//-------------------------------------------------------------------------------------------------
void ArrayProperty::accept(ClassVisitor& visitor) const
{
    visitor.visit(*this);
}

//-------------------------------------------------------------------------------------------------
Value ArrayProperty::getValue(const UserObject& object) const
{
    // Return first element
    return get(object, 0);
}

//-------------------------------------------------------------------------------------------------
void ArrayProperty::setValue(const UserObject& object, const Value& value) const
{
    // Set first element
    set(object, 0, value);
}

} // namespace camp
