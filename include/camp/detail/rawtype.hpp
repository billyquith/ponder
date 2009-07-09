/****************************************************************************
**
** Copyright (C) 2009 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** Commercial Usage
** Licensees holding valid CAMP Commercial licenses may use this file in
** accordance with the CAMP Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and TECHNOGERMA Systems France.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3.txt included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at sales@technogerma.fr.
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
