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


#ifndef PONDER_DETAIL_VALUEPROVIDER_HPP
#define PONDER_DETAIL_VALUEPROVIDER_HPP


#include <ponder/args.hpp>
#include <ponder/class.hpp>
#include <ponder/classget.hpp>
#include <ponder/valuemapper.hpp>


namespace ponder
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
    T operator()() {return T();}
};

/*
 * Specialization for user types: use metaclass to instanciate
 * so that we get an exception rather than a compile error
 * if the type has no default constructor
 */
template <typename T>
struct ValueProviderImpl<T, userType>
{
    ValueProviderImpl() : m_value(classByType<T>().construct(Args::empty).template get<T*>()) {}
    ~ValueProviderImpl() {classByType<T>().destroy(m_value);}
    T& operator()() {return *m_value;}
    T* m_value;
};

/*
 * Specialization for pointer to primitive types: use new to allocate objects
 * Here we assume that the caller will take ownership of the returned value
 */
template <typename T, int Type>
struct ValueProviderImpl<T*, Type>
{
    T* operator()() {return new T;}
};

/*
 * Specialization for pointer to user types: use metaclass to allocate objects
 * Here we assume that the caller will take ownership of the returned value
 */
template <typename T>
struct ValueProviderImpl<T*, userType>
{
    T* operator()() {return classByType<T>().construct().template get<T*>();}
};

/*
 * Helper structure to instanciate new values based on their type
 */
template <typename T>
struct ValueProvider : ValueProviderImpl<T, ponder_ext::ValueMapper<T>::type>
{
};

} // namespace detail

} // namespace ponder


#endif // PONDER_DETAIL_VALUEPROVIDER_HPP
