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


#ifndef PONDER_ARRAYMAPPER_HPP
#define PONDER_ARRAYMAPPER_HPP


#include <ponder/config.hpp>
#include <ponder/detail/yesnotype.hpp>
#include <list>
#include <vector>


namespace ponder_ext
{
/**
 * \class ArrayMapper
 *
 * \brief Template providing a mapping between C++ arrays and Ponder ArrayProperty
 *
 * ArrayMapper<T> must define the following members in order to make T fully compliant with the system:
 *
 * \li \c ElementType: type of the elements stored in the array
 * \li \c dynamic(): tells if the array is dynamic (i.e. supports insert and remove)
 * \li \c size(): retrieve the size of the array
 * \li \c get(): get the value of an element
 * \li \c set(): set the value of an element
 * \li \c insert(): insert a new element
 * \li \c remove(): remove an element
 *
 * ValueMapper is specialized for every supported type, and can be specialized
 * for any of your own array types in order to extend the system.
 *
 * By default, ValueMapper supports the following types of array:
 *
 * \li T[]
 * \li boost::array
 * \li std::vector
 * \li std::list
 *
 * Here is an example of mapping for the std::vector class:
 *
 * \code
 * namespace ponder_ext
 * {
 *     template <typename T>
 *     struct ArrayMapper<std::vector<T> >
 *     {
 *         typedef T ElementType;
 *         
 *         static bool dynamic()
 *         {
 *             return true;
 *         }
 *
 *         static std::size_t size(const std::vector<T>& arr)
 *         {
 *             return arr.size();
 *         }
 *
 *         static const T& get(const std::vector<T>& arr, std::size_t index)
 *         {
 *             return arr[index];
 *         }
 *
 *         static void set(std::vector<T>& arr, std::size_t index, const T& value)
 *         {
 *             arr[index] = value;
 *         }
 *
 *         static void insert(std::vector<T>& arr, std::size_t before, const T& value)
 *         {
 *             arr.insert(arr.begin() + before, value);
 *         }
 *
 *         static void remove(std::vector<T>& arr, std::size_t index)
 *         {
 *             arr.erase(arr.begin() + index);
 *         }
 *     };
 * }
 * \endcode
 */

/*
 * Generic version -- doesn't define anything
 */
template <typename T>
struct ArrayMapper
{
};

/*
 * Specialization of ArrayMapper for built-in static arrays
 */
template <typename T, std::size_t N>
struct ArrayMapper<T[N]>
{
    typedef T ElementType;

    static bool dynamic()
    {
        return false;
    }

    static std::size_t size(T (&)[N])
    {
        return N;
    }

    static const T& get(T (& arr)[N], std::size_t index)
    {
        return arr[index];
    }

    static void set(T (& arr)[N], std::size_t index, const T& value)
    {
        arr[index] = value;
    }

    static void insert(T (&)[N], std::size_t, const T&)
    {
    }

    static void remove(T (&)[N], std::size_t)
    {
    }
};

/*
 * Specialization of ArrayMapper for std::array
 */
template <typename T, std::size_t N>
struct ArrayMapper<std::array<T, N> >
{
    typedef T ElementType;

    static bool dynamic()
    {
        return false;
    }

    static std::size_t size(const std::array<T, N>&)
    {
        return N;
    }

    static const T& get(const std::array<T, N>& arr, std::size_t index)
    {
        return arr[index];
    }

    static void set(std::array<T, N>& arr, std::size_t index, const T& value)
    {
        arr[index] = value;
    }

    static void insert(std::array<T, N>&, std::size_t, const T&)
    {
    }

    static void remove(std::array<T, N>&, std::size_t)
    {
    }
};

/*
 * Specialization of ArrayMapper for boost::array
 */
//template <typename T, std::size_t N>
//struct ArrayMapper<boost::array<T, N> >
//{
//    typedef T ElementType;
//
//    static bool dynamic()
//    {
//        return false;
//    }
//
//    static std::size_t size(const boost::array<T, N>&)
//    {
//        return N;
//    }
//
//    static const T& get(const boost::array<T, N>& arr, std::size_t index)
//    {
//        return arr[index];
//    }
//
//    static void set(boost::array<T, N>& arr, std::size_t index, const T& value)
//    {
//        arr[index] = value;
//    }
//
//    static void insert(boost::array<T, N>&, std::size_t, const T&)
//    {
//    }
//
//    static void remove(boost::array<T, N>&, std::size_t)
//    {
//    }
//};

/*
 * Specialization of ArrayMapper for std::vector
 */
 template <typename T>
 struct ArrayMapper<std::vector<T> >
 {
    typedef T ElementType;

    static bool dynamic()
    {
        return true;
    }

    static std::size_t size(const std::vector<T>& arr)
    {
        return arr.size();
    }

    static const T& get(const std::vector<T>& arr, std::size_t index)
    {
        return arr[index];
    }

    static void set(std::vector<T>& arr, std::size_t index, const T& value)
    {
        arr[index] = value;
    }

    static void insert(std::vector<T>& arr, std::size_t before, const T& value)
    {
        arr.insert(arr.begin() + before, value);
    }

    static void remove(std::vector<T>& arr, std::size_t index)
    {
        arr.erase(arr.begin() + index);
    }
};

/*
 * Specialization of ArrayMapper for std::list
 */
 template <typename T>
 struct ArrayMapper<std::list<T> >
 {
    typedef T ElementType;

    static bool dynamic()
    {
        return true;
    }

    static std::size_t size(const std::list<T>& arr)
    {
        return arr.size();
    }

    static const T& get(const std::list<T>& arr, std::size_t index)
    {
        typename std::list<T>::const_iterator it = arr.begin();
        std::advance(it, index);
        return *it;
    }

    static void set(std::list<T>& arr, std::size_t index, const T& value)
    {
        typename std::list<T>::iterator it = arr.begin();
        std::advance(it, index);
        *it = value;
    }

    static void insert(std::list<T>& arr, std::size_t before, const T& value)
    {
        typename std::list<T>::iterator it = arr.begin();
        std::advance(it, before);
        arr.insert(it, value);
    }

    static void remove(std::list<T>& arr, std::size_t index)
    {
        typename std::list<T>::iterator it = arr.begin();
        std::advance(it, index);
        arr.erase(it);
    }
};

} // namespace ponder_ext


namespace ponder
{
namespace detail
{
/**
 * \brief Helper structure to check at compile time if a type is an array
 *
 * This structure check if the specialization of ArrayMapper for the
 * template parameter T exists (i.e. the ElementType member is properly defined).
 */
template <typename T>
struct IsArray
{
    template <typename U> static TypeYes check(typename U::ElementType*);
    template <typename U> static TypeNo  check(...);

    enum {value = sizeof(check<ponder_ext::ArrayMapper<T> >(0)) == sizeof(TypeYes)};
};

} // namespace detail

} // namespace ponder


#endif // PONDER_ARRAYMAPPER_HPP
