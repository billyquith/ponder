/****************************************************************************
**
** Copyright (C) 2009 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** Commercial Usage
** Licensees holding valid CAMP Commercial licenses may use this file in
** accordance with the CAMP Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and TECHNOGERMA Systems France.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3.txt included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at contact@technogerma.fr.
**
****************************************************************************/


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
