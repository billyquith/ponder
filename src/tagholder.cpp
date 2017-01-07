/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2017 Nick Trout.
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
** 
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
** 
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
**
****************************************************************************/


#include <ponder/tagholder.hpp>


namespace ponder
{
    
TagHolder::~TagHolder()
{
}

std::size_t TagHolder::tagCount() const
{
    return m_tags.size();
}

const Value& TagHolder::tagId(std::size_t index) const
{
    // Make sure that the index is not out of range
    if (index >= m_tags.size())
        PONDER_ERROR(OutOfRange(index, m_tags.size()));

    TagsTable::const_iterator it = m_tags.begin();
    std::advance(it, index);

    return it->first;
}

bool TagHolder::hasTag(const Value& id) const
{
    return m_tags.find(id) != m_tags.end();
}

const Value& TagHolder::tag(const Value& id) const
{
    TagsTable::const_iterator it = m_tags.find(id);
    if (it != m_tags.end())
        return it->second.get();

    return Value::nothing;
}

Value TagHolder::tag(const Value& id, const UserObject& object) const
{
    TagsTable::const_iterator it = m_tags.find(id);
    if (it != m_tags.end())
        return it->second.get(object);

    return Value::nothing;
}

TagHolder::TagHolder()
{
}

} // namespace ponder
