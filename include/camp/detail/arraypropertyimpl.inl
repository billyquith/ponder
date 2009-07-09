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
template <typename A>
ArrayPropertyImpl<A>::ArrayPropertyImpl(const std::string& name, const A& accessor)
    : ArrayProperty(name, mapType<ElementType>(), Mapper::dynamic())
    , m_accessor(accessor)
{
}

//-------------------------------------------------------------------------------------------------
template <typename A>
std::size_t ArrayPropertyImpl<A>::getSize(const UserObject& object) const
{
    return Mapper::size(array(object));
}

//-------------------------------------------------------------------------------------------------
template <typename A>
Value ArrayPropertyImpl<A>::getElement(const UserObject& object, std::size_t index) const
{
    return Mapper::get(array(object), index);
}

//-------------------------------------------------------------------------------------------------
template <typename A>
void ArrayPropertyImpl<A>::setElement(const UserObject& object, std::size_t index, const Value& value) const
{
    Mapper::set(array(object), index, value.to<ElementType>());
}

//-------------------------------------------------------------------------------------------------
template <typename A>
void ArrayPropertyImpl<A>::insertElement(const UserObject& object, std::size_t before, const Value& value) const
{
    Mapper::insert(array(object), before, value.to<ElementType>());
}

//-------------------------------------------------------------------------------------------------
template <typename A>
void ArrayPropertyImpl<A>::removeElement(const UserObject& object, std::size_t index) const
{
    Mapper::remove(array(object), index);
}

//-------------------------------------------------------------------------------------------------
template <typename A>
typename ArrayPropertyImpl<A>::ArrayType& ArrayPropertyImpl<A>::array(const UserObject& object) const
{
    return m_accessor.get(object.get<typename A::ClassType>());
}

} // namespace detail

} // namespace camp
