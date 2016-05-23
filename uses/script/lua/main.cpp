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

#define LUDB_DECL int db_top(0),zz_ltop=lua_gettop(L),db_d_top(0),db_typei(-1),db_idx(0); \
                const char *db_type(0), *db_valstr(0);
#define LUDB(I) db_top=lua_gettop(L); db_d_top=db_top-zz_ltop; zz_ltop=db_top; \
                db_typei=lua_type(L,I); db_type=lua_typename(L,db_typei); \
                db_valstr=lua_tostring(L,I); db_idx= I>=0 ? I : db_top+I+1

namespace ponder {
namespace lua {

    namespace fmt = ponder::detail::fmt;
    
    static int pushValue(lua_State *L, const ponder::Value& val)
    {
        switch (val.type())
        {
            case boolType:
                lua_pushboolean(L, val.to<bool>());
                return 1;
            case intType:
                lua_pushinteger(L, val.to<int>());
                return 1;
            case realType:
                lua_pushnumber(L, val.to<lua_Number>());
                return 1;
            case stringType:
                lua_pushstring(L, val.to<std::string>().c_str());
                return 1;
//            case enumType:
//                lua_pushinteger(L, val.to<int>());
//                return 1;
//            case arrayType:
//                lua_pushinteger(L, val.to<int>());
//                return 1;
//            case userType:
//                lua_pushinteger(L, val.to<int>());
//                return 1;
            default:
                lua_pushliteral(L, "Unknown type in Ponder value");
                lua_error(L);
        }
        return 0;
    }
    
    static int l_method_call(lua_State *L)
    {
        void *ud = lua_touserdata(L, 1);                // userobj
        if (!ud)
        {
            lua_pushliteral(L, "Method call this is null. (Use Class:method() ?)");
            lua_error(L);
        }
        ponder::UserObject *uobj = (ponder::UserObject*) ud;

        lua_pushvalue(L, lua_upvalueindex(1));
        const Function *func = (const Function *) lua_touserdata(L, -1);
        
        Args args;
        const auto c_argOffset = 2u;
        for (std::size_t nargs = func->argCount(), i = 0; i < nargs; ++i)
        {
            const Type at = func->argType(i);
            switch (at)
            {
                case boolType:
                    args += lua_toboolean(L, i + c_argOffset);
                    break;
                case intType:
                    args += lua_tointeger(L, i + c_argOffset);
                    break;
                case realType:
                    args += lua_tonumber(L, i + c_argOffset);
                    break;
                case stringType:
                    args += lua_tostring(L, i + c_argOffset);
                    break;
                    //            case enumType:
                    //                return 1;
                    //            case arrayType:
                    //                return 1;
                    //            case userType:
                    //                return 1;
                default:
                    lua_pushliteral(L, "Unknown type in method call");
                    lua_error(L);
            }
        }
        
        func->call(*uobj, args);
        
        return 0;
    }

