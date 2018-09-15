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
    
    template <class TA>
    class ReadOnlyInterface
    {
    public:
        typedef TA TypeAccess;
        typedef typename TypeAccess::ClassType ClassType;
        typedef typename RawType<typename TypeAccess::AccessType>::Type DataType;

        ReadOnlyInterface(const TypeAccess& a) : m_access(a) {}
        
        typename TypeAccess::AccessType getter(ClassType& c) const {return m_access.access(c);}
        
        bool setter(ClassType&, const DataType&) const {return false;}
        bool setter(ClassType&, DataType&&) const {return false;}
    protected:
        TypeAccess m_access;
    };

    template <class TA>
    class WritableInterface : public ReadOnlyInterface<TA>
    {
    public:
        typedef ReadOnlyInterface<TA> Base;
        
        WritableInterface(const typename Base::TypeAccess& a) : ReadOnlyInterface<TA>(a) {}
        
        bool setter(typename Base::ClassType& c, const typename Base::DataType& v) const
        {return this->m_access.access(c) = v, true;}
        bool setter(typename Base::ClassType& c, typename Base::DataType&& v) const
        {return this->m_access.access(c) = std::move(v), true;}
    };

    template <typename A>
    using Impl = SimplePropertyImpl<A>;
};

/*
 * Enums.
 */
template <typename T>
struct AccessTraits<T, typename std::enable_if<std::is_enum<T>::value>::type>
{
    static constexpr PropertyAccessKind kind = PropertyAccessKind::Enum;

    template <class TA>
    class ReadOnlyInterface
    {
    public:
        typedef TA TypeAccess;
        typedef typename TypeAccess::ClassType ClassType;
        typedef typename RawType<typename TypeAccess::AccessType>::Type DataType;
        
        ReadOnlyInterface(const TypeAccess& a) : m_access(a) {}
        
        T getter(ClassType& c) const {return m_access.access(c);}
        
        bool setter(ClassType&, const DataType&) const {return false;}
        bool setter(ClassType&, DataType&&) const {return false;}
    protected:
        TypeAccess m_access;
    };
    
    template <class TA>
    class WritableInterface : public ReadOnlyInterface<TA>
    {
    public:
        typedef ReadOnlyInterface<TA> Base;
        
        WritableInterface(const typename Base::TypeAccess& a) : ReadOnlyInterface<TA>(a) {}
        
        bool setter(typename Base::ClassType& c, const typename Base::DataType& v) const
        {return this->m_access.access(c) = v, true;}
        bool setter(typename Base::ClassType& c, typename Base::DataType&& v) const
        {return this->m_access.access(c) = std::move(v), true;}
    };

    template <typename A>
    using Impl = EnumPropertyImpl<A>;
};

/*
 * Array types.
 */
template <typename T>
struct AccessTraits<T, typename std::enable_if<ponder_ext::ArrayMapper<T>::isArray>::type>
{
    static constexpr PropertyAccessKind kind = PropertyAccessKind::Container;
    typedef ponder_ext::ArrayMapper<T> ArrayTraits;

    template <typename TA>
    class ReadOnlyInterface : public ArrayTraits
    {
    public:
        typedef TA TypeAccess;
        typedef T ArrayType;
        typedef typename TypeAccess::ClassType ClassType;
        typedef typename ArrayTraits::ElementType DataType;

        ReadOnlyInterface(const TypeAccess& a) : m_access(a) {}
        
        ArrayType& array(ClassType& c) const {return m_access.access(c);}
        const ArrayType& array(const ClassType& c) const {return m_access.access(c);}
    protected:
        TypeAccess m_access;
    };
    
    template <class TA>
    class WritableInterface : public ReadOnlyInterface<TA>
    {
    public:
        typedef ReadOnlyInterface<TA> Base;
        
        WritableInterface(const typename Base::TypeAccess& a) : ReadOnlyInterface<TA>(a) {}
    };
    
    template <typename A>
    using Impl = ArrayPropertyImpl<A>;
};

/*
 * User objects.
 *  - I.e. Registered classes.
 *  - Enums also use registration so must differentiate.
 */
