

#ifndef CAMP_DETAIL_ISSMARTPOINTER_HPP
#define CAMP_DETAIL_ISSMARTPOINTER_HPP


#include <camp/detail/yesnotype.hpp>
#include <boost/utility/enable_if.hpp>


namespace camp
{
namespace detail
{
/**
 * \brief Utility class which tells at compile-time if a type T is a smart pointer to a type U
 *
 * To detect a smart pointer type, we check using SFINAE if T implements an operator -> returning a U*
 */
template <typename T, typename U>
struct IsSmartPointer
{
    // Check non-const signature
    template <typename V, U* (V::*)()> struct TestForMember {};
    template <typename V> static TypeYes check(TestForMember<V, &V::operator-> >*);
    template <typename V> static TypeNo  check(...);

    // Check const signature
    template <typename V, U* (V::*)() const> struct TestForConstMember {};
    template <typename V> static TypeYes checkConst(TestForConstMember<V, &V::operator-> >*);
    template <typename V> static TypeNo  checkConst(...);

    enum {value = (sizeof(check<T>(0)) == sizeof(TypeYes))
               || (sizeof(checkConst<T>(0)) == sizeof(TypeYes))};
};

} // namespace detail

} // namespace camp


namespace boost
{
/**
 * \brief Specialization of boost::get_pointer for all smart pointer types (const version)
 *
 * This function is specialized for every type T for which IsSmartPointer<T> is true. It makes
 * the stored value available for all boost algorithms (especially for boost::bind).
 */
template <template <typename> class T, typename U>
U* get_pointer(const T<U>& obj, typename enable_if<camp::detail::IsSmartPointer<T<U>, U> >::type* = 0)
{
    return obj.operator->();
}

/**
 * \brief Specialization of boost::get_pointer for all smart pointer types (non-const version)
 *
 * This function is specialized for every type T for which IsSmartPointer<T> is true. It makes
 * the stored value available for all boost algorithms (especially for boost::bind).
 */
template <template <typename> class T, typename U>
U* get_pointer(T<U>& obj, typename enable_if<camp::detail::IsSmartPointer<T<U>, U> >::type* = 0)
{
    return obj.operator->();
}

} // namespace boost


#endif // CAMP_DETAIL_ISSMARTPOINTER_HPP
