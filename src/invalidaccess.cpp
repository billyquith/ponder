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


#include <camp/invalidaccess.hpp>
#include <string.h>


namespace camp
{
//-------------------------------------------------------------------------------------------------
InvalidAccess::InvalidAccess(const char* attribute, Action action) throw()
    : m_action(action)
{
    strncpy(m_attribute, attribute ? attribute : "", sizeof(m_attribute));
}

//-------------------------------------------------------------------------------------------------
InvalidAccess::~InvalidAccess() throw()
{
}

//-------------------------------------------------------------------------------------------------
const char* InvalidAccess::what() const throw()
{
    return "Invalid access to a property or function";
}

//-------------------------------------------------------------------------------------------------
const char* InvalidAccess::attribute() const throw()
{
    return m_attribute;
}

//-------------------------------------------------------------------------------------------------
InvalidAccess::Action InvalidAccess::action() const throw()
{
    return m_action;
}

} // namespace camp
