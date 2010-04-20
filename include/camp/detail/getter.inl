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
//-------------------------------------------------------------------------------------------------
template <typename T>
GetterInterface<T>::~GetterInterface()
{
}

//-------------------------------------------------------------------------------------------------
template <typename T, typename C>
GetterImpl<T, C>::GetterImpl(boost::function<T (C&)> function)
    : m_function(function)
{
}

//-------------------------------------------------------------------------------------------------
template <typename T, typename C>
T GetterImpl<T, C>::get(const UserObject& object) const
{
    return m_function(object.get<C>());
}

//-------------------------------------------------------------------------------------------------
template <typename T>
Getter<T>::Getter(const T& defaultValue)
    : m_getter()
    , m_defaultValue(defaultValue)
{
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename C>
Getter<T>::Getter(boost::function<T (C&)> function)
    : m_getter(new GetterImpl<T, C>(function))
{
}

//-------------------------------------------------------------------------------------------------
template <typename T>
const T& Getter<T>::get() const
{
    return m_defaultValue;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
T Getter<T>::get(const UserObject& object) const
{
    return m_getter ? m_getter->get(object) : m_defaultValue;
}

} // namespace detail

} // namespace camp
