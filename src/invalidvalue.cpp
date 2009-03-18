// <insert copyright>

#include <camp/invalidvalue.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
InvalidValue::InvalidValue(Type sourceType, Type requestedType) throw()
    : m_sourceType(sourceType)
    , m_requestedType(requestedType)
{
}

//-------------------------------------------------------------------------------------------------
InvalidValue::~InvalidValue() throw()
{
}

//-------------------------------------------------------------------------------------------------
const char* InvalidValue::what() const throw()
{
    return "Invalid value; failed to convert to the requested type";
}

//-------------------------------------------------------------------------------------------------
Type InvalidValue::sourceType() const throw()
{
    return m_sourceType;
}

//-------------------------------------------------------------------------------------------------
Type InvalidValue::requestedType() const throw()
{
    return m_requestedType;
}

} // namespace camp
