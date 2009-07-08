

#ifndef CAMP_DETAIL_OBJECTTRAITS_HPP
#define CAMP_DETAIL_OBJECTTRAITS_HPP


#include <camp/detail/rawtype.hpp>
#include <camp/detail/issmartpointer.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/utility/enable_if.hpp>


namespace camp
{
namespace detail
{
/**
 * \class ObjectTraits
 *
 * \brief Utility class which gives compile-time informations about the semantics of a type
 *
 * It provides the following constants:
 *
 * \li isWritable: true if the type allows to modify the object (non-const references and pointers)
 * \li isRef: true if the type is a reference type (references and pointers)
 *
 * ... the following types:
 *
 * \li RefReturnType: the reference type closest to T which allows to have direct access to the object (T& for raw types and references, T* for pointer types)
 * \li PointerType: the pointer type closest to T which allows to have direct access to the object (T*)
 * \li DataType: the actual raw type of the object (removes all indirections, as well const and reference modifiers)
 *
 * ... and the following functions:
 *
 * \li get(void*): get a direct access to an object given by a typeless pointer (in other words, converts from void* to RefReturnType)
 * \li getPointer(T): get a direct pointer to an object, regardless its original storage / modifiers (in other words, convert from T to PointerType)
 */

/*
 * Generic version -- raw types
 */
template <typename T, typename E = void>
struct ObjectTraits
{
    enum
    {
        isWritable = false,
        isRef = false
    };

    typedef T& RefReturnType;
    typedef typename RawType<T>::Type DataType;

    static RefReturnType get(void* pointer) {return *static_cast<T*>(pointer);}
};

/*
 * Specialized version for raw pointers
 */
template <typename T>
struct ObjectTraits<T*>
{
    enum
    {
        isWritable = !boost::is_const<T>::value,
        isRef = true
    };

    typedef T* RefReturnType;
    typedef T* PointerType;
    typedef typename RawType<T>::Type DataType;

    static RefReturnType get(void* pointer) {return static_cast<T*>(pointer);}
    static PointerType getPointer(T* value) {return value;}
};

/*
 * Specialized version for smart pointers
 */
template <template <typename> class T, typename U>
struct ObjectTraits<T<U>, typename boost::enable_if<IsSmartPointer<T<U>, U> >::type>
{
    enum
    {
        isWritable = !boost::is_const<U>::value,
        isRef = true
    };

    typedef U* RefReturnType;
    typedef U* PointerType;
    typedef typename RawType<U>::Type DataType;

    static RefReturnType get(void* pointer) {return static_cast<U*>(pointer);}
    static PointerType getPointer(T<U> value) {return boost::get_pointer(value);}
};

/*
 * Specialized version for built-in arrays
 */
template <typename T, int N>
struct ObjectTraits<T[N]>
{
    enum
    {
        isWritable = false,
        isRef = true
    };

    typedef T(&RefReturnType)[N];
    typedef typename RawType<T>::Type DataType;
};

/*
 * Specialized version for references to non-ref types
 */
template <typename T>
struct ObjectTraits<T&, typename boost::disable_if<boost::is_pointer<typename ObjectTraits<T>::RefReturnType> >::type>
{
    enum
    {
        isWritable = !boost::is_const<T>::value,
        isRef = true
    };

    typedef T& RefReturnType;
    typedef T* PointerType;
    typedef typename RawType<T>::Type DataType;

    static RefReturnType get(void* pointer) {return *static_cast<T*>(pointer);}
    static PointerType getPointer(T& value) {return &value;}
};

/*
 * Specialized version for references to ref types -- just remove the reference modifier
 */
template <typename T>
struct ObjectTraits<T&, typename boost::enable_if<boost::is_pointer<typename ObjectTraits<T>::RefReturnType> >::type> : ObjectTraits<T>
{
};

/*
 * Specialized version for types with const modifier -- just remove it
 */
template <typename T>
struct ObjectTraits<const T> : ObjectTraits<T>
{
};

} // namespace detail

} // namespace camp


#endif // CAMP_DETAIL_OBJECTTRAITS_HPP
