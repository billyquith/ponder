/****************************************************************************
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
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


namespace ponder
{
namespace detail
{
/**
 * \brief Helper structure allowing a shortcut when converting a ponder::Value to ponder::Value
 */
template <typename T>
struct ValueTo
{
    static T convert(const Value& value) {return value.visit(ConvertVisitor<T>());}
};
template <>
struct ValueTo<Value>
{
    static Value convert(const Value& value) {return value;}
};

} // namespace detail

template <typename T>
Value::Value(const T& val)
    : m_value(ponder_ext::ValueMapper<T>::to(val))
    , m_type(mapType<T>())
{
}

template <typename T>
T Value::to() const
{
    try
    {
        return detail::ValueTo<T>::convert(*this);
    }
    catch (detail::bad_conversion&)
    {
        PONDER_ERROR(BadType(type(), mapType<T>()));
    }
}

template <typename T>
Value::operator T() const
{
    return to<T>();
}

template <typename T>
bool Value::isCompatible() const
{
    try
    {
        to<T>();
        return true;
    }
    catch (std::exception&)
    {
        return false;
    }
}

template <typename T>
typename T::result_type Value::visit(T visitor) const
{
    return mapbox::util::apply_visitor(visitor, m_value);
}

template <typename T>
typename T::result_type Value::visit(T visitor, const Value& other) const
{
    return mapbox::util::apply_visitor(visitor, m_value, other.m_value);
}

} // namespace ponder
