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
#include <ponder/detail/typeid.hpp>

namespace ponder {
namespace detail {
    
/*
 * Map accessed property type to traits.
 *  - PropertyTrait::Traits supplies:
 *      - Type : accessor type (member object/function pointer).
 *      - ExposedType : The type T referenced.
 *      - DataType : The dereferenced, raw type.
 *      - isWritable : Boolean; is ExposedType writable?
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
 * How to deal with the thing we are accessing. Type interface traits.
 *  - T : the type of the accessed value
 *  - A : the accessor; what gets the value
 */
template <typename T, typename E = void>
struct AccessTraits
{
    static constexpr PropertyAccessKind kind = PropertyAccessKind::Simple;
    
    template <typename A> struct Impl
    {
        typedef SimplePropertyImpl<A> Type;
    };
};

template <typename T>
struct AccessTraits<T, typename std::enable_if<std::is_enum<T>::value>::type>
{
    static constexpr PropertyAccessKind kind = PropertyAccessKind::Enum;
    
    template <typename A> struct Impl
    {
        typedef EnumPropertyImpl<A> Type;
    };
};

template <typename T>
struct AccessTraits<T, typename std::enable_if<ponder_ext::ArrayMapper<T>::isArray>::type>
{
    static constexpr PropertyAccessKind kind = PropertyAccessKind::Container;
    typedef ponder_ext::ArrayMapper<T> TypeTraits;
    
    template <typename A> struct Impl
    {
        typedef ArrayPropertyImpl<A> Type;
    };
};

template <typename T>
struct AccessTraits<T, typename std::enable_if<StaticTypeId<T>::defined>::type>
{
    static constexpr PropertyAccessKind kind = PropertyAccessKind::User;
    
    template <typename A> struct Impl
    {
        typedef UserPropertyImpl<A> Type;
    };
};

template <typename T>
struct AccessTraits<T&>
{
    typedef int TypeShouldBeDereferenced[-(int)sizeof(T)];
};

template <typename T>
struct AccessTraits<T*>
{
    typedef int TypeShouldBeDereferenced[-(int)sizeof(T)];
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
    static_assert(!Traits::isWritable, "!isWritable expected");
    typedef C ClassType;
    typedef typename Traits::ExposedType ExposedType;
    typedef ReferenceTraits<ExposedType> RefTraits;
    typedef typename RefTraits::DereferencedType AccessType;
    typedef typename Traits::DataType DataType; // raw type
    static constexpr bool canRead = true;
    static constexpr bool canWrite = false;

    typename Traits::template ClassAccess<ClassType> m_access;

    Accessor1(typename Traits::Type getter)
        : m_access(getter)
    {}

    ExposedType get(const ClassType& object) const
    {
        return m_access.getter(object);
    }
    
    template <typename... A>
    bool set(A... args) const {return false;}   // read-only

    //bool set(ClassType&, const DataType&) const {return false;} // read-only
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
    typedef typename Traits::ExposedType ExposedType;
    typedef ReferenceTraits<ExposedType> RefTraits;
    typedef typename RefTraits::DereferencedType AccessType;
    typedef typename Traits::DataType DataType; // raw type
    static constexpr bool canRead = true;
    static constexpr bool canWrite = true;
    
    typename Traits::template ClassAccess<ClassType> m_access;

    Accessor1(typename Traits::Type getter)
        : m_access(getter)
    {}

    ExposedType get(const ClassType& object) const
    {
        return m_access.getter(object);
    }

    bool set(ClassType& object, const DataType& value) const
    {
        return m_access.setter(object, value);
    }
    bool set(ClassType& object, DataType&& value) const
    {
        return m_access.setter(object, std::move(value));
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
    typedef C ClassType;
    typedef typename Traits::ExposedType ExposedType;
    typedef ReferenceTraits<ExposedType> RefTraits;
    typedef typename RefTraits::DereferencedType AccessType;
    typedef typename Traits::DataType DataType; // raw type
    static constexpr bool canRead = true;
    static constexpr bool canWrite = true;
    
    template <typename F1, typename F2>
    Accessor2(F1 getter, F2 setter)
        : m_getter(getter)
        , m_setter(setter)
    {
    }

    ExposedType get(const ClassType& object) const
    {
        return m_getter(object);
    }
    
    bool set(ClassType& object, const DataType& value) const
    {
        return m_setter(object, value), true;
    }

private:

    std::function<typename Traits::DispatchType> m_getter;
    std::function<void(ClassType&, DataType)> m_setter;
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
        // what exposes the value
        typedef typename PropertyTraitMapper<T>::Traits PropertyTraits;
        
        // unify how we retrieve the exposed type
        typedef Accessor1<C, PropertyTraits> AccessorType;
        
        // which interface do we need to deal with the type?
        typedef AccessTraits<typename AccessorType::AccessType> ExposedType;
        
        // the property wrapper that provides the correct interface for the type
        typedef typename ExposedType::template Impl<AccessorType>::Type PropertyImplType;
        
        // instance the interface for our type
        return new PropertyImplType(name, AccessorType(accessor));
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
        // function that will expose the type (setter will mirror)
        typedef FunctionTraits<F1> Traits;
        
        // unify how we retrieve the exposed type
        typedef Accessor2<C, Traits> AccessorType;
        
        // which interface do we need to deal with the type?
        typedef AccessTraits<typename AccessorType::AccessType> ExposedType;
        
        // the property wrapper that provides the correct interface for the type
        typedef typename ExposedType::template Impl<AccessorType>::Type PropertyImplType;
        
        // instance the interface for our type
        return new PropertyImplType(name, AccessorType(accessor1, accessor2));
    }
};

} // namespace detail
} // namespace ponder

#endif // PONDER_DETAIL_PROPERTYFACTORY_HPP
