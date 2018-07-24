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

#include <ponder/userobject.hpp>
#include <ponder/userproperty.hpp>
#include <ponder/class.hpp>

namespace ponder {
    
const UserObject UserObject::nothing;

UserObject::UserObject()
    : m_class(nullptr)
    , m_holder()
{
}

UserObject::UserObject(const UserObject& other)
    : m_class(other.m_class)
    , m_holder(other.m_holder)
{
}

UserObject::UserObject(UserObject&& other) noexcept
{
    std::swap(m_class, other.m_class);
    m_holder.swap(other.m_holder);
}

UserObject& UserObject::operator = (const UserObject& other)
{
    m_class = other.m_class;
    m_holder = other.m_holder;
    return *this;
}

UserObject& UserObject::operator = (UserObject&& other) noexcept
{
    std::swap(m_class, other.m_class);
    m_holder.swap(other.m_holder);
    return *this;
}

void* UserObject::pointer() const
{
    return m_holder != nullptr ? m_holder->object() : nullptr;
}

const Class& UserObject::getClass() const
{
    if (m_class)
    {
        return *m_class;
    }
    else
    {
        PONDER_ERROR(NullObject(m_class));
    }
}

Value UserObject::get(IdRef property) const
{
    return getClass().property(property).get(*this);
}

Value UserObject::get(std::size_t index) const
{
    return getClass().property(index).get(*this);
}

void UserObject::set(IdRef property, const Value& value) const
{
    getClass().property(property).set(*this, value);
}

void UserObject::set(std::size_t index, const Value& value) const
{
    getClass().property(index).set(*this, value);
}

bool UserObject::operator == (const UserObject& other) const
{
    if (m_holder && other.m_holder)
    {
        return m_holder->object() == other.m_holder->object();
    }
    else if (!m_class && !other.m_class)
    {
        return true; // both are UserObject::nothing
    }

    return false;
}

bool UserObject::operator < (const UserObject& other) const
{
    if (m_holder)
    {
        if (other.m_holder)
        {
            return m_holder->object() < other.m_holder->object();
        }
    }
    assert(0);
    return false;
}

void UserObject::set(const Property& property, const Value& value) const
{
    if (m_holder)
    {
        // Just forward to the property, no extra processing required
        property.setValue(*this, value);
    }
    else
    {
        // Error, null object
        PONDER_ERROR(NullObject(m_class));
    }
}

} // namespace ponder
