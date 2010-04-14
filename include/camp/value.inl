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



namespace camp
{
namespace detail
{
/**
 * \brief Helper structure allowing a shortcut when converting a camp::Value to camp::Value
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

//-------------------------------------------------------------------------------------------------
template <typename T>
Value::Value(const T& val)
    : m_value(camp_ext::ValueMapper<T>::to(val))
    , m_type(mapType<T>())
{
}

//-------------------------------------------------------------------------------------------------
template <typename T>
T Value::to() const
{
    try
    {
        return detail::ValueTo<T>::convert(*this);
    }
    catch (std::exception&)
    {
        CAMP_ERROR(InvalidValue(m_type, mapType<T>()));
    }
}

//-------------------------------------------------------------------------------------------------
template <typename T>
Value::operator T() const
{
    return to<T>();
}

//-------------------------------------------------------------------------------------------------
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
};

//-------------------------------------------------------------------------------------------------
template <typename T>
typename T::result_type Value::visit(T visitor) const
{
    return boost::apply_visitor(visitor, m_value);
}

//-------------------------------------------------------------------------------------------------
template <typename T>
typename T::result_type Value::visit(T visitor, const Value& other) const
{
    return boost::apply_visitor(visitor, m_value, other.m_value);
}

} // namespace camp
