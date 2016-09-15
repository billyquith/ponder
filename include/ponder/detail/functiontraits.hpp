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

#include <ponder/type.hpp>
#include <type_traits>
#include <array>
#include <vector>
#include <list>
#include <functional>

namespace ponder {
    
namespace detail {
    
namespace function {

// Function (ex-class)
template <typename T>
struct FunctionDetails
{
    typedef void ReturnType;
};

template <typename R, typename... A>
struct FunctionDetails<R(*)(A...)>
{
    typedef std::tuple<A...> ParamTypes;
    typedef R ReturnType;
    typedef ReturnType(*Typedef)(A...);
    typedef ReturnType(FunctionType)(A...);
    typedef std::tuple<A...> FunctionCallTypes;
};

template <typename R, typename... A>
struct FunctionDetails<R(A...)>
{
    typedef std::tuple<A...> ParamTypes;
    typedef R ReturnType;
    typedef ReturnType(Typedef)(A...);
    typedef ReturnType(FunctionType)(A...);
    typedef std::tuple<A...> FunctionCallTypes;
};
    
    
// Class method
template <typename T>
struct MethodDetails {};

template <typename C, typename R, typename... A>
struct MethodDetails<R(C::*)(A...)>
{
    typedef C ClassType;
    typedef std::tuple<A...> ParamTypes;
    typedef R ReturnType;
    typedef ReturnType(ClassType::*Typedef)(A...);
    typedef ReturnType(FunctionType)(ClassType&, A...);
    typedef std::tuple<ClassType&, A...> FunctionCallTypes;
};

template <typename C, typename R, typename... A>
struct MethodDetails<R(C::*)(A...) const>
{
    typedef const C ClassType;
    typedef std::tuple<A...> ParamTypes;
    typedef const R ReturnType;
    typedef ReturnType(FunctionType)(ClassType const&, A...);
    typedef ReturnType(ClassType::*Typedef)(A...) const;
    typedef std::tuple<const ClassType&, A...> FunctionCallTypes;
};


template<typename T, typename = void>
struct IsCallable : std::false_type {};
    
template<typename T>
struct IsCallable<T,
    typename std::enable_if< std::is_same<decltype(void(&T::operator())), void>::value
                             && !std::is_function<T>::value >::type> : std::true_type
{};

template <typename T, typename U = void>
struct IsFunctionWrapper : std::false_type {};

template <typename T>
struct IsFunctionWrapper<std::function<T>> : std::true_type {};

// T::operator() callable
template <typename T>
struct CallableDetails : public CallableDetails<decltype(&T::operator())> {};

template <typename C, typename R, typename... A>
struct CallableDetails<R(C::*)(A...) const>
{
    typedef C ClassType;
    typedef std::tuple<A...> ParamTypes;
    typedef R ReturnType;
    typedef R(Typedef)(A...);
    typedef R(FunctionType)(A...);
    typedef std::tuple<A...> FunctionCallTypes;
};


// References
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
    static constexpr FunctionKind kind = FunctionKind::None;
    static constexpr bool isFunction = false;
};

/**
 * Specialization for native callable types (function and function pointer types)
 */
template <typename T>
struct FunctionTraits<T,
    typename std::enable_if<std::is_function<typename std::remove_pointer<T>::type>::value>::type>
{
    static constexpr FunctionKind kind = FunctionKind::Function;
    static constexpr bool isFunction = true;
    
    typedef typename function::FunctionDetails<T> Details;
    typedef typename Details::FunctionType FunctionType;
    typedef typename Details::Typedef Typedef;
    typedef typename Details::ReturnType ReturnType;
};

/**
 * Specialization for native callable types (method pointer types)
 */
template <typename T>
struct FunctionTraits<T, typename std::enable_if<std::is_member_function_pointer<T>::value>::type>
{
    static constexpr FunctionKind kind = FunctionKind::MemberFunction;
    static constexpr bool isFunction = true;
    
    typedef typename function::MethodDetails<T> Details;
    typedef typename Details::FunctionType FunctionType;
    typedef typename Details::Typedef Typedef;
    typedef typename Details::ReturnType ReturnType;
};

/**
 * Specialization for native callable types (member pointer types)
 * This isn't a function, but we include it so that we can genericise member types.
 *
 * TODO - should member be wrapped (in lambda?) so we don't have to include this?
 */
template <typename T>
struct FunctionTraits<T, typename std::enable_if<std::is_member_object_pointer<T>::value>::type>
{
    static constexpr FunctionKind kind = FunctionKind::MemberObject;
    static constexpr bool isFunction = false;
    
    typedef typename function::RefDetails<T>::RefType ReturnType;
};

/**
 * Specialization for functors (classes exporting a result_type type, T operator() ).
 */
template <typename T>
struct FunctionTraits<T, typename std::enable_if<std::is_bind_expression<T>::value>::type>
{
    static constexpr FunctionKind kind = FunctionKind::BindExpression;
    static constexpr bool isFunction = true;
    
    typedef typename T::result_type ReturnType;
};

/**
 * Specialization for function wrappers (std::function<>).
 */
template <typename T>
struct FunctionTraits<T,
    typename std::enable_if<function::IsCallable<T>::value
                            && function::IsFunctionWrapper<T>::value>::type>
{
    static constexpr FunctionKind kind = FunctionKind::FunctionWrapper;
    static constexpr bool isFunction = true;
    
    typedef function::CallableDetails<T> Details;
    typedef typename Details::FunctionType FunctionType;
    typedef typename Details::Typedef Typedef;
    typedef typename Details::ReturnType ReturnType;
};

/**
 * Specialization for lambda functions ([](){}).
 */
template <typename T>
struct FunctionTraits<T,
    typename std::enable_if<function::IsCallable<T>::value
                            && !function::IsFunctionWrapper<T>::value>::type>
{
    static constexpr FunctionKind kind = FunctionKind::Lambda;
    static constexpr bool isFunction = true;
    
    typedef function::CallableDetails<T> Details;
    typedef typename Details::FunctionType FunctionType;
    typedef typename Details::Typedef Typedef;
    typedef typename Details::ReturnType ReturnType;
};

} // namespace detail
} // namespace ponder


#endif // PONDER_DETAIL_FUNCTIONTRAITS_HPP
