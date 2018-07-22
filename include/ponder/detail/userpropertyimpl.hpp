/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2018 Nick Trout.
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
#ifndef PONDER_DETAIL_USERPROPERTYIMPL_HPP
#define PONDER_DETAIL_USERPROPERTYIMPL_HPP

#include <ponder/userproperty.hpp>

namespace ponder {
namespace detail {
    
/**
 * \brief Typed implementation of UserProperty
 *
 * UserPropertyImpl is a template implementation of UserProperty, which is strongly typed
 * in order to keep track of the true underlying C++ types involved in the property.
 *
 * The template parameter A is an abstract helper to access the actual C++ property.
 *
 * \sa UserProperty
 */
template <typename A>
class UserPropertyImpl : public UserProperty
{
public:

    /**
     * \brief Construct the property from its accessors
     *
     * \param name Name of the property
     * \param accessor Object used to access the actual C++ property
     */
    UserPropertyImpl(IdRef name, A&& accessor);

protected:

    /**
     * \see Property::isReadable
     */
    bool isReadable() const final;

    /**
     * \see Property::isWritable
     */
    bool isWritable() const final;

    /**
     * \see Property::getValue
     */
    Value getValue(const UserObject& object) const final;

    /**
     * \see Property::setValue
     */
    void setValue(const UserObject& object, const Value& value) const final;

    /**
    * \see UserProperty::getObject
    */
    UserObject getObject(const UserObject& parentInstance) const final;

private:

    A m_accessor; // Object used to access the actual C++ property
};

} // namespace detail
} // namespace ponder

#include <ponder/detail/userpropertyimpl.inl>


#endif // PONDER_DETAIL_USERPROPERTYIMPL_HPP
