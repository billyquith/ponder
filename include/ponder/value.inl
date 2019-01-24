/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2019 Nick Trout.
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


namespace ponder {
namespace detail {


// Is T a user type.
template <typename T> struct IsUserType {
    typedef typename detail::RawType<T>::Type RawType;
    static constexpr bool value = std::is_class<RawType>::value
        && !std::is_same<RawType, Value>::value
        && !std::is_same<RawType, UserObject>::value
        && !std::is_same<RawType, ponder::String>::value;
};

// Decide whether the UserObject holder should be ref (true) or copy (false).
template <typename T> struct IsUserObjRef {
    static constexpr bool value = std::is_pointer<T>::value || std::is_reference<T>::value;
};

/**
 * \brief Helper structure allowing a shortcut when converting a ponder::Value to type
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
    , m_type(ponder_ext::ValueMapper<T>::kind) // mapType<T> NOT used so get same kind as to()
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
        PONDER_ERROR(BadType(kind(), mapType<T>()));
    }
}

template <typename T>
T& Value::ref()
{
    try
    {
        return m_value.get<T>();
    }
    catch (detail::bad_conversion&)
    {
        PONDER_ERROR(BadType(kind(), mapType<T>()));
    }
}

template <typename T>
const T& Value::cref() const
{
    try
    {
        return m_value.get<T>();
    }
    catch (detail::bad_conversion&)
    {
        PONDER_ERROR(BadType(kind(), mapType<T>()));
    }
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
