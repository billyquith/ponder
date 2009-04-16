// <insert copyright>

#include <camp/invalidargument.hpp>
#include <string.h>


namespace camp
{
//-------------------------------------------------------------------------------------------------
InvalidArgument::InvalidArgument(Type sourceType, Type requestedType, const char* function, std::size_t index) throw()
    : InvalidValue(sourceType, requestedType)
    , m_index(index)
{
    strncpy(m_function, function ? function : "", sizeof(m_function));
}

//-------------------------------------------------------------------------------------------------
InvalidArgument::~InvalidArgument() throw()
{
}

//-------------------------------------------------------------------------------------------------
const char* InvalidArgument::what() const throw()
{
    return "Invalid argument passed to function call; failed to convert to requested type";
}

//-------------------------------------------------------------------------------------------------
const char* InvalidArgument::function() const throw()
{
    return m_function;
}

//-------------------------------------------------------------------------------------------------
std::size_t InvalidArgument::index() const throw()
{
    return m_index;
}

} // namespace camp
