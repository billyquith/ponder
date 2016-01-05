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


#ifndef PONDER_DETAIL_SIMPLEPROPERTYIMPL_HPP
#define PONDER_DETAIL_SIMPLEPROPERTYIMPL_HPP


#include <ponder/simpleproperty.hpp>


namespace ponder
{
namespace detail
{
/**
 * \brief Typed implementation of SimpleProperty
 *
 * SimplePropertyImpl is a template implementation of SimpleProperty, which is strongly typed
 * in order to keep track of the true underlying C++ types involved in the property.
 *
 * The template parameter A is an abstract helper to access the actual C++ property.
 *
 * \sa SimpleProperty
 */
template <typename A>
class SimplePropertyImpl : public SimpleProperty
{
public:

    /**
     * \brief Construct the property from its accessors
     *
     * \param name Name of the property
     * \param accessor Object used to access the actual C++ property
     */
    SimplePropertyImpl(const std::string& name, const A& accessor);

protected:

    /**
     * \see Property::isReadable
     */
    virtual bool isReadable() const override;

    /**
     * \see Property::isWritable
     */
    virtual bool isWritable() const override;

    /**
     * \see Property::getValue
     */
    virtual Value getValue(const UserObject& object) const override;

    /**
     * \see Property::setValue
     */
    virtual void setValue(const UserObject& object, const Value& value) const override;

private:

    A m_accessor; ///< Object used to access the actual C++ property
};

} // namespace detail

} // namespace ponder

#include <ponder/detail/simplepropertyimpl.inl>


#endif // PONDER_DETAIL_SIMPLEPROPERTYIMPL_HPP
