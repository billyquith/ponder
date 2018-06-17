/****************************************************************************
**
** This file is part of the Ponder library.
**
** The MIT License (MIT)
**
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
#ifndef PONDER_USES_LUA_HPP
#define PONDER_USES_LUA_HPP

/**
 * \file
 * \brief This file contains the user Lua API for Ponder
 */

#include <ponder/class.hpp>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

/**
 * \namespace ponder::lua
 * \brief Contains Ponder Lua support public API.
 *
 * \namespace ponder::lua::detail
 * \brief Ponder Lua support hidden implementation details.
 *
 * \namespace ponder::lua::impl
 * \brief Ponder Lua support hidden implementation details.
 */

namespace ponder {
namespace lua {

/**
 * \brief Expose a single Ponder metaclass to a Lua state
 *
 * \param L Lua state in which to expose the class.
 * \param cls Metaclass instance in Ponder.
 * \param exposeName The name of the class in the Lua state.
 */
PONDER_API void expose(lua_State *L, const Class& cls, const IdRef exposeName);

/**
 * \brief Expose a single Ponder enumeration to a Lua state
 *
 * \param L Lua state in which to expose the class.
 * \param e Enum instance in Ponder.
 * \param exposeName The name of the class in the Lua state.
 */
PONDER_API void expose(lua_State *L, const Enum& e, const IdRef exposeName);

namespace detail {

template <typename T, typename U = void> struct Exposer {};
    
template <typename T>
struct Exposer<T, typename std::enable_if<std::is_class<T>::value>::type>
{
    static inline void exposeType(lua_State *L, const IdRef exposeName)
    {
        expose(L, classByType<T>(), exposeName);
    }
};

template <typename T>
struct Exposer<T, typename std::enable_if<std::is_enum<T>::value>::type>
{
    static inline void exposeType(lua_State *L, const IdRef exposeName)
    {
        expose(L, enumByType<T>(), exposeName);
    }
};

} // namespace detail

/**
 * \brief Expose a single Ponder type to a Lua state
 *
 * \param T Type to expose. Can be a class or enumeration.
 * \param L Lua state in which to expose the class.
 * \param exposeName The name of the class in the Lua state.
 */
template <typename T>
PONDER_API void expose(lua_State *L, const IdRef exposeName)
{
    detail::Exposer<T>::exposeType(L, exposeName);
}

/**
 * \brief Push a copy of a UserObject onto the Lua stack
 *
 * You choose whether to copy or reference an object. E.g.
 * \code
 * pushUserObject(L, val.cref<UserObject>());  // reference
 * pushUserObject(L, val.to<UserObject>());    // copy
 * \endcode
 *
 * \param L Lua state to use.
 * \param uobj The UserObject to copy.
 * \return Number of items pushed onto the Lua stack
 */
PONDER_API int pushUserObject(lua_State *L, const UserObject& uobj);
    
/**
 * \brief Expose all existing Ponder registered objects to a Lua state
 *
 * \param L Lua state in which to expose the objects.
 */
// void exposeAll(lua_State *L);
    
PONDER_API bool runString(lua_State *L, const char *luaCode);

} // namespace lua
} // namespace ponder

//----------------------------------------------------------------------------

// define once in client program to instance this
#ifdef PONDER_USES_LUA_IMPL

#include <ponder/uses/runtime.hpp>
#include <ponder/uses/detail/lua.hpp>

#define PONDER__LUA_METATBLS "_ponder_meta"
#define PONDER__LUA_INSTTBLS "_instmt"

