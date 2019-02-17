/****************************************************************************
**
** This file is part of the Ponder library.
**
** The MIT License (MIT)
**
** Copyright (C) 2015-2019 Nick Trout.
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
#ifndef PONDER_USES_DETAIL_HPP
#define PONDER_USES_DETAIL_HPP

#include <ponder/uses/detail/runtime.hpp>
#include <ponder/uses/detail/lua.hpp>

/**
 * \namespace ponder::uses
 * \brief Uses for Ponder.
 * Code within this namespace uses the types registered with Ponder for various purposes.
 */

namespace ponder {
namespace uses {

/**
 *  The "uses" are a way for users of the compile-time types to store
 *  information. For example, this may be templated code that uses the types only
 *  available during compilation. These may then be used at runtime. The idea is
 *  to decouple modules from the metaclass data to avoid complexity.
 *      
 *  Each module supplies (pseudo-code):
 *      
 *  \code
 *  struct Use_name {
 *      static module_ns::detail::PerConstructor_t* perConstructor(IdRef name, C constructor)
 *      static module_ns::detail::PerFunc_t* perFunction(IdRef name, F function)
 *  }
 *  \endcode
 */
        
/**
 *  \brief Runtime behaviour
 *
 *  This module provides runtime behaviour like creation of UserObjects and calling 
 *  functions
 */
struct RuntimeUse
{
    /// Factory for per-function runtime data
    template <typename F, typename FTraits, typename Policies_t>
    static runtime::detail::FunctionCaller* perFunction(IdRef name, F function)
    {
        return new runtime::detail::FunctionCallerImpl<F, FTraits, Policies_t>(name, function);
    }
};

/**
 *  \brief Lua behaviour
 *
 *  This module provides Lua support.
 */
#if PONDER_USING_LUA
struct LuaUse
{
    /// Factory for per-function runtime data
    template <typename F, typename FTraits, typename Policies_t>
    static lua::detail::FunctionCaller* perFunction(IdRef name, F function)
    {
        return new lua::detail::FunctionCallerImpl<F, FTraits, Policies_t>(name, function);
    }
};
#endif // PONDER_USING_LUA

/**
 * \brief Global information on the compile-time type Uses.
 *
 *  - This can be extended for other modular uses
 */
struct Uses
{
    enum {
        eRuntimeModule,                 ///< Runtime module enumeration
        PONDER_IF_LUA(eLuaModule,)      ///< Lua module enumeration
        eUseCount
    };
    
     /// Metadata uses we are using.
    typedef std::tuple<RuntimeUse
                       PONDER_IF_LUA(,LuaUse)
                      > Users;

    /// Type that stores the per-function uses data
    typedef std::tuple<
            runtime::detail::FunctionCaller*
            PONDER_IF_LUA(,lua::detail::FunctionCaller*)
        > PerFunctionUserData;
    
    // Access note:
    //  typedef typename std::tuple_element<I, PerFunctionUserData>::type PerFunc_t;
    //  PerFunc_t* std::get<I>(getUsesData());
};
    
static_assert(Uses::eUseCount==std::tuple_size<Uses::Users>::value, "Size mismatch");
static_assert(Uses::eUseCount==std::tuple_size<Uses::PerFunctionUserData>::value, "Size mismatch");

} // namespace uses
} // namespace ponder

#endif // PONDER_USES_DETAIL_HPP
