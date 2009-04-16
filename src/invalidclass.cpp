// <insert copyright>

#include <camp/invalidclass.hpp>
#include <string.h>


namespace camp
{
//-------------------------------------------------------------------------------------------------
InvalidClass::InvalidClass(const char* className) throw()
{
    strncpy(m_className, className ? className : "", sizeof(m_className));
}

//-------------------------------------------------------------------------------------------------
InvalidClass::~InvalidClass() throw()
{
}

//-------------------------------------------------------------------------------------------------
const char* InvalidClass::what() const throw()
{
    return "Invalid metaclass request";
}

//-------------------------------------------------------------------------------------------------
const char* InvalidClass::className() const throw()
{
    return m_className;
}

} // namespace camp
