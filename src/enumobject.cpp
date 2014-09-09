/****************************************************************************
**
** This file is part of the CAMP library.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
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


#include <camp/enumobject.hpp>
#include <camp/enum.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
long EnumObject::value() const
{
    return m_value;
}

//-------------------------------------------------------------------------------------------------
const std::string& EnumObject::name() const
{
    return m_enum->name(m_value);
}

//-------------------------------------------------------------------------------------------------
const Enum& EnumObject::getEnum() const
{
    return *m_enum;
}

//-------------------------------------------------------------------------------------------------
bool EnumObject::operator==(const EnumObject& other) const
{
    return (m_enum == other.m_enum) && (m_value == other.m_value);
}

//-------------------------------------------------------------------------------------------------
bool EnumObject::operator<(const EnumObject& other) const
{
    if (m_enum != other.m_enum)
    {
        return m_enum < other.m_enum;
    }
    else
    {
        return m_value < other.m_value;
    }
}

} // namespace camp
