

#include <camp/invalidproperty.hpp>
#include <string.h>


namespace camp
{
//-------------------------------------------------------------------------------------------------
InvalidProperty::InvalidProperty(const char* property, const Class& ownerClass) throw()
    : m_ownerClass(&ownerClass)
{
    strncpy(m_property, property ? property : "", sizeof(m_property));
}

//-------------------------------------------------------------------------------------------------
InvalidProperty::~InvalidProperty() throw()
{
}

//-------------------------------------------------------------------------------------------------
const char* InvalidProperty::what() const throw()
{
    return "The requested property doesn't exist in the metaclass";
}

//-------------------------------------------------------------------------------------------------
const char* InvalidProperty::property() const throw()
{
    return m_property;
}

//-------------------------------------------------------------------------------------------------
const Class& InvalidProperty::ownerClass() const throw()
{
    return *m_ownerClass;
}

} // namespace camp
