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


#include <camp/enumobject.hpp>
#include <camp/enum.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
long EnumObject::value() const
{
    return m_value;
}

//-------------------------------------------------------------------------------------------------
const std::string& EnumObject::name() const
{
    return m_enum->name(m_value);
}

//-------------------------------------------------------------------------------------------------
const Enum& EnumObject::getEnum() const
{
    return *m_enum;
}

//-------------------------------------------------------------------------------------------------
bool EnumObject::operator==(const EnumObject& other) const
{
    return (m_enum == other.m_enum) && (m_value == other.m_value);
}

//-------------------------------------------------------------------------------------------------
bool EnumObject::operator<(const EnumObject& other) const
{
    if (m_enum != other.m_enum)
    {
        return m_enum < other.m_enum;
    }
    else
    {
        return m_value < other.m_value;
    }
}

} // namespace camp
