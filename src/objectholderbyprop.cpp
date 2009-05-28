// <insert copyright>

#include <camp/detail/objectholderbyprop.hpp>
#include <camp/userproperty.hpp>


namespace camp
{
namespace detail
{
//-------------------------------------------------------------------------------------------------
ObjectHolderByProp::ObjectHolderByProp(const UserObject& parent, const UserProperty& member)
    : m_parent(parent)
    , m_member(&member)
{
}

//-------------------------------------------------------------------------------------------------
AbstractObjectHolder* ObjectHolderByProp::clone() const
{
    return new ObjectHolderByProp(*this);
}

//-------------------------------------------------------------------------------------------------
void* ObjectHolderByProp::object()
{
    m_object = m_member->get(m_parent).to<UserObject>();
    return m_object.pointer();
}

//-------------------------------------------------------------------------------------------------
void ObjectHolderByProp::updateObject()
{
    m_member->set(m_parent, m_object);
}

//-------------------------------------------------------------------------------------------------
const Class& ObjectHolderByProp::getClass() const
{
    return m_member->getClass();
}

} // namespace detail

} // namespace camp
