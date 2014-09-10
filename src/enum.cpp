/****************************************************************************
**
** This file is part of the CAMP library.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
** 
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
** 
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
**
****************************************************************************/


#include <camp/enum.hpp>
#include <camp/errors.hpp>
#include <camp/detail/enummanager.hpp>
#include <algorithm>


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
    // Make sure that the index is not out of range
    if (index >= m_pairs.size())
        CAMP_ERROR(OutOfRange(index, m_pairs.size()));

    return m_pairs[index];
}

//-------------------------------------------------------------------------------------------------
bool Enum::hasName(const std::string& name) const
{
    const NameIndex& names = m_pairs.get<Name>();

    return names.find(name) != names.end();
}

//-------------------------------------------------------------------------------------------------
bool Enum::hasValue(long value) const
{
    const ValueIndex& values = m_pairs.get<Val>();

    return values.find(value) != values.end();
}

//-------------------------------------------------------------------------------------------------
const std::string& Enum::name(long value) const
{
    const ValueIndex& values = m_pairs.get<Val>();

    ValueIndex::const_iterator it = values.find(value);
    if (it == values.end())
        CAMP_ERROR(EnumValueNotFound(value, name()));

    return it->name;
}

//-------------------------------------------------------------------------------------------------
long Enum::value(const std::string& name) const
{
    const NameIndex& names = m_pairs.get<Name>();

    NameIndex::const_iterator it = names.find(name);
    if (it == names.end())
        CAMP_ERROR(EnumNameNotFound(name, m_name));

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
