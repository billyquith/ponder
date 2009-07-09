/****************************************************************************
**
** Copyright (C) 2009 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** Commercial Usage
** Licensees holding valid CAMP Commercial licenses may use this file in
** accordance with the CAMP Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and TECHNOGERMA Systems France.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3.txt included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at sales@technogerma.fr.
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
