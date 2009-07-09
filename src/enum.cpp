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


#include <camp/enum.hpp>
#include <camp/invalidindex.hpp>
#include <camp/invalidenumname.hpp>
#include <camp/invalidenumvalue.hpp>
#include <algorithm>


namespace
{
    /**
     * \brief Function object which finds an enum pair by its name
     */
    struct FindByName
    {
        FindByName(const std::string& name) : m_name(name) {}

        bool operator()(const camp::Enum::Pair& p) const
        {
            return p.name == m_name;
        }

        std::string m_name;
    };

    /**
     * \brief Function object which finds an enum pair by its value
     */
    struct FindByValue
    {
        FindByValue(long value) : m_value(value) {}

        bool operator()(const camp::Enum::Pair& p) const
        {
            return p.value == m_value;
        }

        long m_value;
    };
}

namespace camp
{
//-------------------------------------------------------------------------------------------------
const std::string& Enum::name() const
{
    return m_name;
}

//-------------------------------------------------------------------------------------------------
std::size_t Enum::size() const
{
    return m_pairs.size();
}

//-------------------------------------------------------------------------------------------------
const Enum::Pair& Enum::pair(std::size_t index) const
{
    if (index >= m_pairs.size())
        CAMP_ERROR(InvalidIndex(index, m_pairs.size()));

    return m_pairs[index];
}

//-------------------------------------------------------------------------------------------------
bool Enum::hasName(const std::string& name) const
{
    return std::find_if(m_pairs.begin(), m_pairs.end(), FindByName(name)) != m_pairs.end();
}

//-------------------------------------------------------------------------------------------------
bool Enum::hasValue(long value) const
{
    return std::find_if(m_pairs.begin(), m_pairs.end(), FindByValue(value)) != m_pairs.end();
}

//-------------------------------------------------------------------------------------------------
const std::string& Enum::name(long value) const
{
    PairArray::const_iterator it = std::find_if(m_pairs.begin(), m_pairs.end(), FindByValue(value));

    if (it == m_pairs.end())
        CAMP_ERROR(InvalidEnumValue(value, *this));

    return it->name;
}

//-------------------------------------------------------------------------------------------------
long Enum::value(const std::string& name) const
{
    PairArray::const_iterator it = std::find_if(m_pairs.begin(), m_pairs.end(), FindByName(name));

    if (it == m_pairs.end())
        CAMP_ERROR(InvalidEnumName(name.c_str(), *this));

    return it->value;
}

//-------------------------------------------------------------------------------------------------
bool Enum::operator==(const Enum& other) const
{
    return m_name == other.m_name;
}

//-------------------------------------------------------------------------------------------------
bool Enum::operator!=(const Enum& other) const
{
    return m_name != other.m_name;
}

//-------------------------------------------------------------------------------------------------
Enum::Enum(const std::string& name)
    : m_name(name)
{
}

} // namespace camp
