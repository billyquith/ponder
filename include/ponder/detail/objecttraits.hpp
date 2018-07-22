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
    
//enum class PropertyAccessKind
//{
//    Basic,      // Basic type
//    Array,      // Array
//    Enum        // Enumeration
//};
    
/*
 * Uniform access to the member type T.
 */
template <typename T>
struct MemberTraits;

template <typename C, typename T>
struct MemberTraits<T(C::*)>
{
    typedef T(C::*Type);
    typedef C ClassType;
    typedef T AccessType;
    typedef typename RawType<T>::Type DataType;
    static constexpr bool isWritable = !std::is_const<T>::value;

    class Access
    {
    public:
        Access(Type d) : data(d) {}
        AccessType getter(const ClassType& c) const {return c.*data;}
        bool setter(ClassType& c, AccessType v) const {return c.*data = v, isWritable;}
    private:
        Type data;
    };
};

//template <typename C, typename T, std::size_t S>
//struct MemberTraits<T(C::*)[S]>
//{
//    typedef T(C::*Type)[S];
//    typedef C ClassType;
//    typedef typename RawType<T>::Type DataType;
//    typedef T AccessType;
//    static constexpr bool isWritable = !std::is_const<DataType>::value;
//
//    static_assert(std::is_array<DataType>::value, "Expected array");
//    static_assert(!std::is_array<AccessType>::value, "Unexpected array");
//
//    class Access
//    {
//    public:
//        Access(Type d) : data(d) {}
//        AccessType getter(ClassType& c) const { return c.*data; }
//    private:
//        Type data;
//    };
//};
//
//template <typename C, typename T, std::size_t S>
//struct MemberTraits<std::array<T,S>(C::*)>
//{
//    typedef std::array<T,S>(C::*Type);
//    typedef C ClassType;
//    typedef std::array<T,S> AccessType;
//    typedef typename RawType<T>::Type DataType;
//    //static constexpr bool isWritable = !std::is_const<DataType>::value;
//
//    class Access
//    {
//    public:
//        Access(Type d) : data(d) {}
//        AccessType& getter(ClassType& c) const { return c.*data; }
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
//    typedef std::vector<T> AccessType;
//    typedef typename RawType<T>::Type DataType;
//
//    class Access
//    {
//    public:
//        Access(Type d) : data(d) {}
//        AccessType& getter(ClassType& c) const { return c.*data; }
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
//    typedef std::list<T> AccessType;
//    typedef typename RawType<T>::Type DataType;
//
//    class Access
//    {
//    public:
//        Access(Type d) : data(d) {}
//        AccessType& getter(ClassType& c) const { return c.*data; }
//    private:
//        Type data;
//    };
//};

namespace object {

template <typename T>
struct ObjectDetails
{
    typedef T Type;
    typedef typename RawType<T>::Type DataType;
    typedef T AccessType;
    static constexpr bool isWritable = !std::is_const<DataType>::value;
    
//    static_assert(std::is_array<DataType>::value, "Expected array");
//    static_assert(!std::is_array<AccessType>::value, "Unexpected array");
    
//    class Access
//    {
//    public:
//        Access(Type d) : data(d) {}
//        template <typename C>
//        AccessType getter(C& object) const { return object.*data; }
//    private:
//        Type data;
//    };
};

//template <typename C, typename T, std::size_t S>
//struct ObjectDetails<T(C::*)[S]>
//{
//    typedef T(C::*Type)[S];
//    typedef C ClassType;
//    typedef typename RawType<T>::Type DataType;
//    typedef T AccessType;
//    static constexpr bool isWritable = !std::is_const<DataType>::value;
//
//    static_assert(std::is_array<DataType>::value, "Expected array");
//    static_assert(!std::is_array<AccessType>::value, "Unexpected array");
//
//    class Access
//    {
//    public:
//        Access(Type d) : data(d) {}
//        AccessType getter(ClassType& c) const { return c.*data; }
//    private:
//        Type data;
//    };
//};

    //template <typename C, typename T, std::size_t S>
    //struct MemberTraits<std::array<T,S>(C::*)>
    //{
    //    typedef std::array<T,S>(C::*Type);
    //    typedef C ClassType;
    //    typedef std::array<T,S> AccessType;
    //    typedef typename RawType<T>::Type DataType;
    //    //static constexpr bool isWritable = !std::is_const<DataType>::value;
    //
    //    class Access
    //    {
    //    public:
    //        Access(Type d) : data(d) {}
    //        AccessType& getter(ClassType& c) const { return c.*data; }
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
    //    typedef std::vector<T> AccessType;
    //    typedef typename RawType<T>::Type DataType;
    //
    //    class Access
    //    {
    //    public:
    //        Access(Type d) : data(d) {}
    //        AccessType& getter(ClassType& c) const { return c.*data; }
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
    //    typedef std::list<T> AccessType;
    //    typedef typename RawType<T>::Type DataType;
    //
    //    class Access
    //    {
    //    public:
    //        Access(Type d) : data(d) {}
    //        AccessType& getter(ClassType& c) const { return c.*data; }
    //    private:
    //        Type data;
    //    };
    //};
    
} // namespace object
    
/*
 * - isWritable: true if the type allows to modify the object (non-const references and pointers)
 * - isRef: true if the type is a reference type (references and pointers)
 *
 * - ReferenceType: the reference type closest to T which allows to have direct access
 *   to the object (T& for raw types and references, T* for pointer types)
 * - PointerType: the pointer type closest to T which allows to have direct access to
 *   the object (T*)
 * - DataType: the actual raw type of the object (removes all indirections, as well const
 *   and reference modifiers)
 *
 * - get(void*): get a direct access to an object given by a typeless pointer (in other
 *   words, converts from void* to ReferenceType)
 * - getPointer(T): get a direct pointer to an object, regardless its original
 *   storage / modifiers (in other words, convert from T to PointerType)
 */

