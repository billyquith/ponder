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


#ifndef CAMP_DETAIL_VALUEPROVIDER_HPP
#define CAMP_DETAIL_VALUEPROVIDER_HPP


#include <camp/class.hpp>
#include <camp/classget.hpp>
#include <camp/valuemapper.hpp>


namespace camp
{
namespace detail
{
/*
 * Implementation of ValueProvider
 * Generic version, use default constructor
 */
template <typename T, int Type>
struct ValueProviderImpl
{
    T value() {return T();}
};

/*
 * Specialization for user types: use metaclass to instanciate
 * so that we get an exception rather than a compile error
 * if the type has no default constructor
 */
template <typename T>
struct ValueProviderImpl<T, userType>
{
    ValueProviderImpl() : m_value(classByType<T>().template construct<T>()) {}
    ~ValueProviderImpl() {classByType<T>().destroy(m_value);}
    T& value() {return *m_value;}
    T* m_value;
};

/*
 * Specialization for pointer to primitive types: use new to allocate objects
 * Here we assume that the caller will take ownership of the returned value
 */
template <typename T, int Type>
struct ValueProviderImpl<T*, Type>
{
    T* value() {return new T;}
};

/*
 * Specialization for pointer to user types: use metaclass to allocate objects
 * Here we assume that the caller will take ownership of the returned value
 */
template <typename T>
struct ValueProviderImpl<T*, userType>
{
    T* value() {return classByType<T>().template construct<T>();}
};

/*
 * Helper structure to instanciate new values based on their type
 */
template <typename T>
struct ValueProvider : ValueProviderImpl<T, camp_ext::ValueMapper<T>::type>
{
};

} // namespace detail

} // namespace camp


#endif // CAMP_DETAIL_VALUEPROVIDER_HPP
