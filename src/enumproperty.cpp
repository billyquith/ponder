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


#include <camp/enumproperty.hpp>
#include <camp/classvisitor.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
EnumProperty::EnumProperty(const std::string& name, const Enum& propEnum)
    : Property(name, enumType)
    , m_enum(&propEnum)
{
}

//-------------------------------------------------------------------------------------------------
EnumProperty::~EnumProperty()
{
}

//-------------------------------------------------------------------------------------------------
const Enum& EnumProperty::getEnum() const
{
    return *m_enum;
}

//-------------------------------------------------------------------------------------------------
void EnumProperty::accept(ClassVisitor& visitor) const
{
    visitor.visit(*this);
}

} // namespace camp
