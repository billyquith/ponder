

#include <camp/enumbuilder.hpp>
#include <camp/enum.hpp>
#include <cassert>


namespace camp
{
//-------------------------------------------------------------------------------------------------
EnumBuilder::EnumBuilder(Enum& target)
    : m_target(&target)
{
}

//-------------------------------------------------------------------------------------------------
EnumBuilder& EnumBuilder::value(const std::string& name, long value)
{
    assert(!m_target->hasName(name));
    assert(!m_target->hasValue(value));

    Enum::Pair pair;
    pair.name = name;
    pair.value = value;
    m_target->m_pairs.push_back(pair);

    return *this;
}

} // namespace camp
