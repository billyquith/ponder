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

namespace ponder {
namespace lua {

    namespace fmt = ponder::detail::fmt;

    static int l_class_create(lua_State *L)
    {
        lua_pushvalue(L, lua_upvalueindex(1));
        const ponder::Class *cls = (const ponder::Class*) lua_touserdata(L, -1);
        
        void *ud = lua_newuserdata(L, cls->sizeOf());
        
        ponder::UserObject obj(cls->construct(ponder::Args(), ud));
        if (obj == ponder::UserObject::nothing)
        {
            lua_pop(L, 1);  // pop new user data
            lua_pushliteral(L, "Matching constructor not found");
            lua_error(L);
        }
        
        return 1;
    }
    
    void expose(lua_State *L, const Class& cls, const std::string& name)
    {
        // class metatable
        lua_createtable(L, 0, 0);                   // +1 metatable
        const int mtidx = lua_gettop(L);
        lua_pushliteral(L, "__call");               // +1 k
        lua_pushlightuserdata(L, (void*) &cls);     // +1
        lua_pushcclosure(L, l_class_create, 1);     //  0 = +1-1 v
        lua_rawset(L, -3);                          // -2 meta.__call = fn

        // Want in Lua: ClassName(args) -> new instance
        lua_pushglobaltable(L);                     // +1 global table
        lua_pushstring(L, name.c_str());            // +1 k
        
        void *ud = lua_newuserdata(L, sizeof(ponder::Class*)); // +1 v
        *(const ponder::Class**)ud = &cls;
        lua_pushvalue(L, mtidx);                    // +1 metatable
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
            .function("add", &Vec2f::operator +)
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
    
    const char *tstr = QUOTED(
        print(Vec2)
        v = Vec2()
        print(v)
    );
    
    ponder::lua::runString(L, tstr);
    
    return EXIT_SUCCESS;
}

