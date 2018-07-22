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
#ifndef PONDER_USES_LUA_IMPL_HPP
#define PONDER_USES_LUA_IMPL_HPP

#if PONDER_USING_LUA

#define PONDER_IF_LUA(...) __VA_ARGS__

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

// forward declare
namespace ponder { namespace lua {
    int pushUserObject(lua_State *L, const ponder::UserObject& uobj);
}}

//-----------------------------------------------------------------------------

namespace ponder_ext {
    
using namespace ponder;

inline UserObject* toUserObject(lua_State *L, int index)
{
    return reinterpret_cast<UserObject*>(lua_touserdata(L, index));
}
    
struct LuaTable { lua_State *L; };

//-----------------------------------------------------------------------------
// Convert Lua call arguments to C++ types.

template <typename P, typename U = void> struct LuaValueReader {};

template <typename P>
struct  LuaValueReader<P, typename std::enable_if<std::is_integral<P>::value>::type>
{
    typedef P ParamType;
    static inline ParamType convert(lua_State* L, std::size_t index)
    {
        return luaL_checkinteger(L, index);
    }
};

template <typename P>
struct LuaValueReader<P, typename std::enable_if<std::is_floating_point<P>::value>::type>
{
    typedef P ParamType;
    static inline ParamType convert(lua_State* L, std::size_t index)
    {
        return luaL_checknumber(L, index);
    }
};

template <typename P>
struct LuaValueReader<P, typename std::enable_if<std::is_enum<P>::value>::type>
{
    typedef P ParamType;
    static inline ParamType convert(lua_State* L, std::size_t index)
    {
        const lua_Integer i = luaL_checkinteger(L, index);
        return static_cast<P>(i);
    }
};

template <typename P>
struct LuaValueReader<P,
    typename std::enable_if<std::is_same<std::string,
                            typename detail::RawType<P>::Type>::value>::type>
{
    typedef std::string ParamType;
    static inline ParamType convert(lua_State* L, std::size_t index)
    {
        return ParamType(luaL_checkstring(L, index));
    }
};

template <>
struct LuaValueReader<ponder::detail::string_view>
{
    typedef ponder::detail::string_view ParamType;
    static inline ParamType convert(lua_State* L, std::size_t index)
    {
        return ParamType(luaL_checkstring(L, index));
    }
};

template <typename P>
struct LuaValueReader<P&, typename std::enable_if<detail::IsUserType<P>::value>::type>
{
    typedef P& ParamType;
    typedef typename detail::RawType<ParamType>::Type RawType;
    
    static inline ParamType convert(lua_State* L, std::size_t index)
    {
        if (!lua_isuserdata(L, index))
        {
            luaL_error(L, "Argument %d: expecting user data", index);
        }
        
        UserObject *uobj = reinterpret_cast<UserObject*>(lua_touserdata(L, index));        
        return uobj->ref<RawType>();
    }
};

template <typename P>
struct LuaValueReader<P*, typename std::enable_if<ponder::detail::IsUserType<P>::value>::type>
{
    typedef P* ParamType;
    typedef typename ponder::detail::RawType<ParamType>::Type RawType;
    
