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


#include <camp/invalidenum.hpp>
#include <string.h>


namespace camp
{
//-------------------------------------------------------------------------------------------------
InvalidEnum::InvalidEnum(const char* enumName) throw()
{
    strncpy(m_enumName, enumName ? enumName : "", sizeof(m_enumName));
}

//-------------------------------------------------------------------------------------------------
InvalidEnum::~InvalidEnum() throw()
{
}

//-------------------------------------------------------------------------------------------------
const char* InvalidEnum::what() const throw()
{
    return "Invalid metaenum request";
}

//-------------------------------------------------------------------------------------------------
const char* InvalidEnum::enumName() const throw()
{
    return m_enumName;
}

} // namespace camp
