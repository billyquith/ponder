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

#define PONDER_USES_LUA_IMPL
#define PONDER_USES_RUNTIME_IMPL
#include <ponder/class.hpp>
#include <ponder/classbuilder.hpp>
#include <ponder/uses/lua.hpp>
#include <list>
#include <cstdio>
#include <cmath>

extern "C" {
#include <lualib.h>
}

static_assert(LUA_VERSION_NUM==503, "Expecting Lua 5.3");

#define PLDB(X) X
#define PASSERT(X) if(!(X)) __builtin_trap()

namespace ponder_ext
{
    template <>
    struct ValueMapper<ponder::detail::string_view>
    {
        static const ponder::ValueKind kind = ponder::ValueKind::String;
        
        // convert to ponder::String
        static ponder::String to(const ponder::detail::string_view& source)
        {
            return ponder::String(source);
        }

        // convert to string_view from ponder type
        template <typename T>
        static ponder::detail::string_view from(const T& source)
        {
            return ponder::detail::string_view(ValueMapper<ponder::String>::from(source));
        }
    };
}

namespace lib
{
    static constexpr float FLOAT_EPSILON = 1e-5f;
    
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
        
        std::tuple<float,float> get() const { return std::make_tuple(x,y); }

        void set(float x_, float y_) { x = x_, y = y_; }
        
        Vec operator + (const Vec& o) const { return Vec(x + o.x, y + o.y); }
        const Vec& operator += (const Vec& o) { x += o.x, y += o.y; return *this; }
        
        float length() const        { return std::sqrt(x*x + y*y); }
        
        float dot(const Vec &o) const {
            return x*o.x + y*o.y;
        }
        
        static Vec up() { return Vec(0, 1.f); }     // static function
        
        Vec& ref() { return *this; }                // return ref
    };
    
    
    struct Holder
    {
        Holder() = default;
        Holder(const Holder&) = delete;              
        
        Vec vec;
        
        Holder* ptrRef() { return this; }
        Holder& refRef() { return *this; }
    };
    
    struct Types
    {
        struct Obj {
            Obj(const Obj&) = delete;
        } o;
        
        static int len(const ponder::detail::string_view str) { return str.length(); }
        
        Obj* retPtr() { return &o; }
        void passPtr(Obj *po) {}
        
        static const char* getString() { return "blah"; }
    };
    
    struct Dummy
    {
        static int halve(int x) { return x/2; }
    };
    int twice(int x) { return 2*x; }
    
    enum class Colour { Red, Green, Blue };
    
    struct Parsing
    {
        int a;
        std::string b;
        
        void init(ponder_ext::LuaTable lt)
        {
            assert(lua_istable(lt.L, -1)); // should be guaranteed by conversion
            
            lua_getfield(lt.L, -1, "a");
            a = luaL_checknumber(lt.L, -1);
            lua_pop(lt.L, 1);
            
            lua_getfield(lt.L, -1, "b");
            b = luaL_checkstring(lt.L, -1);
            lua_pop(lt.L, 1);
        }
    };
    
    void declare()
    {
        using namespace ponder;
    
        ponder::Class::declare<Vec>()
            .constructor()
            .constructor<float, float>()
            .constructor<const Vec&>()
            .property("x", &Vec::x)
            .property("y", &Vec::y)
            .function("get", &Vec::get, policy::ReturnMultiple()) // tuple
            .function("set", &Vec::set)
            .function("add", &Vec::operator+=)
            .function("add2", &Vec::operator+) //.tag("+")
            .function("length", &Vec::length)
            .function("dot", &Vec::dot)
        
            .function("up", &Vec::up)   // static
        
            .function("funcRef", &Vec::ref, policy::ReturnInternalRef())  // ref function
            .property("propRef", &Vec::ref)     // ref property
            ;

        ponder::Class::declare<std::tuple<float,float>>();

        ponder::Class::declare<Holder>()
            .constructor()
            //  .property("pref", &Holder::ptrRef) // TODO - fix for self ref pointers
            .function("rref", &Holder::refRef, policy::ReturnInternalRef())
            .property("vec", &Holder::vec)
            ;

        ponder::Class::declare<Types>()
            .function("len", &Types::len)
            .function("retp", &Types::retPtr, policy::ReturnInternalRef())
            .function("passp", &Types::passPtr)
            .function("getStr", &Types::getString)
            ;

        ponder::Class::declare<Dummy>()
            .function("halve", &Dummy::halve)
            .function("twice", &twice)
            ;

        ponder::Enum::declare<Colour>()
            .value("red", Colour::Red)
            .value("green", Colour::Green)
            .value("blue", Colour::Blue)
            ;
        
        ponder::Class::declare<Parsing>()
            .constructor()
            .function("init", &Parsing::init)
            .property("a", &Parsing::a)
            .property("b", &Parsing::b)
            ;
    }
    
} // namespace lib

PONDER_TYPE(lib::Vec)
PONDER_TYPE(std::tuple<float,float>)
PONDER_TYPE(lib::Holder)
PONDER_TYPE(lib::Types)
PONDER_TYPE(lib::Types::Obj)
PONDER_TYPE(lib::Dummy)
PONDER_TYPE(lib::Colour)
PONDER_TYPE(lib::Parsing)

static bool luaTest(lua_State *L, const char *source, int lineNb, bool success = true)
{
    std::printf("l:%d------------------------------------------------------\n", lineNb);
    std::printf("Test%s: %s\n", success ? "" : "(should fail)", source);
    const bool ok = ponder::lua::runString(L, source);
    if (ok != success)
    {
        std::printf("FAILED");
        exit(EXIT_FAILURE);
        return false;
    }
    std::printf("\n");
    return true;
}

