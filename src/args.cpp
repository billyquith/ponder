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


#include <camp/args.hpp>
#include <camp/value.hpp>
#include <camp/errors.hpp>


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
    // Make sure that the index is not out of range
    if (index >= m_values.size())
        CAMP_ERROR(OutOfRange(index, m_values.size()));

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
