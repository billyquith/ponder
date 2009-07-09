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


#include <camp/property.hpp>
#include <camp/invalidaccess.hpp>
#include <camp/classvisitor.hpp>
#include <camp/userobject.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
Property::~Property()
{
}

//-------------------------------------------------------------------------------------------------
const std::string& Property::name() const
{
    return m_name;
}

//-------------------------------------------------------------------------------------------------
Type Property::type() const
{
    return m_type;
}

//-------------------------------------------------------------------------------------------------
bool Property::readable(const UserObject& object) const
{
    return isReadable() && m_readable.get(object);
}

//-------------------------------------------------------------------------------------------------
bool Property::writable(const UserObject& object) const
{
    return isWritable() && m_writable.get(object);
}

//-------------------------------------------------------------------------------------------------
Value Property::get(const UserObject& object) const
{
    // Check if the property is readable
    if (!readable(object))
        CAMP_ERROR(InvalidAccess(m_name.c_str(), InvalidAccess::Read));

    return getValue(object);
}

//-------------------------------------------------------------------------------------------------
void Property::set(const UserObject& object, const Value& value) const
{
    // Check if the property is writable
    if (!writable(object))
        CAMP_ERROR(InvalidAccess(m_name.c_str(), InvalidAccess::Write));

    // Here we don't call setValue directly, we rather let the user object do it
    // and add any processing needed for proper propagation of the modification
    object.set(*this, value);
}

//-------------------------------------------------------------------------------------------------
void Property::accept(ClassVisitor& visitor) const
{
    visitor.visit(*this);
}

//-------------------------------------------------------------------------------------------------
bool Property::isReadable() const
{
    return true;
}

//-------------------------------------------------------------------------------------------------
bool Property::isWritable() const
{
    return true;
}

//-------------------------------------------------------------------------------------------------
Property::Property(const std::string& name, Type type)
    : m_name(name)
    , m_type(type)
    , m_readable(true)
    , m_writable(true)
{
}

} // namespace camp
