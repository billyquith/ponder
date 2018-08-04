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
#ifndef PONDER_DETAIL_OBJECTTRAITS_HPP
#define PONDER_DETAIL_OBJECTTRAITS_HPP

#include "rawtype.hpp"
#include <vector>
#include <list>

namespace ponder {
namespace detail {
    
/*
 * - ReferenceType: the reference type closest to T which allows to have direct access
 *   to the object (T& for raw types and references, T* for pointer types)
 *   Note: not T& reference type!
 * - PointerType: the pointer type closest to T which allows to have direct access to
 *   the object (T*)
 * - DataType: the actual raw type of the object (removes all indirections, as well const
 *   and reference modifiers)
 *
 * - isWritable: true if the type allows to modify the object (non-const references and pointers)
 * - isRef: true if the type is a reference type (references and pointers)
 *
 * - get(void*): get a direct access to an object given by a typeless pointer (in other
 *   words, converts from void* to ReferenceType)
 * - getPointer(T): get a direct pointer to an object, regardless its original
 *   storage / modifiers (in other words, convert from T to PointerType)
 */

// How we access an instance of type T.
template <typename T, typename E = void>
struct ReferenceDetails
{
    typedef int unhandled_type[-(int)sizeof(T)];
};

// Object instance.
template <typename T>
struct ReferenceDetails<T>
{
    static constexpr ReferenceKind kind = ReferenceKind::Instance;
    typedef T& ReferenceType;
    typedef T* PointerType;
    typedef T DereferencedType;
    typedef typename RawType<T>::Type DataType;
    static constexpr bool isWritable = !std::is_void<DereferencedType>::value && !std::is_const<DereferencedType>::value;
    static constexpr bool isRef = false;

    static inline ReferenceType get(void* pointer) {return *static_cast<T*>(pointer);}
    static inline PointerType getPointer(T& value) {return &value;}
    static inline PointerType getPointer(T* value) {return value;}
};

// Raw pointers
template <typename T>
struct ReferenceDetails<T*>
{
    static constexpr ReferenceKind kind = ReferenceKind::Pointer;
    typedef T* ReferenceType;
    typedef T* PointerType;
    typedef T DereferencedType;
    typedef typename RawType<T>::Type DataType;
    static constexpr bool isWritable = !std::is_const<DereferencedType>::value;
    static constexpr bool isRef = true;

    static inline ReferenceType get(void* pointer) {return static_cast<T*>(pointer);}
    static inline PointerType getPointer(T& value) {return &value;}
    static inline PointerType getPointer(T* value) {return value;}
};

// References
template <typename T>
struct ReferenceDetails<T&>
{
    static constexpr ReferenceKind kind = ReferenceKind::Reference;
    typedef T& ReferenceType;
    typedef T* PointerType;
    typedef T DereferencedType;
    typedef typename RawType<T>::Type DataType;
    static constexpr bool isWritable = !std::is_const<DereferencedType>::value;
    static constexpr bool isRef = true;

    static inline ReferenceType get(void* pointer) {return *static_cast<T*>(pointer);}
    static inline PointerType getPointer(T& value) {return &value;}
    static inline PointerType getPointer(T* value) {return value;}
};

// Base class for smart pointers
template <class P, typename T>
struct SmartPointerReferenceTraits
{
    typedef P Type;
    static constexpr ReferenceKind kind = ReferenceKind::SmartPointer;
    typedef T& ReferenceType;
    typedef P PointerType;
    typedef T DereferencedType;
    typedef typename RawType<T>::Type DataType;
    static constexpr bool isWritable = !std::is_const<DereferencedType>::value;
    static constexpr bool isRef = true;

    static inline ReferenceType get(void* pointer)   {return *static_cast<P*>(pointer);}
    static inline PointerType getPointer(P& value) {return get_pointer(value);}
};

// std::shared_ptr<>
template <typename T>
struct ReferenceDetails<std::shared_ptr<T>>
    : public SmartPointerReferenceTraits<std::shared_ptr<T>,T> {};


template <typename T>
struct ReferenceTraits : public ReferenceDetails<T> {};

// void
template <>
struct ReferenceTraits<void>
{
    static constexpr ReferenceKind kind = ReferenceKind::None;
    typedef void T;
    typedef T* ReferenceType;
    typedef T* PointerType;
    typedef T DereferencedType;
    typedef typename RawType<T>::Type DataType;
    static constexpr bool isWritable = false;
    static constexpr bool isRef = false;

    static inline ReferenceType get(void* pointer) {return 0;}
    static inline PointerType getPointer(T* value) {return value;}
};

    
// Built-in arrays []
//template <typename T, std::size_t N>
//struct ReferenceTraits<T[N]> //, typename std::enable_if< std::is_array<T>::value >::type>
//{
//    static constexpr ReferenceKind kind = ReferenceKind::BuiltinArray;
//    typedef typename RawType<T>::Type DataType;
//    typedef T(&ReferenceType)[N];
//    typedef T* PointerType;
//    static constexpr std::size_t Size = N;
//    static constexpr bool isWritable = !std::is_const<T>::value;
//    static constexpr bool isRef = false;
//};


//template <typename C, typename T, std::size_t S>
//struct MemberTraits<std::array<T,S>(C::*)>
//{
//    typedef std::array<T,S>(C::*Type);
//    typedef C ClassType;
//    typedef std::array<T,S> ExposedType;
//    typedef typename RawType<T>::Type DataType;
//    //static constexpr bool isWritable = !std::is_const<DataType>::value;
//
//    class Access
//    {
//    public:
//        Access(Type d) : data(d) {}
//        ExposedType& getter(ClassType& c) const { return c.*data; }
//    private:
//        Type data;
//    };
//};
//
//template <typename C, typename T>
//struct MemberTraits<std::vector<T>(C::*)>
//{
//    typedef std::vector<T>(C::*Type);
//    typedef C ClassType;
//    typedef std::vector<T> ExposedType;
//    typedef typename RawType<T>::Type DataType;
//
//    class Access
//    {
//    public:
//        Access(Type d) : data(d) {}
//        ExposedType& getter(ClassType& c) const { return c.*data; }
//    private:
//        Type data;
//    };
//};
//
//template <typename C, typename T>
//struct MemberTraits<std::list<T>(C::*)>
//{
//    typedef std::list<T>(C::*Type);
//    typedef C ClassType;
//    typedef std::list<T> ExposedType;
//    typedef typename RawType<T>::Type DataType;
//
//    class Access
//    {
//    public:
//        Access(Type d) : data(d) {}
//        ExposedType& getter(ClassType& c) const { return c.*data; }
//    private:
//        Type data;
//    };
//};
    
} // namespace detail
} // namespace ponder

#endif // PONDER_DETAIL_OBJECTTRAITS_HPP
