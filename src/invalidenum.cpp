// <insert copyright>

#include <camp/invalidenum.hpp>
#include <string.h>


namespace camp
{
//-------------------------------------------------------------------------------------------------
InvalidEnum::InvalidEnum(const char* enumName) throw()
{
    strncpy(m_enumName, enumName ? enumName : "", sizeof(m_enumName));
}

//-------------------------------------------------------------------------------------------------
InvalidEnum::InvalidEnum(const InvalidEnum& copy) throw()
{
    strncpy(m_enumName, copy.m_enumName, sizeof(m_enumName));
}

//-------------------------------------------------------------------------------------------------
InvalidEnum::~InvalidEnum() throw()
{
}

//-------------------------------------------------------------------------------------------------
const char* InvalidEnum::what() const throw()
{
    return "Invalid metaenum request";
}

//-------------------------------------------------------------------------------------------------
const char* InvalidEnum::enumName() const throw()
{
    return m_enumName;
}

} // namespace camp
