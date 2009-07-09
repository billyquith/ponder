/****************************************************************************
**
** Copyright (C) 2009 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** Commercial Usage
** Licensees holding valid CAMP Commercial licenses may use this file in
** accordance with the CAMP Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and TECHNOGERMA Systems France.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3.txt included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at contact@technogerma.fr.
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
