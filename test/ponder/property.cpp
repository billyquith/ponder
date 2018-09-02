/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
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

#include <ponder/classget.hpp>
#include <ponder/enumget.hpp>
#include <ponder/property.hpp>
#include <ponder/class.hpp>
#include <ponder/enum.hpp>
#include <ponder/classbuilder.hpp>
#include "test.hpp"
#include <string>

namespace PropertyTest
{
    enum MyEnum
    {
        Zero = 0,
        One  = 1,
        Two  = 2
    };

    struct MyType
    {
        MyType(int x_) : x(x_) {}
        int x;
    };

    bool operator == (const MyType& left, const MyType& right) {return left.x == right.x;}
    bool operator < (const MyType& left, const MyType& right) {return left.x < right.x;}
    std::ostream& operator << (std::ostream& stream, const MyType& object)
    {
        return stream << object.x;
    }

    struct MyClass
    {
        MyClass()
        : b(false)
        , i(0)
        , f(0.f)
        {}

        // members
        bool b;
        int i;
        float f;
        std::string s;
        //     const MyEnum p8;
        //     std::shared_ptr<MyType> p9;

#define MEMBER_ACCESSORS(T,N) \
        T& rw_##N() {return N;} \
        const T& r_##N() const {return N;} \
        void w_##N(const T& v) {N = v;}

        MEMBER_ACCESSORS(bool,b)
        MEMBER_ACCESSORS(int,i)
        MEMBER_ACCESSORS(float,f)
        MEMBER_ACCESSORS(std::string,s)
    };

#define FUNCTION_ACCESSORS(T,N) \
    T& rw_##N(MyClass& c) {return c.N;} \
    const T& r_##N(const MyClass& c) {return c.N;} \
    void w_##N(MyClass& c, T v) {c.N = v;}

    FUNCTION_ACCESSORS(bool,b)
    FUNCTION_ACCESSORS(int,i)
    FUNCTION_ACCESSORS(float,f)
    FUNCTION_ACCESSORS(std::string,s)

