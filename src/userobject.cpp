// <insert copyright>

#include <camp/userobject.hpp>
#include <camp/detail/objectholderbyprop.hpp>
#include <camp/property.hpp>


namespace camp
{
const UserObject UserObject::nothing;

//-------------------------------------------------------------------------------------------------
UserObject::UserObject()
    : m_holder()
{
}

//-------------------------------------------------------------------------------------------------
UserObject::UserObject(const UserObject& parent, const UserProperty& member)
{
    m_holder.reset(new detail::ObjectHolderByProp(parent, member));
}

//-------------------------------------------------------------------------------------------------
UserObject::UserObject(const UserObject& copy)
    : m_holder(copy.m_holder ? copy.m_holder->clone() : 0)
{
}

//-------------------------------------------------------------------------------------------------
void* UserObject::pointer() const
{
    return m_holder ? m_holder->object() : 0;
}

//-------------------------------------------------------------------------------------------------
const Class& UserObject::getClass() const
{
    if (!m_holder)
        CAMP_ERROR(InvalidObject(*this));

    return m_holder->getClass();
}

//-------------------------------------------------------------------------------------------------
UserObject& UserObject::operator=(const UserObject& other)
{
    m_holder.reset(other.m_holder ? other.m_holder->clone() : 0);

    return *this;
}

//-------------------------------------------------------------------------------------------------
bool UserObject::operator==(const UserObject& other) const
{
    // @todo compare holders (double dispatch?)
    return pointer() == other.pointer();
}

//-------------------------------------------------------------------------------------------------
bool UserObject::operator<(const UserObject& other) const
{
    // @todo compare holders (double dispatch?)
    return pointer() < other.pointer();
}

//-------------------------------------------------------------------------------------------------
void UserObject::set(const Property& property, const Value& value) const
{
    // Make sure we have a valid holder
    if (!m_holder)
        CAMP_ERROR(InvalidObject(*this));

    property.setValue(*this, value);
    m_holder->updateObject();
}

} // namespace camp
