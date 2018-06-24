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

#include <ponder/args.hpp>
#include <ponder/value.hpp>

namespace ponder {
    
const Args Args::empty;

std::size_t Args::count() const
{
    return m_values.size();
}

const Value& Args::operator[](std::size_t index) const
{
    // Make sure that the index is not out of range
    if (index >= m_values.size())
        PONDER_ERROR(OutOfRange(index, m_values.size()));

    return m_values[index];
}

Args Args::operator+(const Value& arg) const
{
    Args newArgs(*this);
    newArgs += arg;

    return newArgs;
}

Args& Args::operator+=(const Value& arg)
{
    m_values.push_back(arg);

    return *this;
}

Args& Args::insert(std::size_t index, const Value& v)
{
    m_values.insert(m_values.begin() + index, v);
    return *this;
}

} // namespace ponder
