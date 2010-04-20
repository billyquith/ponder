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


#include <camp/userobject.hpp>
#include <camp/userproperty.hpp>
#include <camp/class.hpp>
#include <camp/function.hpp>
#include <camp/property.hpp>


namespace camp
{
const UserObject UserObject::nothing;

//-------------------------------------------------------------------------------------------------
UserObject::UserObject()
    : m_class(0)
    , m_holder()
    , m_parent()
    , m_child(0)
{
}

//-------------------------------------------------------------------------------------------------
UserObject::UserObject(const UserObject& parent, const UserProperty& member)
    : m_class(&member.getClass())
    , m_holder()
    , m_parent(new ParentObject(parent, member))
    , m_child(0)
{
    m_parent->object.m_child = this;
}

//-------------------------------------------------------------------------------------------------
UserObject::UserObject(const UserObject& copy)
    : m_class(copy.m_class)
    , m_holder(copy.m_holder)
    , m_parent(copy.m_parent ? new ParentObject(copy.m_parent->object, copy.m_parent->member) : 0)
{
    if (m_parent)
        m_parent->object.m_child = this;
}

//-------------------------------------------------------------------------------------------------
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
        return 0;
    }
}

//-------------------------------------------------------------------------------------------------
const Class& UserObject::getClass() const
{
    if (m_class)
    {
        return *m_class;
    }
    else
    {
        CAMP_ERROR(InvalidObject(*this));
    }
}

//-------------------------------------------------------------------------------------------------
Value UserObject::get(const std::string& property) const
{
    return getClass().property(property).get(*this);
}

//-------------------------------------------------------------------------------------------------
void UserObject::set(const std::string& property, const Value& value) const
{
    getClass().property(property).set(*this, value);
}

//-------------------------------------------------------------------------------------------------
Value UserObject::call(const std::string& function, const Args& args) const
{
    return getClass().function(function).call(*this, args);
}

//-------------------------------------------------------------------------------------------------
UserObject& UserObject::operator=(const UserObject& other)
{
    boost::scoped_ptr<ParentObject> parent(other.m_parent ? new ParentObject(other.m_parent->object, other.m_parent->member) : 0);

    m_class = other.m_class;
    m_holder = other.m_holder;
    m_parent.swap(parent);
    m_child = 0;

    if (m_parent)
        m_parent->object.m_child = this;

    return *this;
}

//-------------------------------------------------------------------------------------------------
bool UserObject::operator==(const UserObject& other) const
{
    if (m_holder && other.m_holder)
    {
        return m_holder->object() == other.m_holder->object();
    }
    else if (m_parent && other.m_parent)
    {
        return (&m_parent->member == &other.m_parent->member) && (m_parent->object == other.m_parent->object);
    }
    else
    {
        return false;
    }
}

//-------------------------------------------------------------------------------------------------
bool UserObject::operator<(const UserObject& other) const
{
    if (m_holder)
    {
        if (other.m_holder)
        {
            return m_holder->object() < other.m_holder->object();
        }
        else
        {
            return other.m_parent != 0;
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

//-------------------------------------------------------------------------------------------------
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
        CAMP_ERROR(InvalidObject(*this));
    }
}

//-------------------------------------------------------------------------------------------------
void UserObject::cascadeSet(const UserObject& object, const Property& property, const Value& value) const
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

} // namespace camp
