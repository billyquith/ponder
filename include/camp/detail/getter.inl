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
