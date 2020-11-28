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
    
/*
 *  Access traits for an exposed type T.
 *    - I.e. how we use an instance to access the bound property data using the correct interface.
 *  Traits:
 *    - ReadOnlyInterface & ReadWriteInterface : RO (const) or RW data.
 *    - Impl : which specialise property impl to use.
 */
template <typename T, typename E = void>
struct AccessTraits
{
    static constexpr PropertyAccessKind kind = PropertyAccessKind::Simple;
    
    template <class B>
    class ReadOnlyInterface
    {
    public:
        typedef B Binding;
        typedef typename Binding::ClassType ClassType;
        typedef typename DataType<typename Binding::AccessType>::Type DataType;

        ReadOnlyInterface(const Binding& a) : m_bound(a) {}
        
        typename Binding::AccessType getter(ClassType& c) const {return m_bound.access(c);}
        
        bool setter(ClassType&, const DataType&) const {return false;}
        bool setter(ClassType&, DataType&&) const {return false;}
    protected:
        Binding m_bound;
    };

    template <class B>
    class ReadWriteInterface : public ReadOnlyInterface<B>
    {
    public:
        typedef ReadOnlyInterface<B> Base;
        
        ReadWriteInterface(const typename Base::Binding& a) : ReadOnlyInterface<B>(a) {}
        
        bool setter(typename Base::ClassType& c, const typename Base::DataType& v) const {
            return this->m_bound.access(c) = v, true;
        }
        bool setter(typename Base::ClassType& c, typename Base::DataType&& v) const {
            return this->m_bound.access(c) = std::move(v), true;
        }
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

    template <class B>
    class ReadOnlyInterface
    {
    public:
        typedef B Binding;
        typedef typename Binding::ClassType ClassType;
        typedef typename DataType<typename Binding::AccessType>::Type DataType;
        
        ReadOnlyInterface(const Binding& a) : m_bound(a) {}
        
        T getter(ClassType& c) const {return m_bound.access(c);}
        
        bool setter(ClassType&, const DataType&) const {return false;}
        bool setter(ClassType&, DataType&&) const {return false;}
    protected:
        Binding m_bound;
    };
    
    template <class B>
    class ReadWriteInterface : public ReadOnlyInterface<B>
    {
    public:
        typedef ReadOnlyInterface<B> Base;
        
        ReadWriteInterface(const typename Base::Binding& a) : ReadOnlyInterface<B>(a) {}
        
        bool setter(typename Base::ClassType& c, const typename Base::DataType& v) const {
            return this->m_bound.access(c) = v, true;
        }
        bool setter(typename Base::ClassType& c, typename Base::DataType&& v) const {
            return this->m_bound.access(c) = std::move(v), true;
        }
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

    template <typename B>
    class ReadOnlyInterface : public ArrayTraits
    {
    public:
        typedef B Binding;
        typedef T ArrayType;
        typedef typename Binding::ClassType ClassType;
        typedef typename ArrayTraits::ElementType DataType;

        ReadOnlyInterface(const Binding& a) : m_bound(a) {}
        
        ArrayType& array(ClassType& c) const {return m_bound.access(c);}
        const ArrayType& array(const ClassType& c) const {return m_bound.access(c);}
    protected:
        Binding m_bound;
    };
    
    template <class B>
    class ReadWriteInterface : public ReadOnlyInterface<B>
    {
    public:
        typedef ReadOnlyInterface<B> Base;
        
        ReadWriteInterface(const typename Base::Binding& a) : ReadOnlyInterface<B>(a) {}
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
    typename std::enable_if<hasStaticTypeDecl<T>() && !std::is_enum<T>::value>::type>
{
    static constexpr PropertyAccessKind kind = PropertyAccessKind::User;

    template <class B>
    class ReadOnlyInterface
    {
    public:
        typedef B Binding;
        typedef typename Binding::ClassType ClassType;
        typedef typename DataType<typename Binding::AccessType>::Type DataType;
        
        ReadOnlyInterface(const Binding& a) : m_bound(a) {}
        
        T& getter(ClassType& c) const {return m_bound.access(c);}
        const T& getter(const ClassType& c) const {return m_bound.access(c);}
        
        bool setter(ClassType&, const DataType&) const {return false;}
        bool setter(ClassType&, DataType&&) const {return false;}
    protected:
        Binding m_bound;
    };
    
    template <class B>
    class ReadWriteInterface : public ReadOnlyInterface<B>
    {
    public:
        typedef ReadOnlyInterface<B> Base;
        
        ReadWriteInterface(const typename Base::Binding& a) : ReadOnlyInterface<B>(a) {}
        
        bool setter(typename Base::ClassType& c, const typename Base::DataType& v) const {
            return this->m_bound.access(c) = v, true;
        }
        bool setter(typename Base::ClassType& c, typename Base::DataType&& v) const {
            return this->m_bound.access(c) = std::move(v), true;
        }
    };

    template <typename A>
    using Impl = UserPropertyImpl<A>;
};

// Sanity checks.
template <typename T>
struct AccessTraits<T&> { typedef int TypeShouldBeDereferenced[-(int)sizeof(T)]; };
template <typename T>
struct AccessTraits<T*> { typedef int TypeShouldBeDereferenced[-(int)sizeof(T)]; };    

// Read-only accessor wrapper. Not RW, not a pointer.
template <class C, typename TRAITS, typename E = void>
class GetSet1
{
public:
    typedef TRAITS PropTraits;
    static_assert(!PropTraits::isWritable, "!isWritable expected");
    typedef const C ClassType;
    typedef typename PropTraits::ExposedType ExposedType;
    typedef typename PropTraits::TypeTraits TypeTraits;
    typedef typename PropTraits::DataType DataType; // raw type or container
    static constexpr bool canRead = true;
    static constexpr bool canWrite = false;