/*
 * How we access an instance of type T with different references.
 *
 * An instance:
 */
template <typename T, typename E = void>
struct ReferenceTraits
{
    static constexpr ReferenceKind kind = ReferenceKind::Object;
    typedef object::ObjectDetails<T> Details;
    typedef T Type;
    static constexpr bool isWritable = Details::isWritable;
    static constexpr bool isRef = false;
    typedef T& ReferenceType;
    typedef T* PointerType;
    typedef T DataType;

    static inline ReferenceType get(void* pointer) {return *static_cast<T*>(pointer);}
    static inline PointerType getPointer(T& value) {return &value;}
};

/*
 * References to non-ref types
 */
template <typename T>
struct ReferenceTraits<T&, typename std::enable_if<!std::is_pointer<T>::value>::type>
{
    static constexpr ReferenceKind kind = ReferenceKind::Reference;
    typedef object::ObjectDetails<T> Details;
    static constexpr bool isWritable = !std::is_const<T>::value;
    static constexpr bool isRef = true;
    typedef T Type;
    typedef T& ReferenceType;
    typedef T* PointerType;
    typedef typename RawType<T>::Type DataType;

    static ReferenceType get(void* pointer) {return *static_cast<T*>(pointer);}
    static PointerType getPointer(T& value) {return &value;}
};

/*
 * Smart pointers
 */
template <template <typename> class T, typename U>
struct ReferenceTraits<T<U>, typename std::enable_if<IsSmartPointer<T<U>, U>::value>::type>
{
    static constexpr ReferenceKind kind = ReferenceKind::SmartPointer;
    typedef object::ObjectDetails<T<U>> Details;
    typedef T<U> Type;
    static constexpr bool isWritable = !std::is_const<U>::value;
    static constexpr bool isRef = true;
    typedef U& ReferenceType;
    typedef T<U> PointerType;
    typedef U DataType;

    static ReferenceType get(void* pointer)   {return *static_cast<U*>(pointer);}
    static PointerType getPointer(T<U>& value) {return get_pointer(value);}
};

/*
 * Raw pointers
 */
//template <typename T>
//struct ReferenceTraits<T*>
//{
//    typedef T Type;
//    typedef MemberTraits<T> Details;
//    static constexpr ReferenceKind kind = ReferenceKind::Pointer;
//    static constexpr bool isWritable = !std::is_const<T>::value;
//    static constexpr bool isRef = true;
//    typedef T* ReferenceType;
//    typedef T* PointerType;
//    typedef typename RawType<T>::Type DataType;
//
//    static ReferenceType get(void* pointer) {return static_cast<T*>(pointer);}
//    static PointerType getPointer(T* value) {return value;}
//};

    
/*
 * References to non-ref types
 */
//template <typename T>
//struct ReferenceTraits<T&, typename std::enable_if<!std::is_pointer<T>::value>::type>
//{
//    static constexpr ReferenceKind kind = ReferenceKind::Reference;
//    static constexpr bool isWritable = !std::is_const<T>::value;
//    static constexpr bool isRef = true;
//    typedef T Type;
//    typedef MemberTraits<T> Details;
//    typedef T& ReferenceType;
//    typedef T* PointerType;
//    typedef typename RawType<T>::Type DataType;
//
//    static ReferenceType get(void* pointer) {return *static_cast<T*>(pointer);}
//    static PointerType getPointer(T& value) {return &value;}
//};

/*
 * References to ref types -- just remove the reference modifier
 */
//template <typename T>
//struct ReferenceTraits<T&> //, typename std::enable_if<std::is_pointer<T>::type>>
//    : ReferenceTraits<T>
//{
//};
//
///*
// * Types with const modifier -- just remove it
// */
//template <typename T>
//struct ReferenceTraits<const T> : ReferenceTraits<T>
//{
//};
//

/*
 * Built-in arrays
 */
//template <typename T, std::size_t N>
//struct ReferenceTraits<T(&)[N], typename std::enable_if< std::is_array<T>::value >::type>
//{
//    static constexpr ReferenceKind kind = ReferenceKind::BuiltinArray;
//    static constexpr bool isWritable = false;
//    static constexpr bool isRef = true;
//    typedef T Type[N];
//    typedef MemberTraits<T> Details;
//    typedef T(&ReferenceType)[N];
//    typedef typename RawType<T>::Type DataType;
//};

///*
// * References to non-ref types
// */
//template <typename T>
//struct ReferenceTraits<T&> //,
////            typename std::enable_if<
////                !std::is_pointer<typename ReferenceTraits<T>::ReferenceType>::value >::type>
//{
//    static constexpr ReferenceKind kind = ReferenceKind::Reference;
//    static constexpr bool isWritable = !std::is_const<T>::value;
//    static constexpr bool isRef = true;
//    typedef T Type;
//    typedef MemberTraits<T> Details;
//    typedef T& ReferenceType;
//    typedef T* PointerType;
//    typedef typename RawType<T>::Type DataType;
//
//    static ReferenceType get(void* pointer) {return *static_cast<T*>(pointer);}
//    static PointerType getPointer(T& value) {return &value;}
//};

} // namespace detail
} // namespace ponder

#endif // PONDER_DETAIL_OBJECTTRAITS_HPP
