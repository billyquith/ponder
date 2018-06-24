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

#include <ponder/arrayproperty.hpp>
#include <ponder/classvisitor.hpp>

namespace ponder {
    
ArrayProperty::ArrayProperty(IdRef name, ValueKind elementType, bool dynamic)
    : Property(name, ValueKind::Array)
    , m_elementType(elementType)
    , m_dynamic(dynamic)
{
}

ArrayProperty::~ArrayProperty()
{
}

ValueKind ArrayProperty::elementType() const
{
    return m_elementType;
}

bool ArrayProperty::dynamic() const
{
    return m_dynamic;
}

std::size_t ArrayProperty::size(const UserObject& object) const
{
    // Check if the property is readable
    if (!isReadable())
        PONDER_ERROR(ForbiddenRead(name()));

    return getSize(object);
}

void ArrayProperty::resize(const UserObject& object, std::size_t newSize) const
{
    // Check if the array is dynamic
    if (!dynamic())
        PONDER_ERROR(ForbiddenWrite(name()));

    // Check if the property is writable
    if (!isWritable())
        PONDER_ERROR(ForbiddenWrite(name()));

    setSize(object, newSize);
}

Value ArrayProperty::get(const UserObject& object, std::size_t index) const
{
    // Check if the property is readable
    if (!isReadable())
        PONDER_ERROR(ForbiddenRead(name()));

    // Make sure that the index is not out of range
    const std::size_t range = size(object);
    if (index >= range)
        PONDER_ERROR(OutOfRange(index, range));

    return getElement(object, index);
}

void ArrayProperty::set(const UserObject& object, std::size_t index, const Value& value) const
{
    // Check if the property is writable
    if (!isWritable())
        PONDER_ERROR(ForbiddenWrite(name()));

    // Check if the index is in range
    const std::size_t range = size(object);
    if (index >= range)
        PONDER_ERROR(OutOfRange(index, range));

    return setElement(object, index, value);
}

void ArrayProperty::insert(const UserObject& object, std::size_t before, const Value& value) const
{
    // Check if the array is dynamic
    if (!dynamic())
        PONDER_ERROR(ForbiddenWrite(name()));

    // Check if the property is writable
    if (!isWritable())
        PONDER_ERROR(ForbiddenWrite(name()));

    // Check if the index is in range
    const std::size_t range = size(object) + 1;
    if (before >= range)
        PONDER_ERROR(OutOfRange(before, range));

    return insertElement(object, before, value);
}

void ArrayProperty::remove(const UserObject& object, std::size_t index) const
{
    // Check if the array is dynamic
    if (!dynamic())
        PONDER_ERROR(ForbiddenWrite(name()));

    // Check if the property is writable
    if (!isWritable())
        PONDER_ERROR(ForbiddenWrite(name()));

    // Check if the index is in range
    const std::size_t range = size(object);
    if (index >= range)
        PONDER_ERROR(OutOfRange(index, range));

    return removeElement(object, index);
}

void ArrayProperty::accept(ClassVisitor& visitor) const
{
    visitor.visit(*this);
}

Value ArrayProperty::getValue(const UserObject& object) const
{
    // Return first element
    return get(object, 0);
}

void ArrayProperty::setValue(const UserObject& object, const Value& value) const
{
    // Set first element
    set(object, 0, value);
}

} // namespace ponder
