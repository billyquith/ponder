

#include <camp/tagholder.hpp>
#include <camp/value.hpp>
#include <camp/invalidindex.hpp>
#include <algorithm>


namespace camp
{
//-------------------------------------------------------------------------------------------------
TagHolder::~TagHolder()
{
}

//-------------------------------------------------------------------------------------------------
std::size_t TagHolder::tagCount() const
{
    return m_tags.size();
}

//-------------------------------------------------------------------------------------------------
const Value& TagHolder::tagId(std::size_t index) const
{
    if (index >= m_tags.size())
        CAMP_ERROR(InvalidIndex(index, m_tags.size()));

    TagsTable::const_iterator it = m_tags.begin();
    std::advance(it, index);

    return it->first;
}

//-------------------------------------------------------------------------------------------------
bool TagHolder::hasTag(const Value& id) const
{
    return m_tags.find(id) != m_tags.end();
}

//-------------------------------------------------------------------------------------------------
const Value& TagHolder::tag(const Value& id) const
{
    TagsTable::const_iterator it = m_tags.find(id);
    if (it != m_tags.end())
        return it->second.get();

    return Value::nothing;
}

//-------------------------------------------------------------------------------------------------
Value TagHolder::tag(const Value& id, const UserObject& object) const
{
    TagsTable::const_iterator it = m_tags.find(id);
    if (it != m_tags.end())
        return it->second.get(object);

    return Value::nothing;
}

//-------------------------------------------------------------------------------------------------
TagHolder::TagHolder()
{
}

} // namespace camp