    static inline ParamType convert(lua_State* L, std::size_t index)
    {
        if (!lua_isuserdata(L, index))
        {
            luaL_error(L, "Argument %d: expecting user data", index);
        }
        
        UserObject *uobj = reinterpret_cast<UserObject*>(lua_touserdata(L, index));        
        return &uobj->ref<RawType>();
    }
};

// User function wants to parse a Lua table.
template <>
struct LuaValueReader<LuaTable>
{
    typedef LuaTable ParamType;
    static inline ParamType convert(lua_State* L, std::size_t index)
    {
        luaL_checktype(L, index, LUA_TTABLE);
        LuaTable t = {L};
        return t;
    }
};

//-----------------------------------------------------------------------------
// Write values to Lua. Push to stack.

template <typename T, typename U = void> struct LuaValueWriter {};

template <typename T>
struct LuaValueWriter<T, typename std::enable_if<std::is_integral<T>::value>::type>
{
    static inline int push(lua_State *L, T value)
    {
        return lua_pushinteger(L, value), 1;
    }
};

template <typename T>
struct LuaValueWriter<T, typename std::enable_if<std::is_floating_point<T>::value>::type>
{
    static inline int push(lua_State *L, T value)
    {
        return lua_pushnumber(L, value), 1;
    }
};

template <>
struct LuaValueWriter<std::string>
{
    static inline int push(lua_State *L, const std::string& value)
    {
        return lua_pushstring(L, value.data()), 1;
    }
};

template <>
struct LuaValueWriter<const char*>          // non-const object
{
    static inline int push(lua_State *L, const char* value)
    {
        return lua_pushstring(L, value), 1;
    }
};

template <>
struct LuaValueWriter<const char* const>    // const object
{
    static inline int push(lua_State *L, const char* value)
    {
        return lua_pushstring(L, value), 1;
    }
};

template <>
struct LuaValueWriter<ponder::detail::string_view>
{
    static inline int push(lua_State *L, const ponder::detail::string_view& value)
    {
        return lua_pushstring(L, value.data()), 1;
    }
};

template <>
struct LuaValueWriter<UserObject>
{
    static inline int push(lua_State *L, const UserObject& value)
    {
        return ponder::lua::pushUserObject(L, value);
    }
};

// Return tuples as individual elements. tuple<x,y> -> x,y
template <typename... R>
struct LuaValueWriter<const std::tuple<R...>>
{
    template <size_t... Is>
    static inline void pushElements(lua_State *L,
                                    std::tuple<R...> const& value,
                                    PONDER__SEQNS::index_sequence<Is...>)
    {
        const int r[sizeof...(R)] = { LuaValueWriter<R>::push(L, std::get<Is>(value))... };
        (void)r;
    }
    
    static inline int push(lua_State *L, std::tuple<R...> const& value)
    {
        typedef PONDER__SEQNS::make_index_sequence<sizeof...(R)> Enumerator;
        pushElements(L, value, Enumerator());
        return sizeof...(R);
    }
};
    
// Non-const tuples are const to us.
template <typename... R>
struct LuaValueWriter<std::tuple<R...>> : public LuaValueWriter<const std::tuple<R...>> {};
    
} // namespace ponder_ext

namespace ponder {
namespace lua {
namespace impl {
    
using namespace ponder_ext;

//-----------------------------------------------------------------------------
// Handle returning multiple values

template <typename R, typename U = void> struct CallReturnMultiple;

template <typename R>
struct CallReturnMultiple<R>
{
    static inline int value(lua_State *L, R&& o) {return LuaValueWriter<R>::push(L, o);}
};
    
//-----------------------------------------------------------------------------
// Handle returning copies

template <typename R, typename U = void> struct CallReturnCopy;

template <typename R>
struct CallReturnCopy<R, typename std::enable_if<!detail::IsUserType<R>::value>::type>
{
    // "no member named push" error here means the type returned is not covered.
    static inline int value(lua_State *L, R&& o) {return LuaValueWriter<R>::push(L, o);}
};

template <typename R>
struct CallReturnCopy<R, typename std::enable_if<detail::IsUserType<R>::value>::type>
{
    static inline int value(lua_State *L, R&& o)
    {return LuaValueWriter<UserObject>::push(L, UserObject::makeCopy(std::forward<R>(o)));}
};

//-----------------------------------------------------------------------------
// Handle returning internal references

template <typename R, typename U = void> struct CallReturnInternalRef;

template <typename R>
struct CallReturnInternalRef<R,
    typename std::enable_if<
        !detail::IsUserType<R>::value
        && !std::is_same<typename detail::RawType<R>::Type, UserObject>::value
    >::type>
{
    static inline int value(lua_State *L, R&& o) {return LuaValueWriter<R>::push(L, o);}
};

template <typename R>
struct CallReturnInternalRef<R,
    typename std::enable_if<
        detail::IsUserType<R>::value
        || std::is_same<typename detail::RawType<R>::Type, UserObject>::value
    >::type>
{
    static inline int value(lua_State *L, R&& o)
    {
        return LuaValueWriter<UserObject>::push(L, UserObject::makeRef(std::forward<R>(o)));
    }
};

//-----------------------------------------------------------------------------
// Choose which returner to use, based on policy
//  - map policy kind to actionable policy type

template <typename Policies_t, typename R> struct ChooseCallReturner;

template <typename... Ps, typename R>
struct ChooseCallReturner<std::tuple<policy::ReturnCopy, Ps...>, R>
{
    typedef CallReturnCopy<R> type;
};

template <typename... Ps, typename R>
struct ChooseCallReturner<std::tuple<policy::ReturnInternalRef, Ps...>, R>
{
    typedef CallReturnInternalRef<R> type;
};

template <typename... Ps, typename R>
struct ChooseCallReturner<std::tuple<policy::ReturnMultiple, Ps...>, R>
{
    typedef CallReturnMultiple<R> type;
};

template <typename R>
struct ChooseCallReturner<std::tuple<>, R> // default
{
    typedef CallReturnCopy<R> type;
};

template <typename P, typename... Ps, typename R>
struct ChooseCallReturner<std::tuple<P, Ps...>, R> // recurse
{
    typedef typename ChooseCallReturner<std::tuple<Ps...>, R>::type type;
};

//-----------------------------------------------------------------------------
// Object function call helper to allow specialisation by return type. Applies policies.

template <typename P>
struct ConvertArgs
{
    typedef LuaValueReader<P> Convertor;
    
