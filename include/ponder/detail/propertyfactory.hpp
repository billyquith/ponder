/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2020 Nick Trout.
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

// Bind to value.
template <class C, typename PropTraits>
class ValueBinder
{
public:
    typedef C ClassType;
    typedef typename std::conditional<PropTraits::isWritable,
                typename PropTraits::AccessType&, typename PropTraits::AccessType>::type AccessType;
    typedef typename std::remove_reference<AccessType>::type SetType;

    using Binding = typename PropTraits::template Binding<ClassType, AccessType>;

    static_assert(!std::is_pointer<AccessType>::value, "Error: Pointers not handled here");

    ValueBinder(const Binding& b) : m_bound(b) {}

    AccessType getter(ClassType& c) const { return m_bound.access(c); }

    Value getValue(ClassType& c) const {
        if constexpr (PropTraits::isWritable)
            return UserObject::makeRef(getter(c));
        else
            return UserObject::makeCopy(getter(c));
    }

    bool setter(ClassType& c, SetType v) const {
        if constexpr (PropTraits::isWritable)
            return this->m_bound.access(c) = v, true;
        else
            return false;
    }

    bool setter(ClassType& c, Value const& value) const {
        return setter(c, value.to<SetType>());
    }

protected:
    Binding m_bound;
};

template <class C, typename PropTraits>
class ValueBinder2 : public ValueBinder<C, PropTraits>
{
    typedef ValueBinder<C, PropTraits> Base;
public:
    template <typename S>
    ValueBinder2(const Base::Binding& g, S s) : Base(g), m_set(s) {}

    bool setter(ClassType& c, SetType v) const {
        return m_set(c, v), true;
    }

    bool setter(ClassType& c, Value const& value) const {
        return setter(c, value.to<SetType>());
    }

protected:
    std::function<void(Base::ClassType&, Base::AccessType)> m_set;
};


// Bind to internal reference getter.
template <class C, typename PropTraits>
class InternalRefBinder
{
public:        
    typedef C ClassType;
    typedef typename PropTraits::ExposedType AccessType;

    using Binding = typename PropTraits::template Binding<ClassType, AccessType>;

    static_assert(std::is_pointer<AccessType>::value, "Error: Only pointers handled here");

    InternalRefBinder(const Binding& b) : m_bound(b) {}

    AccessType getter(ClassType& c) const {
        if constexpr (std::is_const<AccessType>::value)
            return m_bound.access(c);
        else
            return m_bound.access(const_cast<typename std::remove_const<ClassType>::type&>(c));
    }

    Value getValue(ClassType& c) const {return UserObject::makeRef(getter(c));}

    bool setter(ClassType&, AccessType) const {return false;}
    bool setter(ClassType&, Value const&) const {return false;}

protected:
    Binding m_bound;
};

// Internal reference getter & setter.
template <class C, typename PropTraits>
class InternalRefBinder2 : public InternalRefBinder<C, PropTraits>
{
    typedef InternalRefBinder<C, PropTraits> Base;
public:
    template <typename S>
    InternalRefBinder2(const Base::Binding& g, S s) : Base(g), m_set(s) {}

    bool setter(ClassType& c, AccessType v) const { return m_set(c, v), true; }
    bool setter(ClassType& c, Value const& value) const { return setter(c, value.to<AccessType>()); }

protected:
    std::function<void(ClassType&, AccessType)> m_set;
};

/*
 *  Access traits for an exposed type T.
 *    - I.e. how we use an instance to access the bound property data using the correct interface.
 *  Traits:
 *    - ValueBinder & RefBinder : RO (const) or RW data.
 *    - Impl : which specialise property impl to use.
 */
template <typename PT, typename E = void>
struct AccessTraits
{
    static constexpr PropertyAccessKind kind = PropertyAccessKind::Simple;

    template <class C>
    using ValueBinder = ValueBinder<C, PT>;

    template <class C>
    using ValueBinder2 = ValueBinder2<C, PT>;

    template <typename A>
    using Impl = SimplePropertyImpl<A>;
};

/*
 * Enums.
 */
template <typename PT>
struct AccessTraits<PT,
    typename std::enable_if<std::is_enum<typename PT::ExposedTraits::DereferencedType>::value>::type>
{
    static constexpr PropertyAccessKind kind = PropertyAccessKind::Enum;

    template <class C>
    using ValueBinder = ValueBinder<C, PT>;

    template <class C>
    using ValueBinder2 = ValueBinder2<C, PT>;

    template <typename A>
    using Impl = EnumPropertyImpl<A>;
};

/*
 * Array types.
 */
