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
