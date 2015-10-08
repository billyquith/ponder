/****************************************************************************
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
**
** This file is part of the CAMP library.
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


#ifndef CAMP_DETAIL_CALLHELPER_HPP
#define CAMP_DETAIL_CALLHELPER_HPP


#include <camp/value.hpp>


namespace camp
{
namespace detail
{
/**
 * \class CallHelper
 *
 * \brief Helper structure used to make a function call and return the result as a camp::Value.
 *
 * This structure is necessary because we need a specific code for functions returning void.
 */

/*
 * Generic version of CallHelper
 */
template <typename R, typename C>
struct CallHelper
{
    template <typename F, typename... A>
    static Value call(F func, C obj, A... args) {return func(obj, args...);}
};

/*
 * Specialization of CallHelper for functions returning void
 */
template <typename C>
struct CallHelper<void, C>
{
    template <typename F, typename... A>
    static Value call(F func, C obj, A... args) {func(obj, args...); return Value::nothing;}
};

} // namespace detail

} // namespace camp


#endif // CAMP_DETAIL_CALLHELPER_HPP
