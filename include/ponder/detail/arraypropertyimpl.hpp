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

#pragma once
#ifndef PONDER_DETAIL_ARRAYPROPERTYIMPL_HPP
#define PONDER_DETAIL_ARRAYPROPERTYIMPL_HPP

#include <ponder/arrayproperty.hpp>
#include <ponder/arraymapper.hpp>
#include <ponder/detail/valueprovider.hpp>

namespace ponder {
namespace detail {
    
/**
 * \brief Typed implementation of ArrayProperty
 *
 * ArrayPropertyImpl is a template implementation of ArrayProperty, which is strongly typed
 * in order to keep track of the true underlying C++ types involved in the property.
 *
 * The template parameter A is an abstract helper to access the actual C++ property.
 *
 * This class uses the ponder_ext::ArrayMapper template to implement its operations according
 * to the type of array.
 *
 * \sa ArrayProperty, ponder_ext::ArrayMapper
 */
template <typename A>
class ArrayPropertyImpl final : public ArrayProperty
{
public:

    /**
     * \brief Construct the property
     *
     * \param name Name of the property
     * \param accessor Object used to access the actual C++ property
     */
    ArrayPropertyImpl(IdRef name, A&& accessor);

protected:

    /**
     * \see ArrayProperty::getSize
     */
    std::size_t getSize(const UserObject& object) const final;

    /**
     * \see ArrayProperty::setSize
     */
    void setSize(const UserObject& object, std::size_t size) const final;

    /**
     * \see ArrayProperty::getElement
     */
    Value getElement(const UserObject& object, std::size_t index) const final;

    /**
     * \see ArrayProperty::setElement
     */
    void setElement(const UserObject& object, std::size_t index, const Value& value) const final;

    /**
     * \see ArrayProperty::insertElement
     */
    void insertElement(const UserObject& object, std::size_t before, const Value& value) const final;

    /**
     * \see ArrayProperty::removeElement
     */
    void removeElement(const UserObject& object, std::size_t index) const final;

private:

    typedef typename A::ExposedType ArrayType;
    typedef typename A::InterfaceType Mapper;
    typedef typename Mapper::ElementType ElementType;

    /*
     * \brief Retrieve a reference to the array
     * \param object Owner object
     * \return Reference to the underlying array
     */
    ArrayType& array(const UserObject& object) const;

    A m_accessor; // Object used to access the actual C++ property
};

} // namespace detail
} // namespace ponder

#include <ponder/detail/arraypropertyimpl.inl>

#endif // PONDER_DETAIL_ARRAYPROPERTYIMPL_HPP
