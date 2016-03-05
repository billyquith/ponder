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


#ifndef PONDER_DETAIL_GETTER_HPP
#define PONDER_DETAIL_GETTER_HPP


#include <ponder/userobject.hpp>
#include <functional>

namespace ponder
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
    GetterImpl(std::function<T (C&)> function);

    /**
     * \see GetterInterface::get
     */
    virtual T get(const UserObject& object) const override;

private:

    std::function<T (C&)> m_function; ///< Function object storing the actual getter
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
    Getter(std::function<T (C&)> function);

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

    std::shared_ptr<GetterInterface<T> > m_getter; ///< Implementation of the getter
    T m_defaultValue; ///< Default value to return if no function or no object is specified
};

} // namespace detail

} // namespace ponder

#include <ponder/detail/getter.inl>


#endif // PONDER_DETAIL_GETTER_HPP
