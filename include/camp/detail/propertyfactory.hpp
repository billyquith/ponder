// <insert copyright>

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
 * type is not copyable. Instead, we throw a CAMP error.
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
 * Specialization of CopyTraits for non-copyable types
 *
 * This specialization triggers an error
 */
template <typename T>
struct CopyHelper<T, typename boost::enable_if_c<!StaticTypeId<T>::copyable>::type>
{
    static bool copy(T&, const Value&)
    {
        return false;
    }
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

    typename Traits::ReturnType get(C& object) const
    {
        return Traits::get(m_getter(object));
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

    typename Traits::ReturnType get(C& object) const
    {
        return Traits::get(m_getter(object));
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

    typename Traits::ReturnType get(C& object) const
    {
        return Traits::get(m_getter(object));
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

    typename Traits::ReturnType get(C& object) const
    {
        return Traits::get(m_getter1(m_getter2(object)));
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

    typename Traits::ReturnType get(C& object) const
    {
        return Traits::get(m_getter1(m_getter2(object)));
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
