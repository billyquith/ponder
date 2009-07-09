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
** contact the sales department at sales@technogerma.fr.
**
****************************************************************************/


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
