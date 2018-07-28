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

#define ACCESSORS(T,N) \
        T& rw_##N() {return N;} \
        const T& r_##N() const {return N;} \
        void w_##N(const T& v) {N = v;}

        ACCESSORS(bool,b)
        ACCESSORS(int,i)
        ACCESSORS(float,f)
        ACCESSORS(std::string,s)
    };

    //     // ***** properties used as direct pointer to members *****
    //     bool p5;
    //     const int p6;
    //     ponder::String p7;
    //     const MyEnum p8;
    //     std::shared_ptr<MyType> p9;
    //
    //     // ***** member functions *****
    //     bool p10; bool getP10() {return p10;}
    //     int p11; int getP11() const {return p11;}
    //     ponder::String p12; ponder::String& getP12() {return p12;}
    //     int p13; int getP13() const {return p13;} void setP13(int v) {p13 = v;}
    //
    //     // ***** nested properties *****
    //     struct Inner
    //     {
    //         bool p14;
    //         const int p15;
    //         ponder::String p16;
    //
    //         Inner() : p14(false), p15(15), p16("16"), p17(MyType(17)) {}
    //
    //         ponder::String getP16() const {return p16;}
    //
    //         MyType p17;
    //         const MyType& getP17() const {return p17;}
    //         void setP17(MyType t) {p17 = t;}
    //     };
    //     Inner inner;
    //     Inner& getInner() {return inner;}
    //
    //     // ***** properties used with std::function *****
    //     bool p18;
    //         int get18() const {return p18;}
    //         std::function<bool (MyClass&)> funcGet18{&MyClass::get18};
    //     int p19; int& getP19() {return p19;}
    //     double p20; double getP20() const {return p20;} void setP20(double d) {p20 = d;}
    //
    //     // ***** properties used with std::bind *****
    //     int p21;
    //     MyEnum p22; MyEnum getP22() {return p22;}
    //     ponder::String getP23(const ponder::String& str) const {return str + "23";}
    //     MyType p24; const MyType& getP24() const {return p24;} void setP24(MyType t) {p24 = t;}
    //
    //     // ***** poperties that support method chaining *****
    //     int getP2() const { return p2; }
    //     MyClass& setWithChain(int i) { p2 = i; return *this; }
    // };
    //
    // bool getP1(const MyClass& object) {return object.p1;}
    //
    // const int& getP2(const MyClass& object) {return object.p2;}
    //
    // ponder::String& getP3(MyClass& object) {return object.p3;}
    //
    // const MyType& getP4(const MyClass& object) {return object.p4;}
    // void setP4(MyClass& object, MyType value) {object.p4 = value;}
    //
    // int& getP21(MyClass& object) {return object.p21;}

    void declare()
    {
        using namespace std::placeholders;

        ponder::Enum::declare<MyEnum>("PropertyTest::MyEnum")
            .value("Zero", Zero)
            .value("One",  One)
            .value("Two",  Two);

        ponder::Class::declare<MyType>("PropertyTest::MyType");

#define PROPERTY(N) \
        .property(#N, &MyClass::N) /*member object*/ \
        .property("r_" #N, &MyClass::r_##N) /* ro member func*/ \
        .property("rw_" #N, &MyClass::r_##N, &MyClass::w_##N) /* rw member func*/

        ponder::Class::declare<MyClass>("PropertyTest::MyClass")
            PROPERTY(b)
            PROPERTY(i)
            PROPERTY(f)
            PROPERTY(s)
            ;

//             // ***** non-member functions *****
//             .property("p1", &getP1)         // read-only getter (value)
//             .property("p2", &getP2)         // read-only getter (const ref)
//             .property("p3", &getP3)         // read-write getter
//             .property("p4", &getP4, &setP4) // getter + setter
//
//             // ***** pointer to members *****
//             .property("p5", &MyClass::p5) // pointer to read-write member
//             .property("p6", &MyClass::p6) // pointer to const member
//             .property("p7", &MyClass::p7) // pointer to read-write pointer member
//             .property("p8", &MyClass::p8) // pointer to const pointer member
// //            .property("p9", &MyClass::p9) // pointer to read-write smart pointer member
//
//             // ***** members functions *****
//             .property("p10", &MyClass::getP10)  // read-only getter (return by value)
//             .property("p11", &MyClass::getP11)  // read-only getter (const)
//             .property("p12", &MyClass::getP12)  // read-write getter
//             // read-only getter + write-only setter
//             .property("p13", &MyClass::getP13, &MyClass::setP13)
//
//             // ***** nested functions *****
//             // pointer to read-write member
//             .property("p14", [](MyClass& s) -> bool& {return s.inner.p14;})
//             // Pointer to read-only member
//             .property("p15", [](const MyClass& s) { return s.inner.p15; })
//             // read-only getter
//             .property("p16", [](MyClass& s) { return s.inner.getP16(); })
            // read-only getter + write-only setter
//            .property("p17", [](const MyClass& self) // -> const MyType&
//                                 {return self.getInner().getP17();},  // get
//                             [](MyClass& self, const MyType &value){self.getInner().setP17(value);})    // set

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

            // ***** with method chaining *****
             // member, method chaining
//            .property("p25", &MyClass::getP2, &MyClass::setWithChain)
            // ;
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
        REQUIRE(metaclass.property(#N).kind() == T); \
        REQUIRE(metaclass.property("r_" #N).kind() == T); \
        REQUIRE(metaclass.property("rw_" #N).kind() == T)
        
        CHECK_PROP_TYPE(b, ponder::ValueKind::Boolean);
        CHECK_PROP_TYPE(i, ponder::ValueKind::Integer);
        CHECK_PROP_TYPE(f, ponder::ValueKind::Real);
        CHECK_PROP_TYPE(s, ponder::ValueKind::String);
    }

    SECTION("readable")
    {
        // all should be readable
#define CHECK_PROP_READABLE(N) \
        REQUIRE(metaclass.property(#N).isReadable() == true); \
        REQUIRE(metaclass.property("r_" #N).isReadable() == true); \
        REQUIRE(metaclass.property("rw_" #N).isReadable() == true)

        CHECK_PROP_READABLE(b);
        CHECK_PROP_READABLE(i);
        CHECK_PROP_READABLE(f);
        CHECK_PROP_READABLE(s);
    }

    SECTION("writable")
    {
#define CHECK_PROP_WRITABLE(N) \
        REQUIRE(metaclass.property(#N).isWritable() == true); \
        REQUIRE(metaclass.property("r_" #N).isWritable() == false); \
        REQUIRE(metaclass.property("rw_" #N).isWritable() == true)
        
        CHECK_PROP_WRITABLE(b);
        CHECK_PROP_WRITABLE(i);
        CHECK_PROP_WRITABLE(f);
        CHECK_PROP_WRITABLE(s);
    }

    SECTION("get")
    {
        MyClass object;

#define CHECK_PROP_GET(N) \
        REQUIRE(metaclass.property(#N).get(object) == ponder::Value(object.N)); \
        REQUIRE(metaclass.property("r_" #N).get(object) == ponder::Value(object.N)); \
        REQUIRE(metaclass.property("rw_" #N).get(object) == ponder::Value(object.N))

        CHECK_PROP_GET(b);
        CHECK_PROP_GET(i);
        CHECK_PROP_GET(f);
        CHECK_PROP_GET(s);
    }

    SECTION("set")
    {
#define CHECK_PROP_SET(N,V) \
        { MyClass object; \
            metaclass.property(#N).set(object, V); \
            REQUIRE(metaclass.property(#N).get(object) == object.N); } \
        { MyClass object; \
            REQUIRE_THROWS_AS(metaclass.property("r_" #N).set(object, V), ponder::ForbiddenWrite); }\
        { MyClass object; \
            metaclass.property("rw_" #N).set(object, V); \
            REQUIRE(metaclass.property(#N).get(object) == object.N); }

        CHECK_PROP_SET(b,true)
        CHECK_PROP_SET(i,789)
        CHECK_PROP_SET(f,345.75f)
        CHECK_PROP_SET(s, std::string("The Reverend Black Grape"))
    }
}


