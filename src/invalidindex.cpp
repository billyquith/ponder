// <insert copyright>

#include <camp/invalidindex.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
InvalidIndex::InvalidIndex(std::size_t index, std::size_t size) throw()
    : m_index(index)
    , m_size(size)
{
}

//-------------------------------------------------------------------------------------------------
InvalidIndex::InvalidIndex(const InvalidIndex& copy) throw()
    : m_index(copy.m_index)
    , m_size(copy.m_size)
{
}

//-------------------------------------------------------------------------------------------------
InvalidIndex::~InvalidIndex() throw()
{
}

//-------------------------------------------------------------------------------------------------
const char* InvalidIndex::what() const throw()
{
    return "Index out of range";
}

//-------------------------------------------------------------------------------------------------
std::size_t InvalidIndex::index() const throw()
{
    return m_index;
}

//-------------------------------------------------------------------------------------------------
std::size_t InvalidIndex::size() const throw()
{
    return m_size;
}

} // namespace camp
