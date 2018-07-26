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
#ifndef PONDER_DETAIL_PROPERTYFACTORY_HPP
#define PONDER_DETAIL_PROPERTYFACTORY_HPP

#include <ponder/detail/simplepropertyimpl.hpp>
#include <ponder/detail/arraypropertyimpl.hpp>
#include <ponder/detail/enumpropertyimpl.hpp>
#include <ponder/detail/userpropertyimpl.hpp>
#include <ponder/detail/functiontraits.hpp>

namespace ponder {
namespace detail {
    
/*
 * Map accessed property type to traits.
 *  - Traits::Access: access to exposed type T
 *      - const T& getter() const
 *      - bool setter(...) const
 */
template <typename T, typename E = void>
struct PropertyTraitMapper
{
    static constexpr PropertyKind kind = PropertyKind::Function;
    typedef FunctionTraits<T> Traits;
};

template <typename T>
struct PropertyTraitMapper<T,
    typename std::enable_if<std::is_member_object_pointer<T>::value>::type>
{
    static constexpr PropertyKind kind = PropertyKind::MemberObject;
    typedef MemberTraits<T> Traits;
};
    
/*
 * Property implementation to be used for the Value kind.
 */
template <typename A, ValueKind K>
struct PropertyAccessMapper
{
    typedef SimplePropertyImpl<A> Type;
};

template <typename A>
struct PropertyAccessMapper<A, ValueKind::Array> //T,typename std::enable_if<ponder_ext::ArrayMapper<T>::isArray>::type>
{
    typedef ArrayPropertyImpl<A> Type;
};

template <typename A>
struct PropertyAccessMapper<A, ValueKind::Enum> // T, typename std::enable_if<std::is_enum<T>::value>::type>
{
    typedef EnumPropertyImpl<A> Type;
};

template <typename A>
struct PropertyAccessMapper<A, ponder::ValueKind::User>
{
    typedef UserPropertyImpl<A> Type;
};

/*
 * Helper structure to perform copy and assignment
 *
 * The purpose of this structure is to avoid a compiler error when the copied
 * type is not copyable. Instead, we just return an error so that the caller
 * can throw a Ponder exception.
 */
template <typename T, typename E = void>
struct CopyHelper
{
    static bool copy(T& destination, const Value& source)
    {
        destination = source.to<T>();
        return true;
    }
};

/*
 * Specialization of CopyHelper for non-copyable types
 */
template <typename T>
struct CopyHelper<T, typename std::enable_if< !StaticTypeId<T>::copyable>::type >
{
    static bool copy(T&, const Value&)
    {
        // We don't really return an error, we just skip the assignment
        return true;
    }
};

/*
 * Helper structure to return values
 *
 * The purpose of this structure is to provide workarounds for types
 * that don't exactly satisfy the compiler when returned. For example,
 * it converts smart pointer types to the corresponding raw pointer types.
 */
template <typename T, typename E = void>
struct AccessorReturn
{
    typedef T Type;
    static Type get(T value) {return value;}
};

/*
 * Specialization of AccessorReturn for smart pointer types
 */
template <template <typename> class T, typename U>
struct AccessorReturn<T<U>, std::enable_if< IsSmartPointer<T<U>, U>::value > >
{
    typedef U* Type;
    static Type get(T<U> value) {return get_pointer(value);}
};

/*
 * Specialization of AccessorReturn for built-in array types
 */
template <typename T, std::size_t N>
struct AccessorReturn<T[N]>
{
    typedef T (&Type)[N];
    static Type get(T (&value)[N]) {return value;}
};

/*
 * - Accessors provide a uniform interface to exposed data.
 * - Read-only accessor wrapper.
 */
template <class C, typename TRAITS, typename E = void>
class Accessor1
{
public:

