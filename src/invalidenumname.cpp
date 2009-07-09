

#include <camp/invalidenumname.hpp>
#include <string.h>


namespace camp
{
//-------------------------------------------------------------------------------------------------
InvalidEnumName::InvalidEnumName(const char* name, const Enum& ownerEnum) throw()
    : m_ownerEnum(&ownerEnum)
{
    strncpy(m_name, name ? name : "", sizeof(m_name));
}

//-------------------------------------------------------------------------------------------------
InvalidEnumName::~InvalidEnumName() throw()
{
}

//-------------------------------------------------------------------------------------------------
const char* InvalidEnumName::what() const throw()
{
    return "The requested name doesn't exist in the metaenum";
}

//-------------------------------------------------------------------------------------------------
const char* InvalidEnumName::name() const throw()
{
    return m_name;
}

//-------------------------------------------------------------------------------------------------
const Enum& InvalidEnumName::ownerEnum() const throw()
{
    return *m_ownerEnum;
}

} // namespace camp
