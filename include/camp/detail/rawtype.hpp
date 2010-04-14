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


#ifndef CAMP_DETAIL_RAWTYPE_HPP
#define CAMP_DETAIL_RAWTYPE_HPP


#include <camp/detail/issmartpointer.hpp>


namespace camp
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
struct RawType<T<U>, typename boost::enable_if<IsSmartPointer<T<U>, U> >::type>
{
    typedef typename RawType<U>::Type Type;
};

} // namespace detail

} // namespace camp


#endif // CAMP_DETAIL_RAWTYPE_HPP
