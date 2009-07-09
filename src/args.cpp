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
