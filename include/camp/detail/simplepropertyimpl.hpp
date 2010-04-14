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


#ifndef CAMP_DETAIL_SIMPLEPROPERTYIMPL_HPP
#define CAMP_DETAIL_SIMPLEPROPERTYIMPL_HPP


#include <camp/simpleproperty.hpp>
#include <camp/invalidaccess.hpp>


namespace camp
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
    virtual bool isReadable() const;

    /**
     * \see Property::isWritable
     */
    virtual bool isWritable() const;

    /**
     * \see Property::getValue
     */
    virtual Value getValue(const UserObject& object) const;

    /**
     * \see Property::setValue
     */
    virtual void setValue(const UserObject& object, const Value& value) const;

private:

    A m_accessor; ///< Object used to access the actual C++ property
};

} // namespace detail

} // namespace camp

#include <camp/detail/simplepropertyimpl.inl>


#endif // CAMP_DETAIL_SIMPLEPROPERTYIMPL_HPP
