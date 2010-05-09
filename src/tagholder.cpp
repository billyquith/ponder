/****************************************************************************
**
** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** CAMP is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** 
** CAMP is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public License
** along with CAMP.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/


#include <camp/tagholder.hpp>
#include <camp/value.hpp>
#include <camp/outofrange.hpp>
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
    // Make sure that the index is not out of range
    if (index >= m_tags.size())
        CAMP_ERROR(OutOfRange(index, m_tags.size()));

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
