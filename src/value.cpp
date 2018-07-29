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

#include <ponder/value.hpp>

namespace ponder {
    
const Value Value::nothing;

Value::Value()
    : m_value(NoType())
    , m_type(ValueKind::None)
{
}

Value::Value(const Value& other)
    : m_value(other.m_value)
    , m_type(other.m_type)
{
}

Value::Value(Value&& other)
{
    std::swap(m_value, other.m_value);
    std::swap(m_type, other.m_type);
}

void Value::operator = (const Value& other)
{
    m_value = other.m_value;
    m_type = other.m_type;
}
    
ValueKind Value::kind() const
{
    return m_type;
}

bool Value::operator == (const Value& other) const
{
    return visit(detail::EqualVisitor(), other);
}

bool Value::operator < (const Value& other) const
{
    return visit(detail::LessThanVisitor(), other);
}

std::istream& operator >> (std::istream& stream, Value& value)
{
    // Use the string conversion
//    ponder::Id str;
//    stream >> str;
//    value = str;
    return stream;
}

std::ostream& operator << (std::ostream& stream, const Value& value)
{
    // Use the string conversion
    return stream; // << value.to<ponder::Id>();
}

} // namespace ponder
