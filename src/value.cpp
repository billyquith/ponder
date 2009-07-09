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



#include <camp/value.hpp>
#include <iostream>


namespace camp
{
const Value Value::nothing;

//-------------------------------------------------------------------------------------------------
Value::Value()
    : m_value(NoType())
    , m_type(noType)
{
}

//-------------------------------------------------------------------------------------------------
Value::Value(const Value& other)
    : m_value(other.m_value)
    , m_type(other.m_type)
{
}

//-------------------------------------------------------------------------------------------------
Type Value::type() const
{
    return m_type;
}

//-------------------------------------------------------------------------------------------------
bool Value::operator==(const Value& other) const
{
    return visit(detail::EqualVisitor(), other);
}

//-------------------------------------------------------------------------------------------------
bool Value::operator<(const Value& other) const
{
    return visit(detail::LessThanVisitor(), other);
}

//-------------------------------------------------------------------------------------------------
CAMP_API std::ostream& operator<<(std::ostream& stream, const Value& value)
{
    value.visit(detail::PrintVisitor(stream));
    return stream;
}

} // namespace camp
