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


#ifndef CAMP_DETAIL_GETTER_HPP
#define CAMP_DETAIL_GETTER_HPP


#include <camp/userobject.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>


namespace camp
{
namespace detail
{
/**
 * \brief Represents an abstract getter
 *
 * This class is an interface which must be inherited by typed specializations.
 * The template parameter T is the type returned by the getter.
 *
 * \sa Getter, GetterImpl
 */
template <typename T>
class GetterInterface
{
public:

    /**
     * \brief Destructor
     */
    virtual ~GetterInterface();

    /**
     * \brief Retrieve the value returned by the getter
     *
     * \param object User object to apply the getter to
     */
    virtual T get(const UserObject& object) const = 0;
};

/**
 * \brief Typed implementation of GetterInterface
 */
template <typename T, typename C>
class GetterImpl : public GetterInterface<T>
{
public:

    /**
     * \brief Construct the getter implementation from a function
     */
    GetterImpl(boost::function<T (C&)> function);

    /**
     * \see GetterInterface::get
     */
    virtual T get(const UserObject& object) const;

private:

    boost::function<T (C&)> m_function; ///< Function object storing the actual getter
};

/**
 * \brief Generic getter which can be applied to user objects
 *
 * This class models a Getter as a non-member function; this way it can be
 * used without knowledge about the actual owner class, and be applied
 * uniformly to user objects.
 *
 * Getter can be built upon any kind of callable type or a constant value.
 * The template parameter T is the type returned by the getter.
 *
 * \sa UserObject, GetterInterface
 */
template <typename T>
class Getter
{
public:

    /**
     * \brief Construct the getter from a constant value
     *
     * This value will be returned either if no object is passed, or if no getter function has been defined
     *
     * \param defaultValue Value to return if no function has been set (default value of T by default)
     */
    Getter(const T& defaultValue = T());

    /**
     * \brief Construct the getter from a function
     *
     * \param function Function object storing the actual getter
     */
    template <typename C>
    Getter(boost::function<T (C&)> function);

    /**
     * \brief Get the default value of the getter
     *
     * \return Constant stored in the getter
     */
    const T& get() const;

    /**
     * \brief Retrieve the value returned by the getter
     *
     * If no function has been set, it's equivalent to calling Getter::get().
     *
     * \param object User object to apply the getter to
     *
     * \return Value returned by the getter
     */
    T get(const UserObject& object) const;

private:

    boost::shared_ptr<GetterInterface<T> > m_getter; ///< Implementation of the getter
    T m_defaultValue; ///< Default value to return if no function or no object is specified
};

} // namespace detail

} // namespace camp

#include <camp/detail/getter.inl>


#endif // CAMP_DETAIL_GETTER_HPP
