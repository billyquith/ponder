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

#include <ponder/detail/rawtype.hpp>
#include <vector>
#include <list>

namespace ponder {
namespace detail {    
    
/**
 * \class ObjectTraits
 *
 * \brief Utility class which gives compile-time information about the semantics of a type
 *
 * It is called ObjectTraits as the type is expected to be related to an object, e.g. an
 * instance, or reference to, or pointer to.
 *
 * It provides the following constants:
 *
 * \li isWritable: true if the type allows to modify the object (non-const references and pointers)
 * \li isRef: true if the type is a reference type (references and pointers)
 *
 * ... the following types:
 *
 * \li RefReturnType: the reference type closest to T which allows to have direct access 
 *     to the object (T& for raw types and references, T* for pointer types)
 * \li PointerType: the pointer type closest to T which allows to have direct access to 
 *     the object (T*)
 * \li DataType: the actual raw type of the object (removes all indirections, as well const 
 *     and reference modifiers)
 *
 * ... and the following functions:
 *
 * \li get(void*): get a direct access to an object given by a typeless pointer (in other 
 *     words, converts from void* to RefReturnType)
 * \li getPointer(T): get a direct pointer to an object, regardless its original 
       storage / modifiers (in other words, convert from T to PointerType)
 */

/*
 * Extract details about objects we reference.
 */
template <typename T>
struct ObjectDetails
{
    typedef T ReturnType;
};

template <typename T>
struct ObjectDetails<T*>
{
    typedef T ReturnType;
};

template <typename C, typename T>
struct ObjectDetails<T(C::*)>
{
    typedef C ClassType;
    typedef T& ReturnType;
};

template <typename C, typename T, std::size_t S>
struct ObjectDetails<T(C::*)[S]>
{
    typedef C ClassType;
    typedef T(&ReturnType)[S];
};

template <typename C, typename T, std::size_t S>
struct ObjectDetails<std::array<T,S>(C::*)>
{
    typedef C ClassType;
    typedef std::array<T,S>(&ReturnType);
};

template <typename C, typename T>
struct ObjectDetails<std::vector<T>(C::*)>
{
    typedef C ClassType;
    typedef std::vector<T>(&ReturnType);
};

template <typename C, typename T>
struct ObjectDetails<std::list<T>(C::*)>
{
    typedef C ClassType;
    typedef std::list<T>(&ReturnType);
};
    
/*
 * Adapt the type to make the binding work.
 * Generic version -- raw types
 */
template <typename T, typename E = void>
struct ObjectTraits
{
    static constexpr bool isWritable = false;
    static constexpr bool isRef = false;
    static constexpr ObjectKind kind = ObjectKind::Object;
    typedef T& RefReturnType;
    typedef T* PointerType;
    typedef typename RawType<T>::Type DataType;

    static RefReturnType get(void* pointer) {return *static_cast<T*>(pointer);}
    static PointerType getPointer(T& value) {return &value;}
};

/*
 * Raw pointers
 */
template <typename T>
struct ObjectTraits<T*>
{
    static constexpr bool isWritable = !std::is_const<T>::value;
    static constexpr bool isRef = true;
    static constexpr ObjectKind kind = ObjectKind::Pointer;
    typedef T* RefReturnType;
    typedef T* PointerType;
    typedef typename RawType<T>::Type DataType;

    static RefReturnType get(void* pointer) {return static_cast<T*>(pointer);}
    static PointerType getPointer(T* value) {return value;}
};

/*
 * Smart pointers
 */
template <template <typename> class T, typename U>
struct ObjectTraits<T<U>, typename std::enable_if<IsSmartPointer<T<U>, U>::value>::type>
{
    static constexpr bool isWritable = !std::is_const<U>::value;
    static constexpr bool isRef = true;
    static constexpr ObjectKind kind = ObjectKind::SmartPointer;
    typedef U* RefReturnType;
    typedef U* PointerType;
    typedef typename RawType<U>::Type DataType;

    static RefReturnType get(void* pointer)   {return static_cast<U*>(pointer);}
    static PointerType getPointer(T<U> value) {return get_pointer(value);}
};

/*
 * Built-in arrays
 */
template <typename T, std::size_t N>
struct ObjectTraits<T[N]>
{
    static constexpr bool isWritable = false;
    static constexpr bool isRef = true;
    static constexpr ObjectKind kind = ObjectKind::BuiltinArray;

    typedef T(&RefReturnType)[N];
    typedef typename RawType<T>::Type DataType;
};

/*
 * References to non-ref types
 */
template <typename T>
struct ObjectTraits<T&,
            typename std::enable_if<
                !std::is_pointer<typename ObjectTraits<T>::RefReturnType>::value >::type>
{
    static constexpr bool isWritable = !std::is_const<T>::value;
    static constexpr bool isRef = true;
    static constexpr ObjectKind kind = ObjectKind::Reference;
    typedef T& RefReturnType;
    typedef T* PointerType;
    typedef typename RawType<T>::Type DataType;

    static RefReturnType get(void* pointer) {return *static_cast<T*>(pointer);}
    static PointerType getPointer(T& value) {return &value;}
};

/*
 * References to ref types -- just remove the reference modifier
 */
template <typename T>
struct ObjectTraits<T&,
            typename std::enable_if<
                std::is_pointer<typename ObjectTraits<T>::RefReturnType>::value >::type>
    : ObjectTraits<T>
{
};

/*
 * Types with const modifier -- just remove it
 */
template <typename T>
struct ObjectTraits<const T> : ObjectTraits<T>
{
};

} // namespace detail
} // namespace ponder

#endif // PONDER_DETAIL_OBJECTTRAITS_HPP
