

#include <camp/args.hpp>
#include <camp/value.hpp>
#include <camp/invalidindex.hpp>


namespace camp
{
const Args Args::empty;

//-------------------------------------------------------------------------------------------------
Args::Args()
{
}

//-------------------------------------------------------------------------------------------------
Args::Args(const Value& a0)
{
    m_values.push_back(a0);
}

//-------------------------------------------------------------------------------------------------
Args::Args(const Value& a0, const Value& a1)
{
    m_values.push_back(a0);
    m_values.push_back(a1);
}

//-------------------------------------------------------------------------------------------------
Args::Args(const Value& a0, const Value& a1, const Value& a2)
{
    m_values.push_back(a0);
    m_values.push_back(a1);
    m_values.push_back(a2);
}

//-------------------------------------------------------------------------------------------------
Args::Args(const Value& a0, const Value& a1, const Value& a2, const Value& a3)
{
    m_values.push_back(a0);
    m_values.push_back(a1);
    m_values.push_back(a2);
    m_values.push_back(a3);
}

//-------------------------------------------------------------------------------------------------
Args::Args(const Value& a0, const Value& a1, const Value& a2, const Value& a3, const Value& a4)
{
    m_values.push_back(a0);
    m_values.push_back(a1);
    m_values.push_back(a2);
    m_values.push_back(a3);
    m_values.push_back(a4);
}

//-------------------------------------------------------------------------------------------------
std::size_t Args::count() const
{
    return m_values.size();
}

//-------------------------------------------------------------------------------------------------
const Value& Args::operator[](std::size_t index) const
{
    if (index >= m_values.size())
        CAMP_ERROR(InvalidIndex(index, m_values.size()));

    return m_values[index];
}

//-------------------------------------------------------------------------------------------------
Args Args::operator+(const Value& arg) const
{
    Args newArgs(*this);
    newArgs += arg;

    return newArgs;
}

//-------------------------------------------------------------------------------------------------
Args& Args::operator+=(const Value& arg)
{
    m_values.push_back(arg);

    return *this;
}

} // namespace camp
