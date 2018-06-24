/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2018 Nick Trout.
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

#include <ponder/enum.hpp>
#include <ponder/errors.hpp>

namespace ponder {

Enum::Enum(IdRef name)
    :   m_name(name)
{
}

IdReturn Enum::name() const
{
    return m_name;
}

std::size_t Enum::size() const
{
    return m_enums.size();
}

Enum::Pair Enum::pair(std::size_t index) const
{
    // Make sure that the index is not out of range
    if (index >= m_enums.size())
        PONDER_ERROR(OutOfRange(index, m_enums.size()));

    auto it = m_enums.at(index);
    return Pair(it->first, it->second);
}

bool Enum::hasName(IdRef name) const
{
    return m_enums.containsKey(name);
}

bool Enum::hasValue(EnumValue value) const
{
    return m_enums.containsValue(value);
}

IdReturn Enum::name(EnumValue value) const
{
    auto it = m_enums.findValue(value);
    
    if (it == m_enums.end())
        PONDER_ERROR(EnumValueNotFound(value, name()));

    return it->first;
}

Enum::EnumValue Enum::value(IdRef name) const
{
    auto it = m_enums.findKey(name);

    if (it == m_enums.end())
        PONDER_ERROR(EnumNameNotFound(name, m_name));

    return it->second;
}

bool Enum::operator == (const Enum& other) const
{
    return name() == other.name();
}

bool Enum::operator != (const Enum& other) const
{
    return name() != other.name();
}

} // namespace ponder