template <typename PT>
struct AccessTraits<PT,
    typename std::enable_if<ponder_ext::ArrayMapper<typename PT::ExposedTraits::DereferencedType>::isArray>::type>
{
    static constexpr PropertyAccessKind kind = PropertyAccessKind::Container;

    typedef ponder_ext::ArrayMapper<typename PT::ExposedTraits::DereferencedType> ArrayTraits;

    template <class C>
    class ValueBinder : public ArrayTraits
    {
    public:
        typedef typename PT::ExposedTraits::DereferencedType ArrayType;
        typedef C ClassType;
        typedef typename PT::AccessType& AccessType;

        using Binding = typename PT::template Binding<ClassType, AccessType>;

        ValueBinder(const Binding& a) : m_bound(a) {}
        
        AccessType getter(ClassType& c) const {return m_bound.access(c);}

        bool setter(ClassType& c, AccessType v) const {
            return this->m_bound.access(c) = v, true;
        }
    protected:
        Binding m_bound;
    };

    template <typename A>
    using Impl = ArrayPropertyImpl<A>;
};

/*
 * User objects.
 *  - I.e. Registered classes.
 *  - Enums also use registration so must differentiate.
 */
template <typename PT>
struct AccessTraits<PT,
    typename std::enable_if<hasStaticTypeDecl<typename PT::ExposedTraits::DereferencedType>()
                        && !std::is_enum<typename PT::ExposedTraits::DereferencedType>::value>::type>
{
    static constexpr PropertyAccessKind kind = PropertyAccessKind::User;

    template <class C>
    using ValueBinder = typename std::conditional<
        std::is_pointer<typename PT::ExposedType>::value, InternalRefBinder<C, PT>, ValueBinder<C, PT>
    >::type;

    template <class C>
    using ValueBinder2 = typename std::conditional<
        std::is_pointer<typename PT::ExposedType>::value, InternalRefBinder2<C, PT>, ValueBinder2<C, PT>
    >::type;

    template <typename A>
    using Impl = UserPropertyImpl<A>;
};


// Read-only accessor wrapper. Not RW, not a pointer.
template <class C, typename TRAITS>
class GetSet1
{
public:
    typedef TRAITS PropTraits;
    typedef C ClassType;
    typedef typename PropTraits::ExposedType ExposedType;
    typedef typename PropTraits::ExposedTraits TypeTraits;
    typedef typename PropTraits::DataType DataType; // raw type or container
    static constexpr bool canRead = true;
    static constexpr bool canWrite = PropTraits::isWritable;

    typedef AccessTraits<PropTraits> Access;

    typedef typename Access::template ValueBinder<ClassType> InterfaceType;

    InterfaceType m_interface;

    GetSet1(typename PropTraits::BoundType getter)
        : m_interface(typename InterfaceType::Binding(getter))
    {}
};

/*
 * Property accessor composed of 1 getter and 1 setter
 */
template <typename C, typename FUNCTRAITS>
class GetSet2
{
public:

    typedef FUNCTRAITS PropTraits;
    typedef C ClassType;
    typedef typename PropTraits::ExposedType ExposedType;
    typedef typename PropTraits::ExposedTraits TypeTraits;
    typedef typename PropTraits::DataType DataType; // raw type
    static constexpr bool canRead = true;
    static constexpr bool canWrite = true;
    
    typedef AccessTraits<PropTraits> Access;

    typedef typename Access::template ValueBinder2<ClassType> InterfaceType;

    InterfaceType m_interface;

    template <typename F1, typename F2>
    GetSet2(F1 getter, F2 setter)
        : m_interface(typename InterfaceType::Binding(getter), setter)
    {}
};

/*
 * Property factory which instantiates the proper type of property from 1 accessor.
 */
template <typename C, typename T, typename E = void>
struct PropertyFactory1
{
    static constexpr PropertyKind kind = PropertyKind::Function;

    static Property* create(IdRef name, T accessor)
    {
        typedef GetSet1<C, FunctionTraits<T>> Accessor; // read-only?
        
        typedef typename Accessor::Access::template Impl<Accessor> PropertyImpl;
        
        return new PropertyImpl(name, Accessor(accessor));
    }
};

template <typename C, typename T>
struct PropertyFactory1<C, T, typename std::enable_if<std::is_member_object_pointer<T>::value>::type>
{
    static constexpr PropertyKind kind = PropertyKind::MemberObject;

    static Property* create(IdRef name, T accessor)
    {
        typedef GetSet1<C, MemberTraits<T>> Accessor; // read-only?

        typedef typename Accessor::Access::template Impl<Accessor> PropertyImpl;

        return new PropertyImpl(name, Accessor(accessor));
    }
};

/*
 * Expose property with a getter and setter function.
 * Type of property is the return type of the getter.
 */
template <typename C, typename F1, typename F2, typename E = void>
struct PropertyFactory2
{
    static Property* create(IdRef name, F1 accessor1, F2 accessor2)
    {
        typedef GetSet2<C, FunctionTraits<F1>> Accessor; // read-write wrapper
        
        typedef typename Accessor::Access::template Impl<Accessor> PropertyImpl;

        return new PropertyImpl(name, Accessor(accessor1, accessor2));
    }
};

} // namespace detail
} // namespace ponder

#endif // PONDER_DETAIL_PROPERTYFACTORY_HPP
