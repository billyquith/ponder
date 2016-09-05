/****************************************************************************
**
** This file is part of the Ponder library.
**
** The MIT License (MIT)
**
** Copyright (C) 2016 Bill Quith.
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

#ifndef PONDER_USES_DETAIL_HPP
#define PONDER_USES_DETAIL_HPP

#include <ponder/uses/detail/runtime.hpp>
#include <ponder/uses/detail/lua.hpp>

namespace ponder {
namespace uses {

struct RuntimeModule
{
    template <typename T, typename F>
    static runtime::impl::FunctionCaller* perFunction(IdRef name, F function)
    {
        static constexpr int implType = runtime::impl::FuncImplTypeMap<(int)T::family>::Type;
        
        return new runtime::impl::FunctionCallerImpl<implType, typename T::FunctionType>(name, function);
    }
};

struct LuaModule
{
    template <typename T, typename F>
    static runtime::impl::FunctionCaller* perFunction(IdRef name, F function)
    {
        static constexpr int implType = runtime::impl::FuncImplTypeMap<(int)T::family>::Type;
        
        return new runtime::impl::FunctionCallerImpl<implType, typename T::FunctionType>(name, function);
    }
};
    
// Global information on the compile-time type users.
struct Users
{
    enum { eRuntimeModule, eLuaModule };
    
    typedef std::tuple<RuntimeModule, LuaModule> Modules;
    
    typedef std::tuple<runtime::impl::FunctionCaller*,
                       lua::impl::FunctionBinding*> PerFunctionUserData;

//    template <int I>
//    struct PerFuncType { typedef typename std::tuple_element<I, PerFunctionUserData>::type Type; };
//    
//    template <int I>
//    static typename PerFuncType<I>::Type getPerFuncData(PerFunctionUserData &ud) { return std::get<I>(ud); }
};
    
} // namespace uses
} // namespace ponder

#endif // PONDER_USES_DETAIL_HPP
