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
    template <typename F>
    static Value call(F func, C obj) {return func(obj);}

    template <typename F, typename A0>
    static Value call(F func, C obj, A0 a0) {return func(obj, a0);}

    template <typename F, typename A0, typename A1>
    static Value call(F func, C obj, A0 a0, A1 a1) {return func(obj, a0, a1);}

    template <typename F, typename A0, typename A1, typename A2>
    static Value call(F func, C obj, A0 a0, A1 a1, A2 a2) {return func(obj, a0, a1, a2);}

    template <typename F, typename A0, typename A1, typename A2, typename A3>
    static Value call(F func, C obj, A0 a0, A1 a1, A2 a2, A3 a3) {return func(obj, a0, a1, a2, a3);}

    template <typename F, typename A0, typename A1, typename A2, typename A3, typename A4>
    static Value call(F func, C obj, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {return func(obj, a0, a1, a2, a3, a4);}
};

/*
 * Specialization of CallHelper for functions returning void
 */
template <typename C>
struct CallHelper<void, C>
{
    template <typename F>
    static Value call(F func, C obj) {func(obj); return Value::nothing;}

    template <typename F, typename A0>
    static Value call(F func, C obj, A0 a0) {func(obj, a0); return Value::nothing;}

    template <typename F, typename A0, typename A1>
    static Value call(F func, C obj, A0 a0, A1 a1) {func(obj, a0, a1); return Value::nothing;}

    template <typename F, typename A0, typename A1, typename A2>
    static Value call(F func, C obj, A0 a0, A1 a1, A2 a2) {func(obj, a0, a1, a2); return Value::nothing;}

    template <typename F, typename A0, typename A1, typename A2, typename A3>
    static Value call(F func, C obj, A0 a0, A1 a1, A2 a2, A3 a3) {func(obj, a0, a1, a2, a3); return Value::nothing;}

    template <typename F, typename A0, typename A1, typename A2, typename A3, typename A4>
    static Value call(F func, C obj, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {func(obj, a0, a1, a2, a3, a4); return Value::nothing;}
};

} // namespace detail

} // namespace camp


#endif // CAMP_DETAIL_CALLHELPER_HPP
