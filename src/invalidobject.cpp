// <insert copyright>

#include <camp/invalidobject.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
InvalidObject::InvalidObject(const UserObject& object) throw()
    : m_object(&object)
{
}

//-------------------------------------------------------------------------------------------------
InvalidObject::~InvalidObject() throw()
{
}

//-------------------------------------------------------------------------------------------------
const char* InvalidObject::what() const throw()
{
    return "Invalid object";
}

//-------------------------------------------------------------------------------------------------
const UserObject& InvalidObject::object() const throw()
{
    return *m_object;
}

} // namespace camp
