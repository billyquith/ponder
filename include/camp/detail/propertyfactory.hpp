/****************************************************************************
**
** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** CAMP is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** 
** CAMP is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public License
** along with CAMP.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/


#ifndef CAMP_DETAIL_PROPERTYFACTORY_HPP
#define CAMP_DETAIL_PROPERTYFACTORY_HPP


#include <camp/type.hpp>
#include <camp/detail/simplepropertyimpl.hpp>
#include <camp/detail/arraypropertyimpl.hpp>
#include <camp/detail/enumpropertyimpl.hpp>
#include <camp/detail/userpropertyimpl.hpp>
#include <camp/detail/functiontraits.hpp>
#include <boost/function.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/is_void.hpp>


namespace camp
{
namespace detail
{
/*
 * Instanciate simple properties
 */
template <typename A, int T>
struct PropertyMapper
{
    typedef SimplePropertyImpl<A> Type;
};

/*
 * Instanciate array properties
 */
template <typename A>
struct PropertyMapper<A, camp::arrayType>
{
    typedef ArrayPropertyImpl<A> Type;
};

/*
 * Instanciate enum properties
 */
template <typename A>
struct PropertyMapper<A, camp::enumType>
{
    typedef EnumPropertyImpl<A> Type;
};

/*
 * Instanciate user properties
 */
template <typename A>
struct PropertyMapper<A, camp::userType>
{
    typedef UserPropertyImpl<A> Type;
};

/**
 * Helper structure to perform copy and assignment
 *
 * The purpose of this structure is to avoid a compiler error when the copied
 * type is not copyable. Instead, we just return an error so that the caller
 * can throw a CAMP exception.
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

/**
 * Specialization of CopyHelper for non-copyable types
 */
template <typename T>
struct CopyHelper<T, typename boost::enable_if_c<!StaticTypeId<T>::copyable>::type>
{
    static bool copy(T&, const Value&)
    {
        // We don't really return an error, we just skip the assignment
        return true;
    }
};

/**
 * Helper structure to return values
 *
 * The purpose of this structure is to provide workarounds for types
 * that don't exactly satisfy the compiler when returned. For example,
 * it converts smart pointer types to the corresponding raw pointer types.
 */
template <typename T, typename E = void>
struct ReturnHelper
{
    typedef T Type;
    static Type get(T value) {return value;}
};

/**
 * Specialization of ReturnHelper for smart pointer types
 */
template <template <typename> class T, typename U>
struct ReturnHelper<T<U>, typename boost::enable_if<IsSmartPointer<T<U>, U> >::type>
{
    typedef U* Type;
    static Type get(T<U> value) {return boost::get_pointer(value);}
};

/**
 * Specialization of ReturnHelper for built-in array types
 */
template <typename T, int N>
struct ReturnHelper<T[N]>
{
    typedef T (&Type)[N];
    static Type get(T (&value)[N]) {return value;}
};

/*
 * Property accessor composed of 1 getter
 */
template <typename C, typename R, typename E = void>
class Accessor1
{
public:

    typedef ObjectTraits<R> Traits;
    typedef typename Traits::DataType DataType;
    typedef C ClassType;

    enum
    {
        canRead = true,
        canWrite = false
    };

    template <typename F>
    Accessor1(F getter)
        : m_getter(getter)
    {
    }

    typename ReturnHelper<R>::Type get(C& object) const
    {
        return ReturnHelper<R>::get(m_getter(object));
    }

    bool set(C&, const Value&) const
    {
        // Not available
        return false;
    }

private:

    boost::function<R (C&)> m_getter;
};

/*
 * Property accessor composed of 1 read-write accessor
 */
template <typename C, typename R>
class Accessor1<C, R, typename boost::enable_if_c<ObjectTraits<R>::isWritable>::type>
{
public:

    typedef ObjectTraits<R> Traits;
    typedef typename Traits::DataType DataType;
    typedef C ClassType;

    enum
    {
        canRead = true,
        canWrite = true
    };

    template <typename F>
    Accessor1(F getter)
        : m_getter(getter)
    {
    }

    typename ReturnHelper<R>::Type get(C& object) const
    {
        return ReturnHelper<R>::get(m_getter(object));
    }

    bool set(C& object, const Value& value) const
    {
        return CopyHelper<DataType>::copy(*Traits::getPointer(m_getter(object)), value);
    }

private:

    boost::function<R (C&)> m_getter;
};

/*
 * Property accessor composed of 1 getter and 1 setter
 */
template <typename C, typename R>
class Accessor2
{
public:

    typedef ObjectTraits<R> Traits;
    typedef typename Traits::DataType DataType;
    typedef C ClassType;
    typedef typename boost::remove_reference<R>::type ArgumentType;

    enum
    {
        canRead = true,
        canWrite = true
    };

    template <typename F1, typename F2>
    Accessor2(F1 getter, F2 setter)
        : m_getter(getter)
        , m_setter(setter)
    {
    }

    typename ReturnHelper<R>::Type get(C& object) const
    {
        return ReturnHelper<R>::get(m_getter(object));
    }

    bool set(C& object, const Value& value) const
    {
        m_setter(object, value.to<ArgumentType>());
        return true;
    }

private:

