

#ifndef CAMP_DETAIL_FUNCTIONTRAITS_HPP
#define CAMP_DETAIL_FUNCTIONTRAITS_HPP


#include <camp/detail/yesnotype.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/function_types/is_callable_builtin.hpp>
#include <boost/function_types/result_type.hpp>


namespace camp
{
namespace detail
{
/**
 * \brief Helper structure to check at compile time if a type is a functor
 *
 * This structure checks if the provided type defines a result_type type.
 */
template <typename T>
struct HasResultType
{
    template <typename U, typename V> struct TestForMember {};
    template <typename U> static TypeYes check(TestForMember<U, typename U::result_type>*);
    template <typename U> static TypeNo  check(...);

    enum {value = sizeof(check<T>(0)) == sizeof(TypeYes)};
};

/**
 * \class FunctionTraits
 *
 * \brief Utility class which gives compile-time informations about function types
 *
 * The FunctionTraits provides two informations about a type T:
 * \li A compile-time constant \c isFunction which tells if T is any callable type
 * \li A type \c ReturnType which is the type returned by T
 */

/**
 * General case for all unspecified types: not a function
 */
template <typename T, typename E = void>
struct FunctionTraits
{
    enum {isFunction = false};
};

/**
 * Specialization for native callable types (function and pointer-to-member types)
 */
template <typename T>
struct FunctionTraits<T, typename boost::enable_if<boost::function_types::is_callable_builtin<T> >::type>
{
    enum {isFunction = true};
    typedef typename boost::function_types::result_type<T>::type ReturnType;
};

/**
 * Specialization for functors (classes exporting a result_type type)
 */
template <typename T>
struct FunctionTraits<T, typename boost::enable_if<HasResultType<T> >::type>
{
    enum {isFunction = true};
    typedef typename T::result_type ReturnType;
};

} // namespace detail

} // namespace camp


#endif // CAMP_DETAIL_FUNCTIONTRAITS_HPP
