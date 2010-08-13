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
void ArrayPropertyImpl<A>::setSize(const UserObject& object, std::size_t size) const
{
    std::size_t currentSize = getSize(object);
    if (size < currentSize)
    {
        while (size < currentSize)
            removeElement(object, --currentSize);
    }
    else if (size > currentSize)
    {
        ValueProvider<ElementType> provider;
        while (size > currentSize)
            insertElement(object, currentSize++, provider.value());
    }
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
