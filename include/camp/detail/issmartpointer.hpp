/****************************************************************************
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
**
** This file is part of the CAMP library.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
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


#ifndef CAMP_DETAIL_ISSMARTPOINTER_HPP
#define CAMP_DETAIL_ISSMARTPOINTER_HPP


#include <camp/detail/yesnotype.hpp>
#include <boost/utility/enable_if.hpp>


namespace camp
{

template<class T>
T* get_pointer(T *p)
{
    return p;
}
    
template<class T>
T* get_pointer(std::auto_ptr<T> const& p)
{
    return p.get();
}

template<class T>
T* get_pointer(std::unique_ptr<T> const& p)
{
    return p.get();
}

template<class T>
T* get_pointer(std::shared_ptr<T> const& p)
{
    return p.get();
}

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
