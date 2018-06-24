/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2018 Nick Trout.
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

#pragma once
#ifndef PONDER_DETAIL_VALUEPROVIDER_HPP
#define PONDER_DETAIL_VALUEPROVIDER_HPP

#include <ponder/args.hpp>
#include <ponder/class.hpp>
#include <ponder/classget.hpp>
#include <ponder/valuemapper.hpp>

namespace ponder {
namespace detail {
    
/*
 * Implementation of ValueProvider
 * Generic version, use default constructor
 */
template <typename T, ValueKind Type>
struct ValueProviderImpl
{
    T operator()() {return T();}
};

/*
 * Specialization for user types: use metaclass to instantiate
 * so that we get an exception rather than a compile error
 * if the type has no default constructor
 */
template <typename T>
struct ValueProviderImpl<T, ValueKind::User>
{
    ValueProviderImpl()
        :   m_value(0) //classByType<T>().construct(Args::empty).template get<T*>()) // XXXX
    {}
    ~ValueProviderImpl() {} // {classByType<T>().destroy(m_value);}
    T& operator()() {return *m_value;}
    T* m_value;
};

/*
 * Specialization for pointer to primitive types: use new to allocate objects
 * Here we assume that the caller will take ownership of the returned value
 */
template <typename T, ValueKind Type>
struct ValueProviderImpl<T*, Type>
{
    T* operator()() {return new T;}
};

/*
 * Specialization for pointer to user types: use metaclass to allocate objects
 * Here we assume that the caller will take ownership of the returned value
 */
template <typename T>
struct ValueProviderImpl<T*, ValueKind::User>
{
    T* operator()() {return classByType<T>().construct().template get<T*>();}
};

/*
 * Helper structure to instantiate new values based on their type
 */
template <typename T>
struct ValueProvider : ValueProviderImpl<T, ponder_ext::ValueMapper<T>::kind>
{
};

} // namespace detail
} // namespace ponder

#endif // PONDER_DETAIL_VALUEPROVIDER_HPP
