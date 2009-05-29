// <insert copyright>

#include <camp/userobject.hpp>
#include <camp/userproperty.hpp>


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
        static UserObject o;
        o = m_parent->member.get(m_parent->object).to<UserObject>();
        return o.pointer();
        //return m_parent->member.get(m_parent->object).to<UserObject>().pointer();
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
UserObject& UserObject::operator=(const UserObject& other)
{
    m_holder = other.m_holder;
    m_parent.reset(other.m_parent ? new ParentObject(other.m_parent->object, other.m_parent->member) : 0);

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
    // Get the temporary object to modify
    camp::Value val = m_parent->member.get(object);
    const UserObject& sub = val.to<UserObject>();

    // Modify it
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
    m_parent->member.set(object, sub);
}

} // namespace camp
