

#include <camp/enumobject.hpp>
#include <camp/enum.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
long EnumObject::value() const
{
    return m_value;
}

//-------------------------------------------------------------------------------------------------
const std::string& EnumObject::name() const
{
    return m_enum->name(m_value);
}

//-------------------------------------------------------------------------------------------------
const Enum& EnumObject::getEnum() const
{
    return *m_enum;
}

//-------------------------------------------------------------------------------------------------
bool EnumObject::operator==(const EnumObject& other) const
{
    return (m_enum == other.m_enum) && (m_value == other.m_value);
}

//-------------------------------------------------------------------------------------------------
bool EnumObject::operator<(const EnumObject& other) const
{
    if (m_enum != other.m_enum)
    {
        return m_enum < other.m_enum;
    }
    else
    {
        return m_value < other.m_value;
    }
}

} // namespace camp
