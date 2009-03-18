// <insert copyright>


#include <camp/value.hpp>
#include <iostream>


namespace camp
{
const Value Value::nothing;

//-------------------------------------------------------------------------------------------------
Value::Value()
    : m_value(NoType())
    , m_type(noType)
{
}

//-------------------------------------------------------------------------------------------------
Value::Value(const Value& other)
    : m_value(other.m_value)
    , m_type(other.m_type)
{
}

//-------------------------------------------------------------------------------------------------
Type Value::type() const
{
    return m_type;
}

//-------------------------------------------------------------------------------------------------
bool Value::operator==(const Value& other) const
{
    return visit(detail::EqualVisitor(), other);
}

//-------------------------------------------------------------------------------------------------
bool Value::operator<(const Value& other) const
{
    return visit(detail::LessThanVisitor(), other);
}

//-------------------------------------------------------------------------------------------------
CAMP_API std::ostream& operator<<(std::ostream& stream, const Value& value)
{
    value.visit(detail::PrintVisitor(stream));
    return stream;
}

} // namespace camp
