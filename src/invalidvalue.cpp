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


#include <camp/invalidvalue.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
InvalidValue::InvalidValue(Type sourceType, Type requestedType) throw()
    : m_sourceType(sourceType)
    , m_requestedType(requestedType)
{
}

//-------------------------------------------------------------------------------------------------
InvalidValue::~InvalidValue() throw()
{
}

//-------------------------------------------------------------------------------------------------
const char* InvalidValue::what() const throw()
{
    return "Invalid value; failed to convert to the requested type";
}

//-------------------------------------------------------------------------------------------------
Type InvalidValue::sourceType() const throw()
{
    return m_sourceType;
}

//-------------------------------------------------------------------------------------------------
Type InvalidValue::requestedType() const throw()
{
    return m_requestedType;
}

} // namespace camp
