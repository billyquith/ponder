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


#ifndef CAMP_DETAIL_ARRAYPROPERTYIMPL_HPP
#define CAMP_DETAIL_ARRAYPROPERTYIMPL_HPP


#include <camp/arrayproperty.hpp>
#include <camp/arraymapper.hpp>
#include <boost/type_traits/remove_reference.hpp>


namespace camp
{
namespace detail
{
/**
 * \brief Typed implementation of ArrayProperty
 *
 * ArrayPropertyImpl is a template implementation of ArrayProperty, which is strongly typed
 * in order to keep track of the true underlying C++ types involved in the property.
 *
 * The template parameter A is an abstract helper to access the actual C++ property.
 *
 * This class uses the camp_ext::ArrayMapper template to implement its operations according
 * to the type of array.
 *
 * \sa ArrayProperty, camp_ext::ArrayMapper
 */
template <typename A>
class ArrayPropertyImpl : public ArrayProperty
{
public:

    /**
     * \brief Construct the property
     *
     * \param name Name of the property
     * \param accessor Object used to access the actual C++ property
     */
    ArrayPropertyImpl(const std::string& name, const A& accessor);

protected:

    /**
     * \see ArrayProperty::getSize
     */
    virtual std::size_t getSize(const UserObject& object) const;

    /**
     * \see ArrayProperty::getElement
     */
    virtual Value getElement(const UserObject& object, std::size_t index) const;

    /**
     * \see ArrayProperty::setElement
     */
    virtual void setElement(const UserObject& object, std::size_t index, const Value& value) const;

    /**
     * \see ArrayProperty::insertElement
     */
    virtual void insertElement(const UserObject& object, std::size_t before) const;

    /**
     * \see ArrayProperty::insertElement
     */
    virtual void insertElement(const UserObject& object, std::size_t before, const Value& value) const;

    /**
     * \see ArrayProperty::removeElement
     */
    virtual void removeElement(const UserObject& object, std::size_t index) const;

private:

    typedef typename boost::remove_reference<typename A::DataType>::type ArrayType;
    typedef camp_ext::ArrayMapper<ArrayType> Mapper;
    typedef typename Mapper::ElementType ElementType;

    /**
     * \brief Retrieve a reference to the array
     *
     * \param object Owner object
     *
     * \return Reference to the underlying array
     */
    ArrayType& array(const UserObject& object) const;

    A m_accessor; ///< Object used to access the actual C++ property
};

} // namespace detail

} // namespace camp

#include <camp/detail/arraypropertyimpl.inl>


#endif // CAMP_DETAIL_ARRAYPROPERTYIMPL_HPP
