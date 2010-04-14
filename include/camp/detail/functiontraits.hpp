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


#ifndef CAMP_DETAIL_FUNCTIONTRAITS_HPP
#define CAMP_DETAIL_FUNCTIONTRAITS_HPP


#include <camp/detail/yesnotype.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/function_types/is_callable_builtin.hpp>
#include <boost/function_types/result_type.hpp>


namespace camp
{
namespace detail
{
/**
 * \brief Helper structure to check at compile time if a type is a functor
 *
 * This structure checks if the provided type defines a result_type type.
 */
template <typename T>
struct HasResultType
{
    template <typename U, typename V> struct TestForMember {};
    template <typename U> static TypeYes check(TestForMember<U, typename U::result_type>*);
    template <typename U> static TypeNo  check(...);

    enum {value = sizeof(check<T>(0)) == sizeof(TypeYes)};
};

/**
 * \class FunctionTraits
 *
 * \brief Utility class which gives compile-time informations about function types
 *
 * The FunctionTraits provides two informations about a type T:
 * \li A compile-time constant \c isFunction which tells if T is any callable type
 * \li A type \c ReturnType which is the type returned by T
 */

/**
 * General case for all unspecified types: not a function
 */
template <typename T, typename E = void>
struct FunctionTraits
{
    enum {isFunction = false};
};

/**
 * Specialization for native callable types (function and pointer-to-member types)
 */
template <typename T>
struct FunctionTraits<T, typename boost::enable_if<boost::function_types::is_callable_builtin<T> >::type>
{
    enum {isFunction = true};
    typedef typename boost::function_types::result_type<T>::type ReturnType;
};

/**
 * Specialization for functors (classes exporting a result_type type)
 */
template <typename T>
struct FunctionTraits<T, typename boost::enable_if<HasResultType<T> >::type>
{
    enum {isFunction = true};
    typedef typename T::result_type ReturnType;
};

} // namespace detail

} // namespace camp


#endif // CAMP_DETAIL_FUNCTIONTRAITS_HPP
