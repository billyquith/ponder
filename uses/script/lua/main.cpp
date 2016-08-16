/****************************************************************************
 **
 ** This file is part of the Ponder library.
 **
 ** The MIT License (MIT)
 **
 ** Copyright (C) 2016 Billy Quith.
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

//----------------------------------------------------------------------------

#include <ponder/class.hpp>
#include <ponder/detail/format.hpp>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

static_assert(LUA_VERSION_NUM==502, "Expecting Lua 5.2");

#define LUDB_DECL \
    int db_top(0),zz_ltop=lua_gettop(L),db_d_top(0),db_idx(0); \
    std::string zz_stk; \
    const char *db_stack(0);

#define LUDB(I) \
    db_top=lua_gettop(L); db_d_top=db_top-zz_ltop; zz_ltop=db_top; \
    db_idx= I>=0 ? I : db_top+I+1; \
    do { zz_stk=""; int i=db_top; while(i > 0){ \
    zz_stk += "  " + std::to_string(i) + ":" + lua_typename(L,lua_type(L,i)); \
    const char *vs = lua_tostring(L,i); \
    if (vs) zz_stk += " <" + std::string(vs) + ">"; \
    --i; }} while(false); db_stack=zz_stk.c_str();

#define PLDB(X) X
#define PASSERT(X) if(!(X)) __builtin_trap()

namespace ponder {
namespace lua {

    namespace fmt = ponder::detail::fmt;
    
    #define PONDER_LUA_METATBLS "_ponder_meta"
    #define PONDER_LUA_INSTTBLS "_instmt"
    
    // push a Ponder value onto the Lua state stack
    static int pushValue(lua_State *L, const ponder::Value& val)
    {
        switch (val.type())
        {
            case ValueType::Boolean:
                lua_pushboolean(L, val.to<bool>());
                return 1;
                
            case ValueType::Integer:
                lua_pushinteger(L, val.to<lua_Integer>());
                return 1;
                
            case ValueType::Real:
                lua_pushnumber(L, val.to<lua_Number>());
                return 1;
                
            case ValueType::String:
                lua_pushstring(L, val.to<std::string>().c_str());
                return 1;

            case ValueType::Enum:
                lua_pushinteger(L, val.to<int>());
                return 1;
                
//            case ValueType::Array:
//                lua_pushinteger(L, val.to<int>());
//                return 1;
                
            case ValueType::User:
                {
                    UserObject vuobj = val.to<UserObject>();
                    Class const& cls = vuobj.getClass();
                    void *ud = lua_newuserdata(L, sizeof(UserObject));  // +1
                    new(ud) UserObject(vuobj);

                    // set instance metatable
                    lua_pushglobaltable(L);                     // +1   _G
                    lua_pushliteral(L, PONDER_LUA_METATBLS);    // +1
                    lua_rawget(L, -2);                          // 0 -+ _G.META
                    lua_pushstring(L, cls.name().data());       // +1
                    lua_rawget(L, -2);                          // 0 -+ _G_META.MT
                    lua_setmetatable(L, -4);                    // -1
                    lua_pop(L, 2);
                }
                return 1;
            default:
                lua_pushliteral(L, "Unknown type in Ponder value");
                lua_error(L);
        }
        return 0;
    }
    
    // get a Lua stack value as a Ponder value
    static Value getValue(lua_State *L, int index, ValueType typeExpected = ValueType::None)
    {
        if (index > lua_gettop(L))
        {
            lua_pushliteral(L, "Trying to get arg off stack end");
            lua_error(L);
        }
        
        const int typei = lua_type(L, index);

        // if we expect a type then override Lua type to force an error if incorrect
        if (typeExpected != ValueType::None)
        {
            int vtype = LUA_TNIL;
            switch (typeExpected) {
                case ValueType::Boolean:
                    vtype = LUA_TBOOLEAN;
                    break;
                    
                case ValueType::String:
                    vtype = LUA_TSTRING;
                    break;
                    
                case ValueType::Real:
                case ValueType::Integer:
                    vtype = LUA_TNUMBER;
                    break;
                    
                case ValueType::User:
                    vtype = LUA_TUSERDATA;
                    break;
                    
                default: ;
            }
            if (vtype != typei)
            {
                lua_pushfstring(L, "Expecting %s but got %s",
                                lua_typename(L, vtype), lua_typename(L, typei));
                lua_error(L);
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
                lua_pushfstring(L, "Cannot convert %s to Ponder value", lua_typename(L, typei));
                lua_error(L);
        }
        
        return Value(); // no value
    }

    static int l_method_call(lua_State *L)
    {
        void *ud = lua_touserdata(L, 1);  // userobj
        if (!ud)
        {
            lua_pushliteral(L, "Method call 'this' is null. (Use Class:method() ?)");
            lua_error(L);
        }
        ponder::UserObject *uobj = (ponder::UserObject*) ud;

        lua_pushvalue(L, lua_upvalueindex(1));
        const Function *func = (const Function *) lua_touserdata(L, -1);
        
        Args args;
        constexpr auto c_argOffset = 2u;
        for (std::size_t nargs = func->argCount(), i = 0; i < nargs; ++i)
        {
            // we know the arg type so check it
            const ValueType at = func->argType(i);
            args += getValue(L, i + c_argOffset, at);
        }
        
        Value ret = func->call(*uobj, args);
        if (ret.type() != ValueType::None)
            return pushValue(L, ret);
        
        return 0;
    }

    static int l_inst_index(lua_State *L)   // (obj, key) -> obj[key]
    {
        lua_pushvalue(L, lua_upvalueindex(1));
        const Class *cls = (const Class *) lua_touserdata(L, -1);
        
        void *ud = lua_touserdata(L, 1);                // userobj
        const std::string key(lua_tostring(L, 2));
        
        // check if getting property value
        for (auto&& prop : cls->propertyIterator())
        {
            if (prop.name() == key)
            {
                ponder::UserObject *uobj = (ponder::UserObject*) ud;
                ponder::Value val = uobj->get(key);
                return pushValue(L, val);
            }
        }
        
        // check if calling function object
        for (auto&& func : cls->functionIterator())
        {
            if (func.name() == key)
            {
                lua_pushlightuserdata(L, (void*) func.value().get());
                lua_pushcclosure(L, l_method_call, 1);
                return 1;
            }
        }
        
        return 0;
    }

    static int l_inst_newindex(lua_State *L)   // (obj, key, value) obj[key] = value
    {
        lua_pushvalue(L, lua_upvalueindex(1));
        const Class *cls = (const Class *) lua_touserdata(L, -1);
        
        void *ud = lua_touserdata(L, 1);                // userobj
        const std::string key(lua_tostring(L, 2));
        
        // check if assigning to a property
        for (auto&& prop : cls->propertyIterator())
        {
            if (prop.name() == key)
            {
                ponder::UserObject *uobj = (ponder::UserObject*) ud;
                const Property *pprop = prop.value().get();
                pprop->set(*uobj, getValue(L, 3, pprop->type()));
            }
        }
        
        return 0;
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
        lua_pushliteral(L, PONDER_LUA_METATBLS);    // +1
        lua_rawget(L, -2);                          // 0 -+
        lua_pushstring(L, cls.name().data());       // +1 k
        lua_pushvalue(L, -4);                       // +1 v
        lua_rawset(L, -3);                          // -2 _G.METAS.CLSNAME = META
        lua_pop(L, 2);                              // -1 _G, _G.METAS
    }
    
    static int l_instance_create(lua_State *L)
    {
        // get Class* from class object
        const ponder::Class *cls = *(const ponder::Class**) lua_touserdata(L, 1);
        
        ponder::Args args;
        constexpr auto c_argOffset = 2u;  // 1st arg is userdata object
        const int nargs = lua_gettop(L) - (c_argOffset-1);
        for (int i = c_argOffset; i < c_argOffset + nargs; ++i)
        {
            // there may be multiple constructors so don't check types
            args += getValue(L, i);
        }
        
        ponder::UserObject obj(cls->construct(args));
        if (obj == ponder::UserObject::nothing)
        {
            lua_pop(L, 1);  // pop new user data
            lua_pushliteral(L, "Matching constructor not found");
            lua_error(L);
        }
        
        void *ud = lua_newuserdata(L, sizeof(UserObject));   // +1
        new(ud) UserObject(obj);
        
        // set instance metatable
        lua_getmetatable(L, 1);             // +1
        lua_pushliteral(L, PONDER_LUA_INSTTBLS);       // +1
        lua_rawget(L, -2);                  // -1+1 -> mt
        lua_setmetatable(L, -3);            // -1
        lua_pop(L, 1);
        
        return 1;
    }
    
    void expose(lua_State *L, const Class& cls, const std::string& name)
    {
        // ensure _G.META
        lua_pushglobaltable(L);                     // +1
        lua_pushliteral(L, PONDER_LUA_METATBLS);    // +1
        lua_rawget(L, -2);                          // 0 -+
        if (lua_isnil(L, -1))
        {
            // first time
            lua_pop(L, 1);                              // -1 pop nil
            lua_pushliteral(L, PONDER_LUA_METATBLS);    // +1
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
        
        // create instance metatable. store ref in the class metatable
        lua_pushliteral(L, PONDER_LUA_INSTTBLS);       // +1 k
        createInstanceMetatable(L, cls);            // +1
        lua_rawset(L, -3);                          // -2 meta._inst_ = inst_mt

        // Want in Lua: ClassName(args) -> new instance
        lua_pushglobaltable(L);                     // +1 global table
        lua_pushstring(L, name.c_str());            // +1 k
        
        // class proxy
        void *ud = lua_newuserdata(L, sizeof(ponder::Class*)); // +1 v
        *(const ponder::Class**)ud = &cls;
        lua_pushvalue(L, clsmt);                    // +1 metatable
        lua_setmetatable(L, -2);                    // -1
        
        lua_rawset(L, -3);                          // -2 _G[CLASSNAME] = class_obj
        lua_pop(L, 2);                              // -2 global,meta
    }

    bool runString(lua_State *L, const char *luaCode)
    {
        const int ret = luaL_dostring(L, luaCode);
        if (ret == LUA_OK)
            return true;
        
        fmt::print("Error: {}\n", lua_tostring(L, -1));
        return false; // failed
    }
    
}} // ponder::lua

//----------------------------------------------------------------------------

#include <ponder/classbuilder.hpp>
#include <list>

namespace fmt = ponder::detail::fmt;

namespace lib
{
    static const float FLOAT_EPSILON = 1e-5f;
    
    struct Vec2f
    {
        float   x,y;
        
        Vec2f()                         : x(0), y(0) {}
        Vec2f(float x_, float y_)       : x(x_), y(y_) {}
        Vec2f(const Vec2f& o)           : x(o.x), y(o.y) {}
        
        bool operator == (const Vec2f& o) const {
            const float dx = x - o.x, dy = y - o.y;
            return std::abs(dx) < FLOAT_EPSILON && std::abs(dy) < FLOAT_EPSILON;
        }
        
        void set(float x_, float y_) { x = x_, y = y_; }
        
        Vec2f operator + (const Vec2f& o) const { return Vec2f(x + o.x, y + o.y); }
        const Vec2f& operator += (const Vec2f& o) { x += o.x, y += o.y; return *this; }
        
        Vec2f operator - () const { return Vec2f(-x, -y); }
        Vec2f operator - (const Vec2f& o) const { return Vec2f(x - o.x, y - o.y); }
        void operator -= (const Vec2f& o) { x -= o.x, y -= o.y; }
        
        Vec2f operator * (float s) const { return Vec2f(x*s, y*s); }
        Vec2f operator * (const Vec2f& o) const { return Vec2f(x * o.x, y * o.y); }
        
        Vec2f operator / (float s) const { return Vec2f(x/s, y/s); }
        void operator /= (float s) { x /= s, y /= s; }
        
        float length() const        { return std::sqrt(x*x + y*y); }
        
        float dot(const Vec2f &o) const {
            return x*o.x + y*o.y;
        }
    };

    
    void declare()
    {
        ponder::Class::declare<ponder::UserObject>()
            ;
        
        ponder::Class::declare<Vec2f>()
            .constructor()
            .constructor<float, float>()
            .constructor<const Vec2f&>()
            .property("x", &Vec2f::x)
            .property("y", &Vec2f::y)
            .function("set", &Vec2f::set)
            .function("length", &Vec2f::length)
            .function("dot", &Vec2f::dot)
            .function("add", &Vec2f::operator+=)
            .function("add2", &Vec2f::operator+).tag("+")
            ;
    }
    
} // namespace lib

PONDER_TYPE(ponder::UserObject)
PONDER_TYPE(lib::Vec2f)

static bool luaTest(lua_State *L, const char *source, bool success = true)
{
    fmt::print("Test{}: {}\n", success ? "" : "(fail)", source);
    const bool ok = ponder::lua::runString(L, source);
    if (ok != success)
    {
        fmt::print("FAILED");
        exit(1);
        return false;
    }
    fmt::print("\n");
    return true;
}

int main()
{
    fmt::print("Lua version {}\n", LUA_VERSION);
    
    lua_State *L = luaL_newstate();
    luaopen_base(L);
    
    lib::declare();
    ponder::lua::expose(L, ponder::classByType<lib::Vec2f>(), "Vec2");

    // class defined
    luaTest(L, "print(Vec2); assert(Vec2 ~= nil)");

    // instance
    luaTest(L, "v = Vec2(); assert(v ~= nil)");
    luaTest(L, "assert(type(v) == 'userdata')");
    
    // property read
    luaTest(L, "assert(v.x == 0)");
    luaTest(L, "assert(v.y == 0)");

    // property write
    luaTest(L, "v.x = 7; assert(v.x == 7)");
    luaTest(L, "v.y = -3; assert(v.y == -3)");
    luaTest(L, "v.x = 1.25");
    luaTest(L, "v.x = 1.76e6");
    luaTest(L, "v.x = 'fail'", false);

    // method call with args
    luaTest(L, "v:set(12, 8.5); assert(v.x == 12 and v.y == 8.5)");
    luaTest(L, "v:set('fail'); print(v.x, v.y)", false);
    luaTest(L, "v:set(); print(v.x, v.y)", false);

    // method call return args
    luaTest(L, "l = Vec2(3,0); assert(l:length() == 3)");

    // method call with object arg
    luaTest(L, "a,b = Vec2(2,3), Vec2(3,4); c = a:dot(b); print(c); assert(c ~= nil)");
    
    // method call with return object
    luaTest(L, "c = a:add(b); assert(c ~= nil); print(c.x, c.y);");
    
    return EXIT_SUCCESS;
}