template <typename T>
struct AccessTraits<T,
    typename std::enable_if<StaticTypeId<T>::defined && !std::is_enum<T>::value>::type>
{
    static constexpr PropertyAccessKind kind = PropertyAccessKind::User;

    template <class TA>
    class ReadOnlyInterface
    {
    public:
        typedef TA TypeAccess;
        typedef typename TypeAccess::ClassType ClassType;
        typedef typename RawType<typename TypeAccess::AccessType>::Type DataType;
        
        ReadOnlyInterface(const TypeAccess& a) : m_access(a) {}
        
        T& getter(ClassType& c) const {return m_access.access(c);}
        const T& getter(const ClassType& c) const {return m_access.access(c);}
        
        bool setter(ClassType&, const DataType&) const {return false;}
        bool setter(ClassType&, DataType&&) const {return false;}
    protected:
        TypeAccess m_access;
    };
    
    template <class TA>
    class WritableInterface : public ReadOnlyInterface<TA>
    {
    public:
        typedef ReadOnlyInterface<TA> Base;
        
        WritableInterface(const typename Base::TypeAccess& a) : ReadOnlyInterface<TA>(a) {}
        
        bool setter(typename Base::ClassType& c, const typename Base::DataType& v) const
        {return this->m_access.access(c) = v, true;}
        bool setter(typename Base::ClassType& c, typename Base::DataType&& v) const
        {return this->m_access.access(c) = std::move(v), true;}
    };

    template <typename A>
    using Impl = UserPropertyImpl<A>;
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
template <class C, typename PT, typename E = void>
class Accessor1
{
public:

    typedef PT PropertyTraits;
    static_assert(!PropertyTraits::isWritable, "!isWritable expected");
    typedef const C ClassType;
    typedef typename PropertyTraits::ExposedType ExposedType;
    typedef typename PropertyTraits::RefTraits RefTraits;
    typedef typename PropertyTraits::DataType DataType; // raw type or container
    static constexpr bool canRead = true;
    static constexpr bool canWrite = false;

    typedef AccessTraits<typename RefTraits::DereferencedType> PropAccessTraits;
    typedef typename PropAccessTraits::template
        ReadOnlyInterface<typename PropertyTraits::template TypeAccess<ClassType,
                          typename PropertyTraits::AccessType>> InterfaceType;
    InterfaceType m_interface;

    Accessor1(typename PropertyTraits::Type getter)
        : m_interface(typename PropertyTraits::template TypeAccess<ClassType,
                      typename PropertyTraits::AccessType>(getter))
    {}
};

/*
 * Read-write accessor wrapper.
 */
template <class C, typename TRAITS>
class Accessor1<C, TRAITS, typename std::enable_if<TRAITS::isWritable>::type>
{
public:

    typedef TRAITS PropertyTraits;
    static_assert(PropertyTraits::isWritable, "isWritable expected");
    typedef C ClassType;
    typedef typename PropertyTraits::ExposedType ExposedType;
    typedef ReferenceTraits<ExposedType> RefTraits;
    typedef typename PropertyTraits::DataType DataType; // raw type or container
    static constexpr bool canRead = true;
    static constexpr bool canWrite = true;
    
    typedef AccessTraits<typename RefTraits::DereferencedType> PropAccessTraits;
    typedef typename PropAccessTraits::template
        WritableInterface<typename PropertyTraits::template TypeAccess<ClassType,
                          typename PropertyTraits::AccessType>> InterfaceType;
    InterfaceType m_interface;

    Accessor1(typename PropertyTraits::Type getter)
        : m_interface(typename PropertyTraits::template TypeAccess<ClassType,
                      typename PropertyTraits::AccessType>(getter))
    {}
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
    typedef typename Traits::DataType DataType; // raw type
    static constexpr bool canRead = true;
    static constexpr bool canWrite = true;
    
    typedef AccessTraits<typename RefTraits::DereferencedType> PropAccessTraits;

    struct InterfaceType
    {
        template <typename F1, typename F2>
        InterfaceType(F1 get, F2 set) : getter(get), m_setter(set) {}
        
        std::function<typename Traits::DispatchType> getter;
        std::function<void(ClassType&, DataType)> m_setter;
        // setter returns writable status, so wrap it
        bool setter(ClassType& c, DataType v) const {return m_setter(c,v), true;}
    } m_interface;

    template <typename F1, typename F2>
    Accessor2(F1 getter, F2 setter)
        : m_interface(getter, setter)
    {}
};
    
/*
 * Property factory which instantiates the proper type of property from 1 accessor.
 * The accessor can be a member object or a function.
 */
template <typename C, typename T>
struct PropertyFactory1
{
    static Property* create(IdRef name, T accessor)
    {
        // what exposes the value
        typedef typename PropertyTraitMapper<T>::Traits PropertyTraits;

        // what type of access, read-only?
        typedef Accessor1<C, PropertyTraits> AccessorType;
        
        // the property wrapper that provides the correct interface for the type
        typedef typename AccessorType::PropAccessTraits::template Impl<AccessorType> PropertyImplType;
        
        // instance the interface for our type
        return new PropertyImplType(name, AccessorType(accessor));
    }
};

/*
 * Property factory which instantiates the proper type of property from 2 accessors.
 * Both getter and setter should be functions.
 */
template <typename C, typename F1, typename F2, typename E = void>
struct PropertyFactory2
{
    static Property* create(IdRef name, F1 accessor1, F2 accessor2)
    {
        // function that will expose the type (setter will mirror)
        typedef FunctionTraits<F1> Traits;
        
        // unify how we retrieve the exposed type
        typedef Accessor2<C, Traits> AccessorType;
        
        // the property wrapper that provides the correct interface for the type
        typedef typename AccessorType::PropAccessTraits::template Impl<AccessorType> PropertyImplType;

        // instance the interface for our type
        return new PropertyImplType(name, AccessorType(accessor1, accessor2));
    }
};

} // namespace detail
} // namespace ponder

#endif // PONDER_DETAIL_PROPERTYFACTORY_HPP
