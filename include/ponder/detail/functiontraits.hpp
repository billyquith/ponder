/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2018 Nick Trout.
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

#pragma once
#ifndef PONDER_DETAIL_FUNCTIONTRAITS_HPP
#define PONDER_DETAIL_FUNCTIONTRAITS_HPP

#include <ponder/type.hpp>
#include "objecttraits.hpp"
#include <type_traits>
#include <array>
#include <vector>
#include <list>
#include <functional>

namespace ponder {
namespace detail {
namespace function {

template <typename T>
struct FunctionDetails {};

template <typename R, typename... A>
struct FunctionDetails<R(*)(A...)>
{
    typedef std::tuple<A...> ParamTypes;
    typedef R ReturnType;
    typedef ReturnType(*Type)(A...);
    typedef ReturnType(DispatchType)(A...);
    typedef std::tuple<A...> FunctionCallTypes;
};

template <typename R, typename... A>
struct FunctionDetails<R(A...)>
{
    typedef std::tuple<A...> ParamTypes;
    typedef R ReturnType;
    typedef ReturnType(*Type)(A...);
    typedef ReturnType(DispatchType)(A...);
    typedef std::tuple<A...> FunctionCallTypes;
};


// Class method
template <typename T>
struct MethodDetails {};

// Non-const method.
template <typename C, typename R, typename... A>
struct MethodDetails<R(C::*)(A...)>
{
    typedef C ClassType;
    typedef std::tuple<A...> ParamTypes;
    typedef R ReturnType;
    typedef ReturnType(ClassType::*Type)(A...);
    typedef ReturnType(DispatchType)(ClassType&, A...);
    typedef std::tuple<ClassType&, A...> FunctionCallTypes;
    static constexpr bool isConst = false;
};

// Const method.
template <typename C, typename R, typename... A>
struct MethodDetails<R(C::*)(A...) const>
{
    typedef const C ClassType;
    typedef std::tuple<A...> ParamTypes;
    typedef R ReturnType;
    typedef ReturnType(DispatchType)(const ClassType&, A...);
    typedef ReturnType(ClassType::*Type)(A...) const;
    typedef std::tuple<const ClassType&, A...> FunctionCallTypes;
    static constexpr bool isConst = true;
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
struct CallableDetails : public CallableDetails<decltype(&T::operator())>
{
    typedef T Type;
};

template <typename L, typename R, typename... A>
struct CallableDetails<R(L::*)(A...) const>
{
    typedef L LambdaClassType;    // N.B. Lambda class
    typedef std::tuple<A...> ParamTypes;
    typedef R ReturnType;
    typedef ReturnType(DispatchType)(A...);
    typedef std::tuple<A...> FunctionCallTypes;
};
    
/*
 * For functions, if they return an rvalue, we cannot return by reference.
 */
template <typename T, bool isWritable = true>
struct ReturnType
{
    typedef T& Type; // Not a rvalue so we can reference
};

template <>
struct ReturnType<void>
{
    typedef void Type;
};

template <typename T>
struct ReturnType<T, false>
{
    typedef T Type; // T is an rvalue so return by value
};

} // namespace function

/*
 * Uniform type declaration to all function types.
 *  - Used by property and function declaration, so not class specific.
 *  - DataType - scalar return type. E.g. int.
 *  - ExposedType - Stored type, e.g. int[].
 *  - getter/setter are both const functions but may reference non-const objects.
 *  - getter returns ExposedType and is set via DataType, which may be component, e.g. int[]
 */
template <typename T, typename E = void>
struct FunctionTraits
{
    static constexpr FunctionKind kind = FunctionKind::None;
};

/*
 * Specialization for native callable types (function and function pointer types)
 *  - We cannot derive a ClassType from these as they may not have one. e.g. int get()
 */
template <typename T>
struct FunctionTraits<T,
    typename std::enable_if<std::is_function<typename std::remove_pointer<T>::type>::value>::type>
{
    static constexpr FunctionKind kind = FunctionKind::Function;    
    typedef typename function::FunctionDetails<typename std::remove_pointer<T>::type> Details;
    typedef typename Details::Type Type;
    typedef typename Details::ReturnType ExposedType;
    typedef ReferenceTraits<ExposedType> RefTraits;
    static constexpr bool isWritable =
        std::is_lvalue_reference<ExposedType>::value
        && !std::is_const<typename RefTraits::DereferencedType>::value;
    typedef typename function::ReturnType<typename RefTraits::DereferencedType, isWritable>::Type AccessType;
    typedef typename RawType<AccessType>::Type DataType;
    typedef typename Details::DispatchType DispatchType;

    template <typename C, typename A>
    class TypeAccess
    {
    public:
        typedef C ClassType;
        typedef A AccessType;
        
        TypeAccess(Type d) : data(d) {}
        
        AccessType access(ClassType& c) const {return (*data)(c);}
    private:
        Type data;
    };
};

/*
 * Specialization for native callable types (member function types)
 */
template <typename T>
struct FunctionTraits<T, typename std::enable_if<std::is_member_function_pointer<T>::value>::type>
{
    static constexpr FunctionKind kind = FunctionKind::MemberFunction;
    typedef typename function::MethodDetails<T> Details;
    typedef typename Details::Type Type;
    typedef typename Details::ReturnType ExposedType;
    typedef ReferenceTraits<ExposedType> RefTraits;
    static constexpr bool isWritable =
        std::is_lvalue_reference<ExposedType>::value && !Details::isConst;
    typedef typename function::ReturnType<typename RefTraits::DereferencedType, isWritable>::Type AccessType;
    typedef typename RawType<AccessType>::Type DataType;
    typedef typename Details::DispatchType DispatchType;

