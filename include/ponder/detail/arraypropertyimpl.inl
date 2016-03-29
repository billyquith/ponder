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
template <typename A>
ArrayPropertyImpl<A>::ArrayPropertyImpl(const std::string& name, const A& accessor)
    : ArrayProperty(name, mapType<ElementType>(), Mapper::dynamic())
    , m_accessor(accessor)
{
}

template <typename A>
std::size_t ArrayPropertyImpl<A>::getSize(const UserObject& object) const
{
    return Mapper::size(array(object));
}

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
            insertElement(object, currentSize++, provider());
    }
}

template <typename A>
Value ArrayPropertyImpl<A>::getElement(const UserObject& object, std::size_t index) const
{
    return Mapper::get(array(object), index);
}

template <typename A>
void ArrayPropertyImpl<A>::setElement(const UserObject& object, std::size_t index, const Value& value) const
{
    Mapper::set(array(object), index, value.to<ElementType>());
}

template <typename A>
void ArrayPropertyImpl<A>::insertElement(const UserObject& object, std::size_t before, const Value& value) const
{
    Mapper::insert(array(object), before, value.to<ElementType>());
}

template <typename A>
void ArrayPropertyImpl<A>::removeElement(const UserObject& object, std::size_t index) const
{
    Mapper::remove(array(object), index);
}

template <typename A>
typename ArrayPropertyImpl<A>::ArrayType& ArrayPropertyImpl<A>::array(const UserObject& object) const
{
    return m_accessor.get(object.get<typename A::ClassType>());
}

} // namespace detail

} // namespace ponder
