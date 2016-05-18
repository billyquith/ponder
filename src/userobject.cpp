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


#include <ponder/userobject.hpp>
#include <ponder/userproperty.hpp>
#include <ponder/class.hpp>


namespace ponder
{
    
const UserObject UserObject::nothing;

UserObject::UserObject()
    : m_class(nullptr)
    , m_holder()
    , m_parent()
    , m_child(nullptr)
{
}

UserObject::UserObject(const UserObject& parent, const UserProperty& member)
    : m_class(&member.getClass())
    , m_holder()
    , m_parent(new ParentObject(parent, member))
    , m_child(nullptr)
{
    m_parent->object.m_child = this;
}

UserObject::UserObject(const UserObject& copy)
    : m_class(copy.m_class)
    , m_holder(copy.m_holder)
    , m_parent(copy.m_parent
               ? new ParentObject(copy.m_parent->object, copy.m_parent->member)
               : nullptr)
{
    if (m_parent)
        m_parent->object.m_child = this;
}

void* UserObject::pointer() const
{
    if (m_holder)
    {
        return m_holder->object();
    }
    else if (m_parent)
    {
        // warning: this may not be multi-thread safe,
        // but it is required to make the returned value persistent
        m_parent->lastValue = m_parent->member.get(m_parent->object).to<UserObject>();
        return m_parent->lastValue.pointer();
    }
    else
    {
        return nullptr;
    }
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

Value UserObject::get(const std::string& property) const
{
    return getClass().property(property).get(*this);
}

Value UserObject::get(std::size_t index) const
{
    return getClass().property(index).get(*this);
}

void UserObject::set(const std::string& property, const Value& value) const
{
    getClass().property(property).set(*this, value);
}

void UserObject::set(std::size_t index, const Value& value) const
{
    getClass().property(index).set(*this, value);
}

Value UserObject::call(const std::string& function, const Args& args) const
{
    return getClass().function(function).call(*this, args);
}

UserObject& UserObject::operator=(const UserObject& other)
{
    std::unique_ptr<ParentObject>
        parent(other.m_parent
               ? new ParentObject(other.m_parent->object, other.m_parent->member)
               : nullptr);

    m_class = other.m_class;
    m_holder = other.m_holder;
    m_parent.swap(parent);
    m_child = nullptr;

    if (m_parent)
        m_parent->object.m_child = this;

    return *this;
}

bool UserObject::operator == (const UserObject& other) const
{
    if (m_holder && other.m_holder)
    {
        return m_holder->object() == other.m_holder->object();
    }
    else if (m_parent && other.m_parent)
    {
        return (&m_parent->member == &other.m_parent->member)
            && (m_parent->object == other.m_parent->object);
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
        else
        {
            return other.m_parent != nullptr;
        }
    }
    else if (m_parent)
    {
        if (other.m_parent)
        {
            if (&m_parent->member != &other.m_parent->member)
                return &m_parent->member < &other.m_parent->member;
            else
                return m_parent->object < other.m_parent->object;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return other.m_holder || other.m_parent;
    }
}

void UserObject::set(const Property& property, const Value& value) const
{
    if (m_holder)
    {
        // Just forward to the property, no extra processing required
        property.setValue(*this, value);
    }
    else if (m_parent)
    {
        // Find the root object
        const UserObject* root = this;
        while (root->m_parent->object.m_parent)
            root = &root->m_parent->object;

        // Launch the recursive process
        root->cascadeSet(root->m_parent->object, property, value);
    }
    else
    {
        // Error, null object
        PONDER_ERROR(NullObject(m_class));
    }
}

void UserObject::cascadeSet(const UserObject& object,
                            const Property& property,
                            const Value& value) const
{
    // @todo Manually check the access (read / write) to the properties,
    // as we bypass the standard path?

    // Get the temporary object to modify
    UserObject sub = m_parent->member.getValue(object).to<UserObject>();

    // Make sure that the underlying object is *really* modifiable.
    // If m_holder was holding a const reference to an object, this will
    // replace it with a holder storing a copy of the object.
    detail::AbstractObjectHolder* holder = sub.m_holder->getWritable();
    bool same = (holder == sub.m_holder.get());
    if (!same)
        sub.m_holder.reset(holder);

    if (m_child)
    {
        // ...continue to recurse if it's not the actual property to modify
        m_child->cascadeSet(sub, property, value);
    }
    else
    {
        // ...apply the value if it's the actual property to modify
        property.setValue(sub, value);
    }

    // Assign the modified object back to its parent
    m_parent->member.setValue(object, sub);
}

} // namespace ponder
