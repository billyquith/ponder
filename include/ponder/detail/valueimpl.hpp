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


#ifndef PONDER_DETAIL_VALUEIMPL_HPP
#define PONDER_DETAIL_VALUEIMPL_HPP

#include <ponder/type.hpp>
#include <ponder/valuemapper.hpp>


namespace ponder {
namespace detail {
    
/**
 * \brief Value visitor which converts the stored value to a type T
 */
template <typename T>
struct ConvertVisitor
{
    typedef T result_type;
    
    template <typename U>
    T operator()(const U& value) const
    {
        // Dispatch to the proper ValueConverter
        return ponder_ext::ValueMapper<T>::from(value);
    }

    T operator()(const T& value) const
    {
        // Optimization when source type is the same as requested type
        return value;
    }

    T operator()(NoType) const
    {
        // Error: trying to convert an empty value
        PONDER_ERROR(BadType(noType, mapType<T>()));
    }
};

/**
 * \brief Binary value visitor which compares two values using operator <
 */
struct LessThanVisitor
{
    typedef bool result_type;
    
    template <typename T, typename U>
    bool operator()(const T&, const U&) const
    {
        // Different types : compare types identifiers
        return mapType<T>() < mapType<U>();
    }

    template <typename T>
    bool operator()(const T& v1, const T& v2) const
    {
        // Same types : compare values
        return v1 < v2;
    }

    bool operator()(NoType, NoType) const
    {
        // No type (empty values) : they're considered equal
        return false;
    }
};

/**
 * \brief Binary value visitor which compares two values using operator ==
 */
struct EqualVisitor
{
    typedef bool result_type;

    template <typename T, typename U>
    bool operator()(const T&, const U&) const
    {
        // Different types : not equal
        return false;
    }

    template <typename T>
    bool operator()(const T& v1, const T& v2) const
    {
        // Same types : compare values
        return v1 == v2;
    }

    bool operator()(NoType, NoType) const
    {
        // No type (empty values) : they're considered equal
        return true;
    }
};

} // namespace detail

} // namespace ponder


#endif // PONDER_DETAIL_VALUEIMPL_HPP
