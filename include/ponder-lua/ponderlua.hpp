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

#ifndef PONDER_LUA_HPP
#define PONDER_LUA_HPP

#include <ponder/class.hpp>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace ponder {
namespace lua
    
/**
* \brief Expose a single Ponder metaclass to a Lua state
*
* This
*
* \param cls Name of the metaclass in Ponder.
* \param L Lua state in which to expose the class.
*/
void exposeClass(Class& cls, lua_State *L);

/**
* \brief Expose all existing Ponder registered objects to a Lua state
*
* \param L Lua state in which to expose the objects.
*/
// void exposeAll(lua_State *L);

} // namespace lua
} // namespace ponder

#endif // PONDER_LUA_HPP