    static int l_inst_index(lua_State *L)   // (obj, key)
    {
        lua_pushvalue(L, lua_upvalueindex(1));
        const Class *cls = (const Class *) lua_touserdata(L, -1);
        
        void *ud = lua_touserdata(L, 1);                // userobj
        const std::string key(lua_tostring(L, 2));
        
        for (auto&& prop : cls->propertyIterator())
        {
            if (prop.name() == key)
            {
                ponder::UserObject *uobj = (ponder::UserObject*) ud;
                ponder::Value val = uobj->get(key);
                return pushValue(L, val);
            }
        }
        
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
    
    static void createInstanceMetatable(lua_State *L, const Class& cls)
    {
        lua_createtable(L, 0, 0);
        
        lua_pushliteral(L, "__index");
        lua_pushlightuserdata(L, (void*) &cls);
        lua_pushcclosure(L, l_inst_index, 1);
        lua_rawset(L, -3);
    }
    
    static int l_class_create(lua_State *L)
    {
        // get Class* from class object
        const ponder::Class *cls = *(const ponder::Class**) lua_touserdata(L, 1);
        
        ponder::Args args;
        const auto c_argOffset = 2u;
        const int nargs = lua_gettop(L) - 1;    // 1st arg is userdata object
        for (int i = c_argOffset; i < c_argOffset + nargs; ++i)
        {
            const int argtype = lua_type(L, i);
            switch (argtype)
            {
                case LUA_TNIL:
                    args += 0;
                    break;
                    
                case LUA_TNUMBER:
                    args += lua_tonumber(L, i);
                    break;
                    
                case LUA_TBOOLEAN:
                    args += lua_toboolean(L, i);
                    break;
                    
                    // LUA_TTABLE, LUA_TFUNCTION, LUA_TUSERDATA, LUA_TTHREAD, and LUA_TLIGHTUSERDATA.
                    
                default:
                    lua_pushstring(L, fmt::format("Argument {0} is bad type {1}",
                                                  i, lua_typename(L, i)).c_str());
                    lua_error(L);
            }
        }
        
        ponder::UserObject obj(cls->construct(args));
        if (obj == ponder::UserObject::nothing)
        {
            lua_pop(L, 1);  // pop new user data
            lua_pushliteral(L, "Matching constructor not found");
            lua_error(L);
        }
        
        void *ud = lua_newuserdata(L, sizeof(ponder::UserObject));   // +1
        new(ud) UserObject(obj);
        
        // set instance metatable
        lua_getmetatable(L, 1);             // +1
        lua_pushliteral(L, "_inst_");       // +1
        lua_rawget(L, -2);                  // -1+1 -> mt
        lua_setmetatable(L, -3);            // -1
        lua_pop(L, 1);
        
        return 1;
    }
    
    void expose(lua_State *L, const Class& cls, const std::string& name)
    {
        // class metatable
        lua_createtable(L, 0, 0);                   // +1 metatable
        const int clsmt = lua_gettop(L);

        lua_pushliteral(L, "__call");               // +1 k
        lua_pushcfunction(L, l_class_create);       // +1 v
        lua_rawset(L, -3);                          // -2 meta.__call = constructor_fn
        
        // create instance metatable
        lua_pushliteral(L, "_inst_");               // +1 k
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
        
        bool operator == (const Vec2f& o) const {
            const float dx = x - o.x, dy = y - o.y;
            return std::abs(dx) < FLOAT_EPSILON && std::abs(dy) < FLOAT_EPSILON;
        }
        
        void set(float x_, float y_) { x = x_, y = y_; }
        
        static const Vec2f c_zero;
        static const Vec2f c_x;
        static const Vec2f c_y;
        
        Vec2f operator + (const Vec2f& o) const { return Vec2f(x + o.x, y + o.y); }
        void operator += (const Vec2f& o) { x += o.x, y += o.y; }
        
        Vec2f operator - () const { return Vec2f(-x, -y); }
        Vec2f operator - (const Vec2f& o) const { return Vec2f(x - o.x, y - o.y); }
        void operator -= (const Vec2f& o) { x -= o.x, y -= o.y; }
        
        Vec2f operator * (float s) const { return Vec2f(x*s, y*s); }
        void operator *= (float s) { x *= s, y *= s; }
        Vec2f operator * (const Vec2f& o) const { return Vec2f(x * o.x, y * o.y); }
        void operator *= (const Vec2f& o) { x *= o.x, y *= o.y; }
        
        Vec2f operator / (float s) const { return Vec2f(x/s, y/s); }
        void operator /= (float s) { x /= s, y /= s; }
        
        bool isNull() const { return *this == c_zero; }
        
        float length() const        { return std::sqrt(x*x + y*y); }
        float lengthSquared() const { return x*x + y*y; }
        
        float dot(const Vec2f &o) const {
            return x*o.x + y*o.y;
        }
        
        float normalise()
        {
            const float vlen = length();
            const float invlen = 1.f / vlen;
            x *= invlen;
            y *= invlen;
            return vlen;    // Old length.
        }
        
        float setLength(float newLen)
        {
            const float vlen = length();
            const float scale = newLen / vlen;
            x *= scale;
            y *= scale;
            return vlen;    // Old length.
        }
        
//        static Vec2f polar(float rad);               // anti-clkws from x-axis.
//        static Vec2f compass(float degrees);         // clkws from north.
//        
//        // Rotate vector origin anti-clkws by rad.
//        void rotate(float aclkRad);
//        
//        // Rotate around centre point anti-clkws by rad.
//        void rotateAround(const Vec2f &centre, float aclkRad);
    };

    
    void declare()
    {
        ponder::Class::declare<Vec2f>()
            .constructor()
            .constructor<float, float>()
            .property("x", &Vec2f::x)
            .property("y", &Vec2f::y)
            .function("set", &Vec2f::set)
//            .function("add", &Vec2f::operator +)
//            .function("subtract", (Vec2f::*(const Vec2f&)) &Vec2f::operator -)
            ;
    }
    
} // namespace lib

PONDER_TYPE(lib::Vec2f)

#define QUOTED(T) #T

int main()
{
    fmt::print("Lua version {}\n", LUA_VERSION);
    
    lua_State *L = luaL_newstate();
    luaopen_base(L);
    
    lib::declare();
    ponder::lua::expose(L, ponder::classByType<lib::Vec2f>(), "Vec2");
    
    const char *tstr =
    QUOTED(
        print(Vec2)
        v = Vec2(7,8)
        print(v)
        print(v.x, v.y)
        v:set(2,3)
        print(v.x, v.y)
    );
    
    ponder::lua::runString(L, tstr);
    
    return EXIT_SUCCESS;
}