    typedef TRAITS Traits;
    static_assert(!Traits::isWritable, "isWritable wrong or missing");
    typedef C ClassType;
    typedef typename Traits::DataType DataType; // raw type
    typedef typename Traits::AccessType AccessType; // type accessor exposes
    static constexpr bool canRead = true;
    static constexpr bool canWrite = false;

    typename Traits::template ClassAccess<ClassType> m_access;

    Accessor1(typename Traits::Type getter)
        : m_access(getter)
    {}

    AccessType get(const ClassType& object) const
    {
        return m_access.getter(object);
    }
    
    template <typename... A>
    bool set(A... args) const {return false;}   // read-only
};

/*
 * Read-write accessor wrapper.
 */
template <class C, typename TRAITS>
class Accessor1<C, TRAITS, typename std::enable_if<TRAITS::isWritable>::type>
{
public:

    typedef TRAITS Traits;
    static_assert(Traits::isWritable, "isWritable expected");
    typedef C ClassType;
    typedef typename Traits::DataType DataType; // raw type
    typedef typename Traits::AccessType AccessType; // type accessor exposes
    static constexpr bool canRead = true;
    static constexpr bool canWrite = true;
    
    typename Traits::template ClassAccess<ClassType> m_access;

    Accessor1(typename Traits::Type getter)
        : m_access(getter)
    {}

    AccessType get(const ClassType& object) const
    {
        return m_access.getter(object);
    }

    bool set(ClassType& object, const AccessType& value) const
    {
        return m_access.setter(object, value);
    }
};

/*
 * Property accessor composed of 1 getter and 1 setter
 */
template <typename C, typename FUNCTRAITS>
class Accessor2
{
public:

    typedef FUNCTRAITS Traits;
    typedef typename Traits::AccessType AccessType;
    typedef typename Traits::DataType DataType;
    typedef C ClassType;
    static constexpr bool canRead = true;
    static constexpr bool canWrite = true;

    template <typename F1, typename F2>
    Accessor2(F1 getter, F2 setter)
        : m_getter(getter)
        , m_setter(setter)
    {
    }

    AccessType get(const ClassType& object) const
    {
        return m_getter(object);
    }
    
    bool set(ClassType& object, const AccessType& value) const
    {
        return m_setter(object, value), true;
    }

private:

    std::function<typename Traits::DispatchType> m_getter;
    std::function<void(ClassType&, AccessType)> m_setter;
};
    
/*
 * Property factory which instantiates the proper type of property from 1 accessor.
 * The accessor can be a member object or a function.
 */
template <typename C, typename T>
struct PropertyFactory1
{
    static Property* get(IdRef name, T accessor)
    {
        typedef typename PropertyTraitMapper<T>::Traits PropertyTraits; // accessor family
        
        typedef Accessor1<C, PropertyTraits> AccessorType; // accessor wrapper
        
        // Properties return Values. Work out which type the property impl supports.
        typedef ponder_ext::ValueMapper<typename AccessorType::DataType> ValueType;
        
        // Property wrapper interface
        typedef typename PropertyAccessMapper<AccessorType, ValueType::kind>::Type PropertyType;
        
        return new PropertyType(name, AccessorType(accessor));
    }
};

/*
 * Property factory which instantiates the proper type of property from 2 accessors.
 * Bother getter and setter should be functions.
 */
template <typename C, typename F1, typename F2, typename E = void>
struct PropertyFactory2
{
    static Property* get(IdRef name, F1 accessor1, F2 accessor2)
    {
        typedef FunctionTraits<F1> Traits;
        typedef Accessor2<C, Traits> AccessorType;
        typedef ponder_ext::ValueMapper<typename AccessorType::DataType> ValueType;
        typedef typename PropertyAccessMapper<AccessorType, ValueType::kind>::Type PropertyType;
        return new PropertyType(name, AccessorType(accessor1, accessor2));
    }
};

} // namespace detail
} // namespace ponder

#endif // PONDER_DETAIL_PROPERTYFACTORY_HPP
