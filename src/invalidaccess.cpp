// <insert copyright>

#include <camp/invalidaccess.hpp>
#include <string.h>


namespace camp
{
//-------------------------------------------------------------------------------------------------
InvalidAccess::InvalidAccess(const char* attribute, Action action) throw()
    : m_action(action)
{
    strncpy(m_attribute, attribute ? attribute : "", sizeof(m_attribute));
}

//-------------------------------------------------------------------------------------------------
InvalidAccess::InvalidAccess(const InvalidAccess& copy) throw()
    : m_action(copy.m_action)
{
    strncpy(m_attribute, copy.m_attribute, sizeof(m_attribute));
}

//-------------------------------------------------------------------------------------------------
InvalidAccess::~InvalidAccess() throw()
{
}

//-------------------------------------------------------------------------------------------------
const char* InvalidAccess::what() const throw()
{
    return "Invalid access to a property or function";
}

//-------------------------------------------------------------------------------------------------
const char* InvalidAccess::attribute() const throw()
{
    return m_attribute;
}

//-------------------------------------------------------------------------------------------------
InvalidAccess::Action InvalidAccess::action() const throw()
{
    return m_action;
}

} // namespace camp