namespace ponder {
namespace lua {
namespace impl {
    
// push a Ponder value onto the Lua state stack
static int pushValue(lua_State *L, const ponder::Value& val,
                     policy::ReturnKind retPolicy = policy::ReturnKind::Copy)
{
    switch (val.kind())
    {
        case ValueKind::Boolean:
            lua_pushboolean(L, val.to<bool>());
            return 1;
            
        case ValueKind::Integer:
            lua_pushinteger(L, val.to<lua_Integer>());
            return 1;
            
        case ValueKind::Real:
            lua_pushnumber(L, val.to<lua_Number>());
            return 1;
            
        case ValueKind::String:
            lua_pushstring(L, val.to<std::string>().c_str());
            return 1;

        case ValueKind::Enum:
            lua_pushinteger(L, val.to<int>());
            return 1;
            
        case ValueKind::User:
            return pushUserObject(L, retPolicy == policy::ReturnKind::InternalRef
                                        ? val.cref<UserObject>()
                                        : val.to<UserObject>());
        default:
            luaL_error(L, "Unknown type in Ponder value");
    }
    return 0;
}

// get a Lua stack value as a Ponder value
static Value getValue(lua_State *L, int index,
                      ValueKind typeExpected = ValueKind::None, int argIndex = -1)
{
    if (index > lua_gettop(L))
        return Value();
    
    const int typei = lua_type(L, index);

    // if we expect a type then override Lua type to force an error if incorrect
    if (typeExpected != ValueKind::None)
    {
        int vtype = LUA_TNIL;
        switch (typeExpected) {
            case ValueKind::Boolean:
                vtype = LUA_TBOOLEAN;
                break;
                
            case ValueKind::String:
                vtype = LUA_TSTRING;
                break;
                
            case ValueKind::Real:
            case ValueKind::Integer:
                vtype = LUA_TNUMBER;
                break;
                
            case ValueKind::User:
                vtype = LUA_TUSERDATA;
                break;
                
            default: ;
        }
        
        if (vtype != typei)
        {
            if (argIndex < 0)
                luaL_error(L, "Expecting %s but got %s",
                           lua_typename(L, vtype), lua_typename(L, typei));
            else
                luaL_error(L, "Argument %d: expecting %s but got %s",
                           argIndex, lua_typename(L, vtype), lua_typename(L, typei));
        }
    }
    
    switch (typei)
    {
        case LUA_TNIL:
            return Value();
            
        case LUA_TBOOLEAN:
            return Value(lua_toboolean(L, index));

        case LUA_TNUMBER:
            return Value(lua_tonumber(L, index));

        case LUA_TSTRING:
            return Value(lua_tostring(L, index));

        case LUA_TUSERDATA:
            {
                void *ud = lua_touserdata(L, index);
                ponder::UserObject *uobj = (ponder::UserObject*) ud;
                return *uobj;
            }

        default:
            luaL_error(L, "Cannot convert %s to Ponder value", lua_typename(L, typei));
    }
    
    return Value(); // no value
}
    
// obj[key]
static int l_inst_index(lua_State *L)
{
    lua_pushvalue(L, lua_upvalueindex(1));
    const Class *cls = (const Class *) lua_touserdata(L, -1);
    
    void *ud = lua_touserdata(L, 1);                // userobj - (obj, key) -> obj[key]
    const IdRef key(lua_tostring(L, 2));
    
    // check if getting property value
    const Property *pp = nullptr;
    if (cls->tryProperty(key, pp))
    {
        ponder::UserObject *uobj = (ponder::UserObject*) ud;
        return pushValue(L, pp->get(*uobj));
    }
    
    // check if calling function object
    const Function *fp = nullptr;
    if (cls->tryFunction(key, fp))
    {
        lua::impl::FunctionCaller *caller =
            std::get<uses::Uses::eLuaModule>(
                *reinterpret_cast<const uses::Uses::PerFunctionUserData*>(fp->getUsesData()));
        
        caller->pushFunction(L);
        return 1;
    }
    
    return 0;
}

// obj[key] = value
static int l_inst_newindex(lua_State *L)   // (obj, key, value) obj[key] = value
{
    lua_pushvalue(L, lua_upvalueindex(1));
    const Class *cls = (const Class *) lua_touserdata(L, -1);
    
    void *ud = lua_touserdata(L, 1);                // userobj
    const std::string key(lua_tostring(L, 2));
    
    // check if assigning to a property
    const Property *pp = nullptr;
    if (cls->tryProperty(key, pp))
    {
        ponder::UserObject *uobj = (ponder::UserObject*) ud;
        pp->set(*uobj, getValue(L, 3, pp->kind()));
    }
    
    return 0;
}

static int l_instance_create(lua_State *L)
{
    // get Class* from class object
    const ponder::Class *cls = *(const ponder::Class**) lua_touserdata(L, 1);
    
    ponder::Args args;
    constexpr int c_argOffset = 2;  // 1st arg is userdata object
    const int nargs = lua_gettop(L) - (c_argOffset-1);
    for (int i = c_argOffset; i < c_argOffset + nargs; ++i)
    {
        // there may be multiple constructors so don't check types
        args += getValue(L, i);
    }
    
    ponder::runtime::ObjectFactory fact(*cls);
    ponder::UserObject obj(fact.construct(args));
    if (obj == ponder::UserObject::nothing)
    {
        lua_pop(L, 1);  // pop new user data
        luaL_error(L, "Matching constructor not found");
    }
    
    void *ud = lua_newuserdata(L, sizeof(UserObject));   // +1
    new(ud) UserObject(obj);
    
    // set instance metatable
    lua_getmetatable(L, 1);             // +1
    lua_pushliteral(L, PONDER__LUA_INSTTBLS);       // +1
    lua_rawget(L, -2);                  // -1+1 -> mt
    lua_setmetatable(L, -3);            // -1
    lua_pop(L, 1);
    
    return 1;
}

// Type.__index(key)
static int l_get_class_static(lua_State *L)
{
    // get Class* from class object
    const ponder::Class *cls = *(const ponder::Class**) lua_touserdata(L, 1);
    
    const IdRef key(lua_tostring(L, 2));
    
    const Function *func = nullptr;
    if (cls->tryFunction(key, func))
    {
        lua::impl::FunctionCaller *caller =
            std::get<uses::Uses::eLuaModule>(
                *reinterpret_cast<const uses::Uses::PerFunctionUserData*>(func->getUsesData()));
        
        caller->pushFunction(L);
        return 1;
    }
    
    return 0; // not found
}

//
// Create instance metatable. This is shared between all instances of the class type
//
static void createInstanceMetatable(lua_State *L, const Class& cls)
{
    lua_createtable(L, 0, 3);                   // +1 mt
    
    lua_pushliteral(L, "__index");              // +1
    lua_pushlightuserdata(L, (void*) &cls);     // +1
    lua_pushcclosure(L, l_inst_index, 1);       // 0 +-
    lua_rawset(L, -3);                          // -2

    lua_pushliteral(L, "__newindex");           // +1
    lua_pushlightuserdata(L, (void*) &cls);     // +1
    lua_pushcclosure(L, l_inst_newindex, 1);    // 0 +-
    lua_rawset(L, -3);                          // -2

    lua_pushglobaltable(L);                     // +1
    lua_pushliteral(L, PONDER__LUA_METATBLS);    // +1
    lua_rawget(L, -2);                          // 0 -+
    lua_pushstring(L, cls.name().data());       // +1 k
    lua_pushvalue(L, -4);                       // +1 v
    lua_rawset(L, -3);                          // -2 _G.METAS.CLSNAME = META
    lua_pop(L, 2);                              // -1 _G, _G.METAS
}

} // namespace impl

void expose(lua_State *L, const Class& cls, const IdRef name)
{
    using namespace impl;
    
    // ensure _G.META
    lua_pushglobaltable(L);                     // +1
    lua_pushliteral(L, PONDER__LUA_METATBLS);   // +1
    lua_rawget(L, -2);                          // 0 -+
    if (lua_isnil(L, -1))
    {
        // first time
        lua_pop(L, 1);                              // -1 pop nil
        lua_pushliteral(L, PONDER__LUA_METATBLS);   // +1
        lua_createtable(L, 0, 20);                  // +1
        lua_rawset(L, -3);                          // -2 _G[META] = {}
    }
    lua_pop(L, 1);                              // -1 _G

    // class metatable, used to customise the typename
    lua_createtable(L, 0, 20);                  // +1 metatable
    const int clsmt = lua_gettop(L);

    lua_pushliteral(L, "__call");               // +1 k
    lua_pushcfunction(L, l_instance_create);    // +1 v
    lua_rawset(L, -3);                          // -2 meta.__call = constructor_fn

    lua_pushliteral(L, "__index");              // +1 k
    lua_pushcfunction(L, l_get_class_static);   // +1 v
    lua_rawset(L, -3);                          // -2 meta.__index = get_class_statics

    // create instance metatable. store ref in the class metatable
    lua_pushliteral(L, PONDER__LUA_INSTTBLS);   // +1 k
    createInstanceMetatable(L, cls);            // +1
    lua_rawset(L, -3);                          // -2 meta._inst_ = inst_mt

    // Want in Lua: ClassName(args) -> new instance
    lua_pushglobaltable(L);                     // +1 global table
    lua_pushstring(L, id::c_str(name));         // +1 k
    
    // class proxy
    void *ud = lua_newuserdata(L, sizeof(ponder::Class*)); // +1 v
    *(const ponder::Class**)ud = &cls;
    lua_pushvalue(L, clsmt);                    // +1 metatable
    lua_setmetatable(L, -2);                    // -1
    
    lua_rawset(L, -3);                          // -2 _G[CLASSNAME] = class_obj
    lua_pop(L, 2);                              // -2 global,meta
}

void expose(lua_State *L, const Enum& enm, const IdRef name)
{
    const auto nb = enm.size();
    lua_createtable(L, 0, nb);
    for (std::size_t i=0; i < nb; ++i)
    {
        auto const& p = enm.pair(i);
        lua_pushstring(L, id::c_str(p.name));
        lua_pushinteger(L, p.value);
        lua_settable(L, -3);
    }
    lua_setglobal(L, id::c_str(name));
}

int pushUserObject(lua_State *L, const UserObject& uobj)
{
    Class const& cls = uobj.getClass();
    void *ud = lua_newuserdata(L, sizeof(UserObject));  // +1
    new(ud) UserObject(uobj);
    
    // set instance metatable
    lua_pushglobaltable(L);                     // +1   _G
    lua_pushliteral(L, PONDER__LUA_METATBLS);   // +1
    lua_rawget(L, -2);                          // 0 -+ _G.META
    lua_pushstring(L, cls.name().data());       // +1
    lua_rawget(L, -2);                          // 0 -+ _G_META.MT
    lua_setmetatable(L, -4);                    // -1
    lua_pop(L, 2);
    return 1;
}

bool runString(lua_State *L, const char *luaCode)
{
    const int ret = luaL_dostring(L, luaCode);
    if (ret == LUA_OK)
        return true;
    
    std::printf("Error: %s\n", lua_tostring(L, -1));
    return false; // failed
}

} // lua
} // ponder

#endif // PONDER_USES_LUA_IMPL

#endif // PONDER_USES_LUA_HPP
