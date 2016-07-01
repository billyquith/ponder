/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2016 Billy Quith.
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


#include <ponder/property.hpp>
#include <ponder/classvisitor.hpp>


namespace ponder
{
    
Property::~Property()
{
}

const std::string& Property::name() const
{
    return m_name;
}

ValueType Property::type() const
{
    return m_type;
}

bool Property::readable(const UserObject& object) const
{
    return isReadable() && m_readable.get(object);
}

bool Property::writable(const UserObject& object) const
{
    return isWritable() && m_writable.get(object);
}

Value Property::get(const UserObject& object) const
{
    // Check if the property is readable
    if (!readable(object))
        PONDER_ERROR(ForbiddenRead(name()));

    return getValue(object);
}

void Property::set(const UserObject& object, const Value& value) const
{
    // Check if the property is writable
    if (!writable(object))
        PONDER_ERROR(ForbiddenWrite(name()));

    // Here we don't call setValue directly, we rather let the user object do it
    // and add any processing needed for proper propagation of the modification
    object.set(*this, value);
}

void Property::accept(ClassVisitor& visitor) const
{
    visitor.visit(*this);
}

bool Property::isReadable() const
{
    return true;
}

bool Property::isWritable() const
{
    return true;
}

Property::Property(const std::string& name, ValueType type)
    : m_name(name)
    , m_type(type)
    , m_readable(true)
    , m_writable(true)
{
}

} // namespace ponder
