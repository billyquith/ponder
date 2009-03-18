// <insert copyright>

#ifndef CAMP_DETAIL_RETURNTYPE_HPP
#define CAMP_DETAIL_RETURNTYPE_HPP


namespace camp
{
namespace detail
{

/**
 * \class ReturnType
 *
 * \brief Utility class used at compile-time to which type to use to avoid copying the returned object
 *
 * \li If T is a reference or a pointer type, ReturnType<T>::Type is T
 * \li Otherwise, ReturnType<T>::Type is T&
 */

template <typename T> struct ReturnType     {typedef T& Type;};
template <typename T> struct ReturnType<T&> {typedef T& Type;};
template <typename T> struct ReturnType<T*> {typedef T* Type;};

} // namespace detail

} // namespace camp


#endif // CAMP_DETAIL_RETURNTYPE_HPP
