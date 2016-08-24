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

#include <ponder/class.hpp>
#include <ponder/detail/format.hpp>
#include <ponder/classbuilder.hpp>
#define PONDER_LUA_IMPL
#include <ponder/uses/lua.hpp>
#include <list>

static_assert(LUA_VERSION_NUM==502, "Expecting Lua 5.2");

#define PLDB(X) X
#define PASSERT(X) if(!(X)) __builtin_trap()

namespace fmt = ponder::detail::fmt;

namespace lib
{
    static const float FLOAT_EPSILON = 1e-5f;
    
    struct Vec
    {
        float   x,y;
        
        Vec()                       : x(0), y(0) {}
        Vec(float x_, float y_)     : x(x_), y(y_) {}
        Vec(const Vec& o)           : x(o.x), y(o.y) {}
        
        bool operator == (const Vec& o) const {
            const float dx = x - o.x, dy = y - o.y;
            return std::abs(dx) < FLOAT_EPSILON && std::abs(dy) < FLOAT_EPSILON;
        }
        
        void set(float x_, float y_) { x = x_, y = y_; }
        
        Vec operator + (const Vec& o) const { return Vec(x + o.x, y + o.y); }
        const Vec& operator += (const Vec& o) { x += o.x, y += o.y; return *this; }
        
        Vec operator * (float s) const { return Vec(x*s, y*s); }
        Vec operator * (const Vec& o) const { return Vec(x * o.x, y * o.y); }

        float length() const        { return std::sqrt(x*x + y*y); }
        
        float dot(const Vec &o) const {
            return x*o.x + y*o.y;
        }
    };

    
    void declare()
    {
        ponder::Class::declare<Vec>()
            .constructor()
            .constructor<float, float>()
            .constructor<const Vec&>()
            .property("x", &Vec::x)
            .property("y", &Vec::y)
            .function("set", &Vec::set)
            .function("add", &Vec::operator+=)
            .function("add2", &Vec::operator+).tag("+")
            .function("length", &Vec::length)
            .function("dot", &Vec::dot)
            ;
    }
    
} // namespace lib

PONDER_TYPE(lib::Vec)

static bool luaTest(lua_State *L, const char *source, bool success = true)
{
    fmt::print("------------------------------------------------------------\n");
    fmt::print("Test{}: {}\n", success ? "" : "(should fail)", source);
    const bool ok = ponder::lua::runString(L, source);
    if (ok != success)
    {
        fmt::print("FAILED");
        exit(EXIT_FAILURE);
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
    ponder::lua::expose<lib::Vec>(L, "Vec2");

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


