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