    static typename Convertor::ParamType convert(lua_State* L, std::size_t index)
    {
        return Convertor::convert(L, index+1);
    }
};

template <typename R, typename FTraits, typename FPolicies>
class CallHelper
{
public:
    
    template<typename F, typename... A, size_t... Is>
    static int call(F func, lua_State* L, PONDER__SEQNS::index_sequence<Is...>)
    {
        typedef typename ChooseCallReturner<FPolicies, R>::type CallReturner;
        return CallReturner::value(L, func(ConvertArgs<A>::convert(L, Is)...));
    }
};

// Specialization of CallHelper for functions returning void
template <typename FTraits, typename FPolicies>
class CallHelper<void, FTraits, FPolicies>
{
public:
    
    template<typename F, typename... A, size_t... Is>
    static int call(F func, lua_State* L, PONDER__SEQNS::index_sequence<Is...>)
    {
        func(ConvertArgs<A>::convert(L, Is)...);
        return 0; // return nil
    }
};

//-----------------------------------------------------------------------------
// Convert traits to callable function wrapper. Generic for all function types.

template <typename R, typename P> struct FunctionWrapper;

template <typename R, typename... P> struct FunctionWrapper<R, std::tuple<P...>>
{
    typedef typename std::function<R(P...)> Type;
    
    template <typename F, typename FTraits, typename FPolicies>
    static int call(F func, lua_State* L)
    {
        typedef PONDER__SEQNS::make_index_sequence<sizeof...(P)> ArgEnumerator;
        
        return CallHelper<R, FTraits, FPolicies>::template call<F, P...>(func, L, ArgEnumerator());
    }
};

//-----------------------------------------------------------------------------
// Base for runtime function caller

class FunctionCaller
{
public:
    FunctionCaller(const IdRef name, int (*fn)(lua_State*) = nullptr)
        :   m_name(name)
        ,   m_luaFunc(fn)
    {}
    
    FunctionCaller(const FunctionCaller&) = delete; // no copying
    virtual ~FunctionCaller() {}
    
    const IdRef name() const { return m_name; }

    void pushFunction(lua_State* L)
    {
        lua_pushlightuserdata(L, (void*) this);
        lua_pushcclosure(L, m_luaFunc, 1);
    }
    
private:
    const IdRef m_name;
    int (*m_luaFunc)(lua_State*);
};

// The FunctionImpl class is a template which is specialized according to the
// underlying function prototype.
template <typename F, typename FTraits, typename FPolicies>
class FunctionCallerImpl : public FunctionCaller
{
public:
    
    FunctionCallerImpl(IdRef name, F function)
    :   FunctionCaller(name, &call)
    ,   m_function(function)
    {}
    
private:
    
    typedef FunctionCallerImpl<F, FTraits, FPolicies> ThisType;
    
    typedef typename FTraits::Details::FunctionCallTypes CallTypes;
    typedef FunctionWrapper<typename FTraits::ReturnType, CallTypes> DispatchType;
    
    typename DispatchType::Type m_function; // Object containing the actual function to call
    
    static int call(lua_State *L)
    {
        lua_pushvalue(L, lua_upvalueindex(1));
        ThisType *self = reinterpret_cast<ThisType*>(lua_touserdata(L, -1));
        lua_pop(L, 1);

        return DispatchType::template
            call<decltype(m_function), FTraits, FPolicies>(self->m_function, L);
    }
};
    
} // namespace impl
} // namespace lua
} // namespace ponder

#else // !PONDER_USING_LUA
#   define PONDER_IF_LUA(...)
#endif

#endif // PONDER_USES_LUA_IMPL_HPP
