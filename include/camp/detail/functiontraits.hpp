/****************************************************************************
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
**
** This file is part of the CAMP library.
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


#ifndef CAMP_DETAIL_FUNCTIONTRAITS_HPP
#define CAMP_DETAIL_FUNCTIONTRAITS_HPP


#include <camp/detail/yesnotype.hpp>
#include <boost/function_types/result_type.hpp>
#include <type_traits>

#include <boost/type_index.hpp> // boost::typeindex::type_id<T>().pretty_name() // human readable
#include <iostream>


//--- debug ---
template <typename T>
void dumpType(const std::string& comment = std::string()) {
    std::cout << comment << (comment.empty() ? "" : " : ") << boost::typeindex::type_id<T>().pretty_name() << std::endl;
}
//--- debug ---


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


template <typename T>
struct FunctionDetails
{
    typedef void ReturnType;
};
    
template <typename R, typename ...A>
struct FunctionDetails<R(*)(A...)>
{
    typedef R ReturnType;
};
    
    
template <typename T>
struct MethodDetails {};

template <typename C, typename R, typename ...A>
struct MethodDetails<R(C::*)(A...)>
{
    typedef C ClassType;
    typedef R(FunctionType)(C&, A...);
    typedef R ReturnType;
};

template <typename C, typename R, typename ...A>
struct MethodDetails<R(C::*)(A...) const>
{
    typedef C ClassType;
    typedef R(FunctionType)(C&, A...) const;
    typedef R ReturnType;
};
    

template <typename T>
struct RefDetails
{
    typedef void ReturnType;
};

template <typename T>
struct RefDetails<T*>
{
    typedef T RefType;
};

template <typename C, typename T>
struct RefDetails<T(C::*)>
{
    typedef C ClassType;
    typedef T RefType;
};

template <typename C, typename T, int S>
struct RefDetails<T(C::*)[S]>
{
    typedef C ClassType;
    typedef T RefType[S];
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
    enum {isFunction = false, isMOP = false};
};

/**
 * Specialization for native callable types (function and function pointer types)
 */
template <typename T>
struct FunctionTraits<T, typename std::enable_if<std::is_function<typename std::remove_pointer<T>::type>::value>::type >
{
    enum {isFunction = true};
    typedef typename std::remove_pointer<T>::type type;
    typedef typename FunctionDetails<T>::ReturnType ReturnType;
};

/**
 * Specialization for native callable types (method pointer types)
 */
template <typename T>
struct FunctionTraits<T, typename std::enable_if<std::is_member_function_pointer<T>::value>::type >
{
    enum {isFunction = true};
    typedef typename MethodDetails<T>::FunctionType type;
    typedef typename MethodDetails<T>::ReturnType ReturnType;
};

/**
 * Specialization for native callable types (member pointer types)
 */
template <typename T>
struct FunctionTraits<T, typename std::enable_if<std::is_member_object_pointer<T>::value>::type >
{
    enum {isFunction = true, isMOP = true};
    
                    typedef typename boost::function_types::result_type<T>::type ReturnType;
//                 typedef typename RefDetails<T>::RefType ReturnType;
};

/**
 * Specialization for functors (classes exporting a result_type type)
 */
template <typename T>
struct FunctionTraits<T, typename std::enable_if<HasResultType<T>::value >::type>
{
    enum {isFunction = true};
    typedef typename T::result_type ReturnType;
};

} // namespace detail

} // namespace camp


#endif // CAMP_DETAIL_FUNCTIONTRAITS_HPP
