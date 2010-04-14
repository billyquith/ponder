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
