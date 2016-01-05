/****************************************************************************
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
**
** This file is part of the Ponder library, formerly CAMP.
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


#ifndef PONDER_DETAIL_RAWTYPE_HPP
#define PONDER_DETAIL_RAWTYPE_HPP


#include <ponder/detail/issmartpointer.hpp>


namespace ponder
{
namespace detail
{
/**
 * \class RawType
 *
 * \brief Helper structure used to extract the raw type of a composed type
 *
 * RawType<T> recursively removes const, reference and pointer modifiers from the given type.
 * In other words:
 *
 * \li RawType<T>::Type == T
 * \li RawType<const T>::Type == RawType<T>::Type
 * \li RawType<T&>::Type == RawType<T>::Type
 * \li RawType<const T&>::Type == RawType<T>::Type
 * \li RawType<T*>::Type == RawType<T>::Type
 * \li RawType<const T*>::Type == RawType<T>::Type
 *
 * \remark RawType is able to detect smart pointers and properly extract the stored type
 */

/*
 * Generic version -- T doesn't match with any of our specialization, and is thus considered a raw type
 */
template <typename T, typename E = void>
struct RawType
{
    typedef T Type;
};

/*
 * Specialized version for const modifier
 */
template <typename T> struct RawType<const T>
{
    typedef typename RawType<T>::Type Type;
};

/*
 * Specialized version for reference modifier
 */
template <typename T> struct RawType<T&>
{
    typedef typename RawType<T>::Type Type;
};

/*
 * Specialized version for raw pointers
 */
template <typename T>
struct RawType<T*>
{
    typedef typename RawType<T>::Type Type;
};

/*
 * Specialized version for smart pointers
 */
template <template <typename> class T, typename U>
struct RawType<T<U>, typename std::enable_if<IsSmartPointer<T<U>, U>::value >::type>
{
    typedef typename RawType<U>::Type Type;
};

} // namespace detail

} // namespace ponder


#endif // PONDER_DETAIL_RAWTYPE_HPP
