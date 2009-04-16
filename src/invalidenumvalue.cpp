// <insert copyright>

#include <camp/invalidenumvalue.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
InvalidEnumValue::InvalidEnumValue(long value, const Enum& ownerEnum) throw()
    : m_value(value)
    , m_ownerEnum(&ownerEnum)
{
}

//-------------------------------------------------------------------------------------------------
InvalidEnumValue::~InvalidEnumValue() throw()
{
}

//-------------------------------------------------------------------------------------------------
const char* InvalidEnumValue::what() const throw()
{
    return "The requested value doesn't exist in the metaenum";
}

//-------------------------------------------------------------------------------------------------
long InvalidEnumValue::value() const throw()
{
    return m_value;
}

//-------------------------------------------------------------------------------------------------
const Enum& InvalidEnumValue::ownerEnum() const throw()
{
    return *m_ownerEnum;
}

} // namespace camp
