/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2016 Billy Quith.
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


#ifndef PONDER_DETAIL_FUNCTIONTRAITS_HPP
#define PONDER_DETAIL_FUNCTIONTRAITS_HPP


#include <type_traits>
#include <array>
#include <vector>
#include <list>
#include <functional>

namespace ponder {
namespace detail {
    
namespace function {

template <typename T>
struct FunctionDetails
{
    typedef void ReturnType;
};

// Function
template <typename R, typename ...A>
struct FunctionDetails<R(*)(A...)>
{
    typedef R ReturnType;
};
    
    
// Class method
template <typename T>
struct MethodDetails {};

template <typename C, typename R, typename ...A>
struct MethodDetails<R(C::*)(A...)>
{
    typedef C ClassType;
    typedef R ReturnType;
    typedef ReturnType(FunctionType)(ClassType&, A...);
};

template <typename C, typename R, typename ...A>
struct MethodDetails<R(C::*)(A...) const>
{
    typedef const C ClassType;
    typedef const R ReturnType;
    typedef ReturnType(FunctionType)(ClassType const&, A...);
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
    typedef T& RefType;
};

template <typename C, typename T, int S>
struct RefDetails<T(C::*)[S]>
{
    typedef C ClassType;
    typedef T(&RefType)[S];
};

template <typename C, typename T, int S>
struct RefDetails<std::array<T,S>(C::*)>
{
    typedef C ClassType;
    typedef std::array<T,S>(&RefType);
};

template <typename C, typename T>
struct RefDetails<std::vector<T>(C::*)>
{
    typedef C ClassType;
    typedef std::vector<T>(&RefType);
};

template <typename C, typename T>
struct RefDetails<std::list<T>(C::*)>
{
    typedef C ClassType;
    typedef std::list<T>(&RefType);
};

} // namespace function
    
// Type of function recognised.
enum class FunctionType
{
    None,               // not a function
    Function,           // a function
    MemberFunction,     // function in a class or struct
    MemberObject,       // object in a class or struct
    FunctionWrapper,    // std::function<>
    BindExpression      // std::bind()
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
    static constexpr bool isFunction = false;
    static constexpr FunctionType which = FunctionType::None;
};

/**
 * Specialization for native callable types (function and function pointer types)
 */
template <typename T>
struct FunctionTraits<T,
    typename std::enable_if<std::is_function<typename std::remove_pointer<T>::type>::value>::type>
{
    static constexpr bool isFunction = true;
    static constexpr FunctionType which = FunctionType::Function;
    typedef typename std::remove_pointer<T>::type type;
    typedef typename function::FunctionDetails<T>::ReturnType ReturnType;
};

/**
 * Specialization for native callable types (method pointer types)
 */
template <typename T>
struct FunctionTraits<T, typename std::enable_if<std::is_member_function_pointer<T>::value>::type>
{
    static constexpr bool isFunction = true;
    static constexpr FunctionType which = FunctionType::MemberFunction;
    typedef typename function::MethodDetails<T>::FunctionType type;
    typedef typename function::MethodDetails<T>::ReturnType ReturnType;
};

/**
 * Specialization for native callable types (member pointer types)
 */
template <typename T>
struct FunctionTraits<T, typename std::enable_if<std::is_member_object_pointer<T>::value>::type>
{
    static constexpr bool isFunction = true;
    static constexpr FunctionType which = FunctionType::MemberObject;
    typedef typename function::RefDetails<T>::RefType ReturnType;
};

/**
 * Specialization for functors (classes exporting a result_type type)
 */
template <typename T>
struct FunctionTraits<T, typename std::enable_if<std::is_bind_expression<T>::value>::type>
{
    static constexpr bool isFunction = true;
    static constexpr FunctionType which = FunctionType::BindExpression;
    typedef typename T::result_type ReturnType;
};

/**
 * Specialization for function wrappers (std::function).
 */
template <typename R, typename ...Args>
struct FunctionTraits<std::function<R(Args...)>>
{
    static constexpr bool isFunction = true;
    static constexpr FunctionType which = FunctionType::FunctionWrapper;
    typedef R(type)(Args...);
    typedef R ReturnType;
};

} // namespace detail
} // namespace ponder


#endif // PONDER_DETAIL_FUNCTIONTRAITS_HPP
