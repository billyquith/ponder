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
#ifndef PONDER_UTIL_HPP
#define PONDER_UTIL_HPP

#include <ponder/config.hpp>
#include <ponder/type.hpp>
#include <type_traits>
#include <memory>

namespace ponder {
namespace detail {

//------------------------------------------------------------------------------

template<bool C, typename T, typename F>
struct if_c
{
    typedef T type;
};

template<typename T, typename F>
struct if_c<false,T,F>
{
    typedef F type;
};

//------------------------------------------------------------------------------

class bad_conversion : public std::exception {};

template <typename T, typename F, typename O = void>
struct convert_impl
{
    T operator () (const F& from)
    {
        const T result = static_cast<T>(from);
        return result;
    }
};

template <typename F>
Id to_str(F from)
{
    return std::to_string(from);
}
    
template <typename F>
struct convert_impl <Id, F>
{
    Id operator () (const F& from)
    {
        return detail::to_str(from);
    }
};

template <>
struct convert_impl <Id, bool>
{
    Id operator () (const bool& from)
    {
        static const Id t("1"), f("0");
        return from ? t : f;
    }
};

PONDER_API bool conv(const String& from, bool& to);
PONDER_API bool conv(const String& from, char& to);
PONDER_API bool conv(const String& from, unsigned char& to);
PONDER_API bool conv(const String& from, short& to);
PONDER_API bool conv(const String& from, unsigned short& to);
PONDER_API bool conv(const String& from, int& to);
PONDER_API bool conv(const String& from, unsigned int& to);
PONDER_API bool conv(const String& from, long& to);
PONDER_API bool conv(const String& from, unsigned long& to);
PONDER_API bool conv(const String& from, long long& to);
PONDER_API bool conv(const String& from, unsigned long long& to);
PONDER_API bool conv(const String& from, float& to);
PONDER_API bool conv(const String& from, double& to);

template <typename T>
struct convert_impl <T, Id,
    typename std::enable_if< (std::is_integral<T>::value || std::is_floating_point<T>::value)
                             && !std::is_const<T>::value
                             && !std::is_reference<T>::value >::type >
{
    T operator () (const String& from)
    {
        T result;
        if (!conv(from, result))
            throw detail::bad_conversion();
        return result;
    }
};

template <typename T, typename F>
T convert(const F& from)
{
    return convert_impl<T,F>()(from);
}

//------------------------------------------------------------------------------
// index_sequence
// From: http://stackoverflow.com/a/32223343/3233
// A pre C++14 version supplied here. MSVC chokes on this but has its own version.
//
#ifdef _MSC_VER
#   define PONDER__SEQNS std
#else
#   define PONDER__SEQNS ::ponder::detail

template <std::size_t... Ints>
struct index_sequence
{
    using type = index_sequence;
    using value_type = std::size_t;
    static constexpr std::size_t size() { return sizeof...(Ints); }
};

template <class Sequence1, class Sequence2>
struct _merge_and_renumber;

template <size_t... I1, size_t... I2>
struct _merge_and_renumber<index_sequence<I1...>, index_sequence<I2...>>
: index_sequence<I1..., (sizeof...(I1)+I2)...>
{};

template <size_t N>
struct make_index_sequence
: _merge_and_renumber<typename make_index_sequence<N/2>::type,
typename make_index_sequence<N - N/2>::type>
{};

template<> struct make_index_sequence<0> : index_sequence<> { };
template<> struct make_index_sequence<1> : index_sequence<0> { };

#endif // index_sequence

//------------------------------------------------------------------------------
// make_unique supplied for compilers missing it (e.g. clang 5.0 on Travis Linux).
// source: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3656.htm
    
template<class T> struct _Unique_if {
    typedef std::unique_ptr<T> _Single_object;
};

template<class T> struct _Unique_if<T[]> {
    typedef std::unique_ptr<T[]> _Unknown_bound;
};

template<class T, std::size_t N> struct _Unique_if<T[N]> {
    typedef void _Known_bound;
};

template<class T, class... Args>
typename _Unique_if<T>::_Single_object
make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template<class T>
typename _Unique_if<T>::_Unknown_bound
make_unique(std::size_t n) {
    typedef typename std::remove_extent<T>::type U;
    return std::unique_ptr<T>(new U[n]());
}

template<class T, class... Args>
typename _Unique_if<T>::_Known_bound
make_unique(Args&&...) = delete;
    
//------------------------------------------------------------------------------
// Return true if all args true. Useful for variadic template expansions.
static inline bool allTrue()
    {return true;}
static inline bool allTrue(bool a0)
    {return a0;}
static inline bool allTrue(bool a0,bool a1)
    {return a0 & a1;}
static inline bool allTrue(bool a0,bool a1,bool a2)
    {return a0 & a1 & a2;}
static inline bool allTrue(bool a0,bool a1,bool a2,bool a3)
    {return a0 & a1 & a2 & a3;}
static inline bool allTrue(bool a0,bool a1,bool a2,bool a3,bool a4)
    {return a0 & a1 & a2 & a3 & a4;}
static inline bool allTrue(bool a0,bool a1,bool a2,bool a3,bool a4, bool a5)
    {return a0 & a1 & a2 & a3 & a4 & a5;}
static inline bool allTrue(bool a0,bool a1,bool a2,bool a3,bool a4, bool a5, bool a6)
    {return a0 & a1 & a2 & a3 & a4 & a5 & a6;}
static inline bool allTrue(bool a0,bool a1,bool a2,bool a3,bool a4, bool a5, bool a6, bool a7)
    {return a0 & a1 & a2 & a3 & a4 & a5 & a6 & a7;}

// Get value type enum as string description.
PONDER_API const char* valueTypeAsString(ValueKind t);

} // detail
} // Ponder

#endif // PONDER_UTIL_HPP