    void declare()
    {
        using namespace std::placeholders;

        ponder::Enum::declare<MyEnum>("PropertyTest::MyEnum")
            .value("Zero", Zero)
            .value("One",  One)
            .value("Two",  Two);

        ponder::Class::declare<MyType>("PropertyTest::MyType");

#define PROPERTY(T,N) \
        .property("m_" #N, &MyClass::N) /* member object */ \
        .property("mf_r_" #N, &MyClass::r_##N) /* ro member func*/ \
        .property("mf_rw_" #N, &MyClass::rw_##N) /* rw member func*/ \
        .property("mf_gs_" #N, &MyClass::r_##N, &MyClass::w_##N) /* get/set member func*/ \
        .property("f_r_" #N, &r_##N) /* ro function */ \
        .property("f_rw_" #N, &rw_##N) /* rw function */ \
        .property("f_gs_" #N, &r_##N, &w_##N) /* get/set function */ \
        .property("l_r_" #N, [](const MyClass& c) -> const T& {return c.N;}) /* ro lambda */ \
        .property("l_rw_" #N, [](MyClass& c) -> T& {return c.N;}) /* rw lambda */ \
        .property("l_gs_" #N, [](const MyClass& c) {return c.N;}, [](MyClass& c, T v) {c.N = v;}) /* g lambda */

        ponder::Class::declare<MyClass>("PropertyTest::MyClass")
            PROPERTY(bool,b)
            PROPERTY(int,i)
            PROPERTY(float,f)
            PROPERTY(std::string,s)
            ;

        // ***** std::function *****
//            .property("p18", std::function<int(MyClass&)>(&MyClass::get18))   // function getter
//              // read-write getter
//            .property("p19", std::function<int& (MyClass&)>(&MyClass::getP19))
//              // read-only getter + write-only setter
//            .property("p20", std::function<double (MyClass&)>(&MyClass::getP20),
//                             std::function<void (MyClass&, double)>(&MyClass::setP20))

            // ***** std::bind *****
             // non-member read-write getter
//            .property("p21", std::bind(&getP21, _1))
//             // read-only getter
//            .property("p22", std::bind(&MyClass::getP22, _1))
//             // read-only getter + extra parameter
//            .property("p23", std::bind(&MyClass::getP23, _1, "str"))
//             // read-only getter + write-only setter
//            .property("p24", std::bind(&MyClass::getP24, _1), std::bind(&MyClass::setP24, _1, _2))
    }
}

PONDER_AUTO_TYPE(PropertyTest::MyEnum,  &PropertyTest::declare)
PONDER_AUTO_TYPE(PropertyTest::MyType,  &PropertyTest::declare)
PONDER_AUTO_TYPE(PropertyTest::MyClass, &PropertyTest::declare)

using namespace PropertyTest;

//-----------------------------------------------------------------------------
//                         Tests for ponder::Property
//-----------------------------------------------------------------------------

TEST_CASE("Classes can have properties")
{
    const ponder::Class& metaclass = ponder::classByType<MyClass>();

    SECTION("type")
    {
#define CHECK_PROP_TYPE(N,T) \
        REQUIRE(metaclass.property("m_" #N).kind() == T); \
        REQUIRE(metaclass.property("mf_r_" #N).kind() == T); \
        REQUIRE(metaclass.property("mf_rw_" #N).kind() == T); \
        REQUIRE(metaclass.property("mf_gs_" #N).kind() == T); \
        REQUIRE(metaclass.property("f_r_" #N).kind() == T); \
        REQUIRE(metaclass.property("f_rw_" #N).kind() == T); \
        REQUIRE(metaclass.property("f_gs_" #N).kind() == T); \
        REQUIRE(metaclass.property("l_r_" #N).kind() == T); \
        REQUIRE(metaclass.property("l_rw_" #N).kind() == T); \
        REQUIRE(metaclass.property("l_gs_" #N).kind() == T)

        CHECK_PROP_TYPE(b, ponder::ValueKind::Boolean);
        CHECK_PROP_TYPE(i, ponder::ValueKind::Integer);
        CHECK_PROP_TYPE(f, ponder::ValueKind::Real);
        CHECK_PROP_TYPE(s, ponder::ValueKind::String);
    }

    SECTION("readable")
    {
        // all should be readable
#define CHECK_PROP_READABLE(N) \
        REQUIRE(metaclass.property("m_" #N).isReadable() == true); \
        REQUIRE(metaclass.property("mf_r_" #N).isReadable() == true); \
        REQUIRE(metaclass.property("mf_rw_" #N).isReadable() == true); \
        REQUIRE(metaclass.property("mf_gs_" #N).isReadable() == true); \
        REQUIRE(metaclass.property("f_r_" #N).isReadable() == true); \
        REQUIRE(metaclass.property("f_rw_" #N).isReadable() == true); \
        REQUIRE(metaclass.property("f_gs_" #N).isReadable() == true); \
        REQUIRE(metaclass.property("l_r_" #N).isReadable() == true); \
        REQUIRE(metaclass.property("l_rw_" #N).isReadable() == true); \
        REQUIRE(metaclass.property("l_gs_" #N).isReadable() == true);

        CHECK_PROP_READABLE(b);
        CHECK_PROP_READABLE(i);
        CHECK_PROP_READABLE(f);
        CHECK_PROP_READABLE(s);
    }

    SECTION("writable")
    {
#define CHECK_PROP_WRITABLE(N) \
        REQUIRE(metaclass.property("m_" #N).isWritable() == true); \
        REQUIRE(metaclass.property("mf_r_" #N).isWritable() == false); \
        REQUIRE(metaclass.property("mf_rw_" #N).isWritable() == true); \
        REQUIRE(metaclass.property("mf_gs_" #N).isWritable() == true); \
        REQUIRE(metaclass.property("f_r_" #N).isWritable() == false); \
        REQUIRE(metaclass.property("f_rw_" #N).isWritable() == true); \
        REQUIRE(metaclass.property("f_gs_" #N).isWritable() == true); \
        REQUIRE(metaclass.property("l_r_" #N).isWritable() == false); \
        REQUIRE(metaclass.property("l_rw_" #N).isWritable() == true); \
        REQUIRE(metaclass.property("l_gs_" #N).isWritable() == true);

        CHECK_PROP_WRITABLE(b);
        CHECK_PROP_WRITABLE(i);
        CHECK_PROP_WRITABLE(f);
        CHECK_PROP_WRITABLE(s);
    }

    SECTION("get")
    {
        MyClass c;
        ponder::UserObject object(&c);

#define CHECK_PROP_GET(T,N) \
        REQUIRE(metaclass.property("m_" #N).get(object).to<T>() == c.N); \
        REQUIRE(metaclass.property("mf_r_" #N).get(object).to<T>() == c.N); \
        REQUIRE(metaclass.property("mf_rw_" #N).get(object).to<T>() == c.N); \
        REQUIRE(metaclass.property("mf_gs_" #N).get(object).to<T>() == c.N); \
        REQUIRE(metaclass.property("f_r_" #N).get(object).to<T>() == c.N); \
        REQUIRE(metaclass.property("f_rw_" #N).get(object).to<T>() == c.N); \
        REQUIRE(metaclass.property("f_gs_" #N).get(object).to<T>() == c.N); \
        REQUIRE(metaclass.property("l_r_" #N).get(object).to<T>() == c.N); \
        REQUIRE(metaclass.property("l_rw_" #N).get(object).to<T>() == c.N); \
        REQUIRE(metaclass.property("l_gs_" #N).get(object).to<T>() == c.N);

        c.b = true;
        c.i = 77;
        c.f = 34.5f;
        c.s = "Woo!";

        CHECK_PROP_GET(bool,b);
        CHECK_PROP_GET(int,i);
        CHECK_PROP_GET(float,f);
        CHECK_PROP_GET(std::string,s);
    }

    SECTION("set")
    {
#define CHECK_PROP_SET_PASS(NAME,N,V) \
    {   MyClass object; \
        auto const& prop = metaclass.property(NAME); \
        REQUIRE(object.N != V); \
        prop.set(&object, V); \
        REQUIRE(object.N == V); \
    }

#define CHECK_PROP_SET_FAIL(NAME,N,V) \
    {   MyClass object; \
        REQUIRE_THROWS_AS(metaclass.property(NAME).set(&object, V), ponder::ForbiddenWrite); \
    }

#define CHECK_PROP_SET(N,V) \
        CHECK_PROP_SET_PASS("m_" #N, N, V); \
        CHECK_PROP_SET_FAIL("mf_r_" #N, N, V); \
        CHECK_PROP_SET_PASS("mf_rw_" #N, N, V); \
        CHECK_PROP_SET_PASS("mf_gs_" #N, N, V); \
        CHECK_PROP_SET_FAIL("f_r_" #N, N, V); \
        CHECK_PROP_SET_PASS("f_rw_" #N, N, V); \
        CHECK_PROP_SET_PASS("f_gs_" #N, N, V); \
        CHECK_PROP_SET_FAIL("l_r_" #N, N, V); \
        CHECK_PROP_SET_PASS("l_rw_" #N, N, V); \
        CHECK_PROP_SET_PASS("l_gs_" #N, N, V)

        CHECK_PROP_SET(b,true);
        CHECK_PROP_SET(i,789);
        CHECK_PROP_SET(f,345.75f);
        CHECK_PROP_SET(s,std::string("The Reverend Black Grape"));
    }
}


