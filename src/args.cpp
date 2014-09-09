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
