

#include <camp/invalidfunction.hpp>
#include <string.h>


namespace camp
{
//-------------------------------------------------------------------------------------------------
InvalidFunction::InvalidFunction(const char* function, const Class& ownerClass) throw()
    : m_ownerClass(&ownerClass)
{
    strncpy(m_function, function ? function : "", sizeof(m_function));
}

//-------------------------------------------------------------------------------------------------
InvalidFunction::~InvalidFunction() throw()
{
}

//-------------------------------------------------------------------------------------------------
const char* InvalidFunction::what() const throw()
{
    return "The requested function doesn't exist in the metaclass";
}

//-------------------------------------------------------------------------------------------------
const char* InvalidFunction::function() const throw()
{
    return m_function;
}

//-------------------------------------------------------------------------------------------------
const Class& InvalidFunction::ownerClass() const throw()
{
    return *m_ownerClass;
}

} // namespace camp