    template <typename C, typename A>
    class TypeAccess
    {
    public:
        typedef C ClassType;
        typedef A AccessType;
        
        TypeAccess(Type d) : data(d) {}
        
        AccessType access(ClassType& c) const {return (c.*data)();}
    private:
        Type data;
    };
};

/**
 * Specialization for functors (classes exporting a result_type type, T operator() ).
 */
template <typename T>
struct FunctionTraits<T, typename
    std::enable_if<std::is_bind_expression<T>::value>::type>
{
    static constexpr FunctionKind kind = FunctionKind::BindExpression;
    typedef function::CallableDetails<T> Details;
    typedef typename Details::Type Type;
    typedef typename Details::ReturnType ExposedType;
    typedef ReferenceTraits<ExposedType> RefTraits;
    static constexpr bool isWritable =
        std::is_lvalue_reference<ExposedType>::value
        && !std::is_const<typename RefTraits::DereferencedType>::value;
    typedef typename function::ReturnType<typename RefTraits::DereferencedType, isWritable>::Type AccessType;
    typedef typename RawType<AccessType>::Type DataType;
    typedef typename Details::DispatchType DispatchType;
    
    template <typename C, typename A>
    class TypeAccess
    {
    public:
        typedef C ClassType;
        typedef A AccessType;
        
        TypeAccess(Type d) : data(d) {}
        
        AccessType access(ClassType& c) const {return data(c);}
    private:
        Type data;
    };
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
    typedef function::CallableDetails<T> Details;
    typedef typename Details::Type Type;
    typedef typename Details::ReturnType ExposedType;
    typedef ReferenceTraits<ExposedType> RefTraits;
    static constexpr bool isWritable =
        std::is_lvalue_reference<ExposedType>::value
        && !std::is_const<typename RefTraits::DereferencedType>::value;
    typedef typename function::ReturnType<typename RefTraits::DereferencedType, isWritable>::Type AccessType;
    typedef typename RawType<AccessType>::Type DataType;
    typedef typename Details::DispatchType DispatchType;

    template <typename C, typename A>
    class TypeAccess
    {
    public:
        typedef C ClassType;
        typedef A AccessType;
        
        TypeAccess(Type d) : data(d) {}
        
        AccessType access(ClassType& c) const {return data(c);}
    private:
        Type data;
    };
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
    typedef function::CallableDetails<T> Details;
    typedef T Type;
    typedef typename Details::ReturnType ExposedType;
    typedef ReferenceTraits<ExposedType> RefTraits;
    static constexpr bool isWritable =
        std::is_lvalue_reference<ExposedType>::value
        && !std::is_const<typename RefTraits::DereferencedType>::value;
    typedef typename function::ReturnType<typename RefTraits::DereferencedType, isWritable>::Type AccessType;
    typedef typename RawType<AccessType>::Type DataType;
    typedef typename Details::DispatchType DispatchType;

    template <typename C, typename A>
    class TypeAccess
    {
    public:
        typedef C ClassType;
        typedef A AccessType;

        TypeAccess(Type d) : data(d) {}
        
        AccessType access(ClassType& c) const {return data(c);}
    private:
        Type data;
    };
};

/*
 * Uniform access to the member type T.
 */
template <typename T>
struct MemberTraits;

template <typename C, typename T>
struct MemberTraits<T(C::*)>
{
    typedef T(C::*Type);    // full type inc ref
    typedef T ExposedType;  // the type exposed inc refs
    typedef ReferenceTraits<ExposedType> RefTraits;
    typedef typename RefTraits::DereferencedType AccessType; // deferenced type
    typedef typename RawType<AccessType>::Type DataType; // raw type or container
    static constexpr bool isWritable = !std::is_const<AccessType>::value;

    template <typename CLASS, typename A>
    class TypeAccess
    {
    public:
        typedef CLASS ClassType;
        typedef A AccessType;

        TypeAccess(const Type& d) : data(d) {}
        
        AccessType& access(ClassType& c) const {return c.*data;}
    private:
        Type data;
    };
};

} // namespace detail
} // namespace ponder

#endif // PONDER_DETAIL_FUNCTIONTRAITS_HPP