    static_assert(TypeTraits::kind != ReferenceKind::Pointer, "Ponder: Cannot return pointer");

    typedef AccessTraits<typename TypeTraits::DereferencedType> Interface;

    typedef typename Interface::template
        ReadOnlyInterface<typename PropTraits::template Binding<ClassType, typename PropTraits::AccessType>>
            InterfaceType;

    InterfaceType m_interface;

    GetSet1(typename PropTraits::BoundType getter)
        : m_interface(typename PropTraits::template Binding<ClassType,
                      typename PropTraits::AccessType>(getter))
    {}
};

// Read-write accessor wrapper.
template <class C, typename TRAITS>
class GetSet1<C, TRAITS, typename std::enable_if<TRAITS::isWritable>::type>
{
public:
    typedef TRAITS PropTraits;
    static_assert(PropTraits::isWritable, "isWritable expected");
    typedef C ClassType;
    typedef typename PropTraits::ExposedType ExposedType;
    typedef typename PropTraits::TypeTraits TypeTraits;
    typedef typename PropTraits::DataType DataType; // raw type or container
    static constexpr bool canRead = true;
    static constexpr bool canWrite = true;
    
    typedef AccessTraits<typename TypeTraits::DereferencedType> Interface; // property interface specialisation

    typedef typename Interface::template
        ReadWriteInterface<typename PropTraits::template Binding<ClassType, typename PropTraits::AccessType>>
            InterfaceType;

    InterfaceType m_interface;

    GetSet1(typename PropTraits::BoundType getter)
        : m_interface(typename PropTraits::template Binding<ClassType, typename PropTraits::AccessType>(getter))
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
    typedef typename PropTraits::TypeTraits TypeTraits;
    typedef typename PropTraits::DataType DataType; // raw type
    static constexpr bool canRead = true;
    static constexpr bool canWrite = true;
    
    typedef AccessTraits<typename TypeTraits::DereferencedType> Interface;

    struct InterfaceType
    {
        template <typename F1, typename F2>
        InterfaceType(F1 get, F2 set) : getter(get), m_setter(set) {}
        
        std::function<typename PropTraits::DispatchType> getter;
        std::function<void(ClassType&, DataType)> m_setter;

        // setter returns writable status, so wrap it
        bool setter(ClassType& c, DataType v) const {return m_setter(c,v), true;}
    } m_interface;

    template <typename F1, typename F2>
    GetSet2(F1 getter, F2 setter)
        : m_interface(getter, setter)
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
        
        typedef typename Accessor::Interface::template Impl<Accessor> PropertyImplType;
        
        return new PropertyImplType(name, Accessor(accessor));
    }
};

template <typename C, typename T>
struct PropertyFactory1<C, T, typename std::enable_if<std::is_member_object_pointer<T>::value>::type>
{
    static constexpr PropertyKind kind = PropertyKind::MemberObject;

    static Property* create(IdRef name, T accessor)
    {
        typedef GetSet1<C, MemberTraits<T>> Accessor; // read-only?

        typedef typename Accessor::Interface::template Impl<Accessor> PropertyImplType;

        return new PropertyImplType(name, Accessor(accessor));
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
        
        typedef typename Accessor::Interface::template Impl<Accessor> PropertyImplType;

        return new PropertyImplType(name, Accessor(accessor1, accessor2));
    }
};

} // namespace detail
} // namespace ponder

#endif // PONDER_DETAIL_PROPERTYFACTORY_HPP