    boost::function<R (C&)> m_getter;
    boost::function<void (C&, ArgumentType)> m_setter;
};

/*
 * Property accessor composed of 1 composed getter
 */
template <typename C, typename N, typename R, typename E = void>
class Accessor3
{
public:

    typedef ObjectTraits<R> Traits;
    typedef typename Traits::DataType DataType;
    typedef C ClassType;

    enum
    {
        canRead = true,
        canWrite = false
    };

    template <typename F1, typename F2>
    Accessor3(F1 getter1, F2 getter2)
        : m_getter1(getter1)
        , m_getter2(getter2)
    {
    }

    typename ReturnHelper<R>::Type get(C& object) const
    {
        return ReturnHelper<R>::get(m_getter1(m_getter2(object)));
    }

    bool set(C&, const Value&) const
    {
        // Not available
        return false;
    }

private:

    boost::function<R (N&)> m_getter1;
    boost::function<N& (C&)> m_getter2;
};

/*
 * Property accessor composed of 1 composed read-write accessor
 */
template <typename C, typename N, typename R>
class Accessor3<C, N, R, typename boost::enable_if_c<ObjectTraits<R>::isWritable>::type>
{
public:

    typedef ObjectTraits<R> Traits;
    typedef typename Traits::DataType DataType;
    typedef C ClassType;

    enum
    {
        canRead = true,
        canWrite = true
    };

    template <typename F1, typename F2>
    Accessor3(F1 getter1, F2 getter2)
        : m_getter1(getter1)
        , m_getter2(getter2)
    {
    }

    typename ReturnHelper<R>::Type get(C& object) const
    {
        return ReturnHelper<R>::get(m_getter1(m_getter2(object)));
    }

    bool set(C& object, const Value& value) const
    {
        return CopyHelper<DataType>::copy(*Traits::getPointer(m_getter1(m_getter2(object))), value);
    }

private:

    boost::function<R (N&)> m_getter1;
    boost::function<N& (C&)> m_getter2;
};


/*
 * Property factory which instanciates the proper type of property from 1 accessor
 */
template <typename C, typename F>
struct PropertyFactory1
{
    typedef typename FunctionTraits<F>::ReturnType ReturnType;

    static Property* get(const std::string& name, F accessor)
    {
        typedef Accessor1<C, ReturnType> AccessorType;

        typedef camp_ext::ValueMapper<typename AccessorType::DataType> ValueMapper;
        typedef typename PropertyMapper<AccessorType, ValueMapper::type>::Type PropertyType;

        return new PropertyType(name, AccessorType(accessor));
    }
};

/*
 * Property factory which instanciates the proper type of property from 2 accessors
 */
template <typename C, typename F1, typename F2, typename E = void>
struct PropertyFactory2
{
    typedef typename FunctionTraits<F1>::ReturnType ReturnType;

    static Property* get(const std::string& name, F1 accessor1, F2 accessor2)
    {
        typedef Accessor2<C, ReturnType> AccessorType;

        typedef camp_ext::ValueMapper<typename AccessorType::DataType> ValueMapper;
        typedef typename PropertyMapper<AccessorType, ValueMapper::type>::Type PropertyType;

        return new PropertyType(name, AccessorType(accessor1, accessor2));
    }
};

/*
 * Specialization of PropertyFactory2 with 2 getters (which will produce 1 composed getter)
 */
template <typename C, typename F1, typename F2>
struct PropertyFactory2<C, F1, F2, typename boost::enable_if_c<!boost::is_void<typename FunctionTraits<F2>::ReturnType>::value>::type>
{
    typedef typename FunctionTraits<F1>::ReturnType ReturnType;
    typedef typename boost::remove_reference<typename FunctionTraits<F2>::ReturnType>::type OtherClassType;

    static Property* get(const std::string& name, F1 accessor1, F2 accessor2)
    {
        typedef Accessor3<C, OtherClassType, ReturnType> AccessorType;

        typedef camp_ext::ValueMapper<typename AccessorType::DataType> ValueMapper;
        typedef typename PropertyMapper<AccessorType, ValueMapper::type>::Type PropertyType;

        return new PropertyType(name, AccessorType(accessor1, accessor2));
    }
};

/*
 * Property factory which instanciates the proper type of property from 3 accessors
 */
template <typename C, typename F1, typename F2, typename F3>
struct PropertyFactory3
{
    typedef typename FunctionTraits<F1>::ReturnType ReturnType;

    static Property* get(const std::string& name, F1 accessor1, F2 accessor2, F3 accessor3)
    {
        typedef Accessor2<C, ReturnType> AccessorType;

        typedef camp_ext::ValueMapper<typename AccessorType::DataType> ValueMapper;
        typedef typename PropertyMapper<AccessorType, ValueMapper::type>::Type PropertyType;

        return new PropertyType(name, AccessorType(boost::bind(boost::type<ReturnType>(), accessor1, boost::bind(accessor3, _1)),
                                                   boost::bind(boost::type<void>(), accessor2, boost::bind(accessor3, _1), _2)));
    }
};

} // namespace detail

} // namespace camp


#endif // CAMP_DETAIL_PROPERTYFACTORY_HPP