#define LUA_PASS(SRC) luaTest(L,SRC,__LINE__,true)
#define LUA_FAIL(SRC) luaTest(L,SRC,__LINE__,false)

int main()
{
    std::printf("Lua version %s\n", LUA_VERSION);
    
    lua_State *L = luaL_newstate();
    luaopen_base(L);
    
    lib::declare();
    ponder::lua::expose<lib::Vec>(L, "Vec2");
    ponder::lua::expose<lib::Holder>(L, "Holder");
    ponder::lua::expose<lib::Types>(L, "Types");
    ponder::lua::expose<lib::Dummy>(L, "Dummy");
    ponder::lua::expose<lib::Colour>(L, "Colour");
    ponder::lua::expose<lib::Parsing>(L, "Parsing");
    
    //------------------------------------------------------------------

    // class defined
    LUA_PASS("print(Vec2); assert(Vec2 ~= nil)");

    // instance
    LUA_PASS("v = Vec2(); assert(v ~= nil)");
    LUA_PASS("assert(type(v) == 'userdata')");
    
    // property read
    LUA_PASS("assert(v.x == 0)");
    LUA_PASS("assert(v.y == 0)");

    // property write
    LUA_PASS("v.x = 7; assert(v.x == 7)");
    LUA_PASS("v.y = -3; assert(v.y == -3)");
    LUA_PASS("assert(type(v.x) == 'number'); assert(type(v.y) == 'number')");
    LUA_PASS("v.x = 1.25");
    LUA_PASS("v.x = 1.76e6");
    LUA_FAIL("v.x = 'fail'");

    // method call with args
    LUA_PASS("v:set(12, 8.5); assert(v.x == 12 and v.y == 8.5)");
    LUA_PASS("v:set(1, 2); assert(v.x == 1 and v.y == 2)");
    LUA_FAIL("v:set('fail'); print(v.x, v.y)");
    LUA_FAIL("v:set(); print(v.x, v.y)");

    // method call return args
    LUA_PASS("l = Vec2(3,0); assert(l:length() == 3)");

    // method call with object arg
    LUA_PASS("a,b = Vec2(2,3), Vec2(3,4); c = a:dot(b); print(c); assert(c == 2*3+3*4)");

    // method call (:) with return object (immutable)
    LUA_PASS("c = a:add2(b); assert(c ~= nil); print(c.x, c.y);");
    LUA_PASS("assert(c.x == 5); assert(c.y == 7);");

    // method call (:) with return object (mutable)
    LUA_PASS("c = a:add(b); assert(c ~= nil); print(c.x, c.y);");
    LUA_PASS("assert(c.x == 5); assert(c.y == 7);");

    // static func
    LUA_PASS("assert(type(Vec2.up) == 'function')");
    LUA_PASS("up = Vec2.up(7); assert(type(up) == 'userdata')");
    LUA_PASS("assert(type(up.x) == 'number')");

    // Vec return ref (func)
    LUA_PASS("r = Vec2(7,8); assert(r.x == 7)");
    LUA_PASS("r.x = 9; assert(r.x == 9)");
    LUA_PASS("r:funcRef().x = 19; assert(r.x == 19)");

    // Vec return ref (prop)
    LUA_PASS("r = Vec2(17,8); assert(r.x == 17)");
    LUA_PASS("r.x = 9; assert(r.x == 9)");
    LUA_PASS("r.propRef.x = 19; assert(r.x == 19)");
    
    // Vec return tuple or multiple values.
    LUA_PASS("t = Vec2(11,22); x,y = t:get(); print(x,y); assert(x == 11); assert(y == 22)");

    //------------------------------------------------------------------

    // Non-copyable return ref
    LUA_PASS("h = Holder()");
    LUA_PASS("h:rref().vec.x = 9; assert(h:rref().vec.x == 9)");
    
    //------------------------------------------------------------------
    
    // Types
    LUA_PASS("assert(type(Types) == 'userdata')");
    LUA_PASS("x = Types.len('two'); assert(type(x) == 'number' and x == 3)");
    LUA_PASS("assert(Types.len('1234567890') ~= 11)");
    
    LUA_PASS("assert(Types.getStr() == 'blah')");

    //------------------------------------------------------------------

    // Class static function
    LUA_PASS("assert(type(Dummy) == 'userdata')");
    LUA_PASS("assert(type(Dummy.halve) == 'function')");
    LUA_PASS("x = Dummy.halve(16); assert(x == 8)");

    // Non-class function
    LUA_PASS("assert(type(Dummy) == 'userdata')");
    LUA_PASS("assert(type(Dummy.twice) == 'function')");
    LUA_PASS("x = Dummy.twice(7); assert(x == 14)");

    //------------------------------------------------------------------
    
    // Enum
    LUA_PASS("assert(type(Colour) == 'table')");
    LUA_PASS("assert(Colour.red == 0)");
    LUA_PASS("assert(Colour.green == 1)");
    LUA_PASS("assert(Colour.blue == 2)");

    //------------------------------------------------------------------
    
    // Parsing
    LUA_PASS("p = Parsing(); assert(type(p)=='userdata')");
    LUA_PASS("p:init{a=77, b='w00t'}; assert(p.a == 77 and p.b == 'w00t')");

    return EXIT_SUCCESS;
}


