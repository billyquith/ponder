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
        : p1(true)
        , p2(2)
        , p3("3")
        , p4(MyType(4))
        , p5(false)
        , p6(6)
        , p7("7")
        , p8(One)
        , p9(new MyType(9))
        , p10(true)
        , p11(11)
        , p12("12")
        , p13(Two)
        , p18(true)
        , p19(19)
        , p20(20.)
        , p21(21)
        , p22(Two)
        , p24(MyType(24))
        {}
        
        // ***** properties used in non-member functions *****
        bool p1;
        int p2;
        ponder::String p3;
        MyType p4;
        
        // ***** properties used as direct pointer to members *****
        bool p5;
        const int p6;
        ponder::String p7;
        const MyEnum p8;
        std::shared_ptr<MyType> p9;
        
        // ***** member functions *****
        bool p10; bool getP10() {return p10;}
        int p11; int getP11() const {return p11;}
        ponder::String p12; ponder::String& getP12() {return p12;}
        MyEnum p13; MyEnum getP13() const {return p13;} void setP13(MyEnum e) {p13 = e;}
        
        // ***** nested properties *****
        struct Inner
        {            
            bool p14;
            const int p15;
            ponder::String p16;
         
            Inner() : p14(false), p15(15), p16("16"), p17(MyType(17)) {}
            
            ponder::String getP16() const {return p16;}
            
            MyType p17;
            const MyType& getP17() const {return p17;}
            void setP17(MyType t) {p17 = t;}
        };
        Inner inner;
        Inner& getInner() {return inner;}
        
        // ***** properties used with std::function *****
        bool p18;
        int p19; int& getP19() {return p19;}
        double p20; double getP20() const {return p20;} void setP20(double d) {p20 = d;}
        
        // ***** properties used with std::bind *****
        int p21;
        MyEnum p22; MyEnum getP22() {return p22;}
        ponder::String getP23(const ponder::String& str) const {return str + "23";}
        MyType p24; const MyType& getP24() const {return p24;} void setP24(MyType t) {p24 = t;}
        
        // ***** poperties that support method chaining *****
        int getP2() const { return p2; }
        MyClass& setWithChain(int i) { p2 = i; return *this; }
    };
    
    bool getP1(const MyClass& object) {return object.p1;}
    
    const int& getP2(MyClass& object) {return object.p2;}
    
    ponder::String& getP3(MyClass& object) {return object.p3;}
    
    const MyType& getP4(const MyClass& object) {return object.p4;}
    void setP4(MyClass& object, MyType value) {object.p4 = value;}
    
    int& getP21(MyClass& object) {return object.p21;}
    
    void declare()
    {
        using namespace std::placeholders;
        
        ponder::Enum::declare<MyEnum>("PropertyTest::MyEnum")
            .value("Zero", Zero)
            .value("One",  One)
            .value("Two",  Two);
        
        ponder::Class::declare<MyType>("PropertyTest::MyType");
        
        ponder::Class::declare<MyClass>("PropertyTest::MyClass")
        
            // ***** non-member functions *****
            .property("p1", &getP1)         // read-only getter (const param)
            .property("p2", &getP2)         // read-only getter (const return)
            .property("p3", &getP3)         // read-write getter
            .property("p4", &getP4, &setP4) // getter + setter
        
            // ***** pointer to members *****
            .property("p5", &MyClass::p5) // pointer to read-write member
            .property("p6", &MyClass::p6) // pointer to const member
            .property("p7", &MyClass::p7) // pointer to read-write pointer member
            .property("p8", &MyClass::p8) // pointer to const pointer member
            .property("p9", &MyClass::p9) // pointer to read-write smart pointer member
        
            // ***** members functions *****
             // read-only getter (return by value)
            .property("p10", &MyClass::getP10)
             // read-only getter (const)
            .property("p11", &MyClass::getP11)
             // read-write getter
            .property("p12", &MyClass::getP12)
             // read-only getter + write-only setter
            .property("p13", &MyClass::getP13, &MyClass::setP13)
        
            // ***** nested functions *****
             // pointer to read-write member
            .property("p14", &MyClass::Inner::p14, &MyClass::getInner)
             // Pointer to read-only member
            .property("p15", &MyClass::Inner::p15, &MyClass::inner)
             // read-only getter
            .property("p16", &MyClass::Inner::getP16, &MyClass::getInner)
             // read-only getter + write-only setter
            .property("p17", [](MyClass& self) -> const MyType& {return self.getInner().getP17();},
                             [](MyClass& self, const MyType &value){self.getInner().setP17(value);})
        
            // ***** std::function *****
             // pointer to read-write member
            .property("p18", std::function<bool (MyClass&)>(&MyClass::p18))
              // read-write getter
            .property("p19", std::function<int& (MyClass&)>(&MyClass::getP19))
              // read-only getter + write-only setter
            .property("p20", std::function<double (MyClass&)>(&MyClass::getP20),
                             std::function<void (MyClass&, double)>(&MyClass::setP20))
        
            // ***** std::bind *****
             // non-member read-write getter
            .property("p21", std::bind(&getP21, _1))
             // read-only getter
            .property("p22", std::bind(&MyClass::getP22, _1))
             // read-only getter + extra parameter
            .property("p23", std::bind(&MyClass::getP23, _1, "str"))
             // read-only getter + write-only setter
            .property("p24", std::bind(&MyClass::getP24, _1), std::bind(&MyClass::setP24, _1, _2))
        
            // ***** with method chaining *****
             // member, method chaining
            .property("p25", &MyClass::getP2, &MyClass::setWithChain)
            ;
    }
    
    static constexpr int c_NumTests = 25;
}

PONDER_AUTO_TYPE(PropertyTest::MyEnum,  &PropertyTest::declare)
PONDER_AUTO_TYPE(PropertyTest::MyType,  &PropertyTest::declare)
PONDER_AUTO_TYPE(PropertyTest::MyClass, &PropertyTest::declare)

using namespace PropertyTest;


struct PropertyFixture
{
    PropertyFixture()
    {
        const ponder::Class& metaclass = ponder::classByType<MyClass>();
        for (int i = 1; i <= c_NumTests; ++i)
        {
            properties[i] = &metaclass.property("p" + std::to_string(i));
        }
    }

    const ponder::Property* properties[25];
};

//-----------------------------------------------------------------------------
//                         Tests for ponder::Property
//-----------------------------------------------------------------------------

TEST_CASE("Classes can have properties")
{
    const ponder::Class& metaclass = ponder::classByType<MyClass>();
    const ponder::Property* properties[c_NumTests+1];
    
    for (int i = 1; i <= c_NumTests; ++i)
    {
        properties[i] = &metaclass.property("p" + std::to_string(i));
    }

    SECTION("type")
    {
        REQUIRE(properties[ 1]->kind() == ponder::ValueKind::Boolean);
        REQUIRE(properties[ 2]->kind() == ponder::ValueKind::Integer);
        REQUIRE(properties[ 3]->kind() == ponder::ValueKind::String);
        IS_TRUE(properties[ 4]->kind() == ponder::ValueKind::User);
        REQUIRE(properties[ 5]->kind() == ponder::ValueKind::Boolean);
        REQUIRE(properties[ 6]->kind() == ponder::ValueKind::Integer);
        REQUIRE(properties[ 7]->kind() == ponder::ValueKind::String);
        REQUIRE(properties[ 8]->kind() == ponder::ValueKind::Enum);
        REQUIRE(properties[ 9]->kind() == ponder::ValueKind::User);
        REQUIRE(properties[10]->kind() == ponder::ValueKind::Boolean);
        REQUIRE(properties[11]->kind() == ponder::ValueKind::Integer);
        REQUIRE(properties[12]->kind() == ponder::ValueKind::String);
        REQUIRE(properties[13]->kind() == ponder::ValueKind::Enum);
        REQUIRE(properties[14]->kind() == ponder::ValueKind::Boolean);
        REQUIRE(properties[15]->kind() == ponder::ValueKind::Integer);
        REQUIRE(properties[16]->kind() == ponder::ValueKind::String);
        REQUIRE(properties[17]->kind() == ponder::ValueKind::User);
        REQUIRE(properties[18]->kind() == ponder::ValueKind::Boolean);
        REQUIRE(properties[19]->kind() == ponder::ValueKind::Integer);
        REQUIRE(properties[20]->kind() == ponder::ValueKind::Real);
        REQUIRE(properties[21]->kind() == ponder::ValueKind::Integer);
        REQUIRE(properties[22]->kind() == ponder::ValueKind::Enum);
        REQUIRE(properties[23]->kind() == ponder::ValueKind::String);
        REQUIRE(properties[24]->kind() == ponder::ValueKind::User);
    }

    SECTION("name")
    {
        REQUIRE(properties[ 1]->name() == "p1");
        REQUIRE(properties[ 2]->name() == "p2");
        REQUIRE(properties[ 3]->name() == "p3");
        REQUIRE(properties[ 4]->name() == "p4");
        REQUIRE(properties[ 5]->name() == "p5");
        REQUIRE(properties[ 6]->name() == "p6");
        REQUIRE(properties[ 7]->name() == "p7");
        REQUIRE(properties[ 8]->name() == "p8");
        REQUIRE(properties[ 9]->name() == "p9");
        REQUIRE(properties[10]->name() == "p10");
        REQUIRE(properties[11]->name() == "p11");
        REQUIRE(properties[12]->name() == "p12");
        REQUIRE(properties[13]->name() == "p13");
        REQUIRE(properties[14]->name() == "p14");
        REQUIRE(properties[15]->name() == "p15");
        REQUIRE(properties[16]->name() == "p16");
        REQUIRE(properties[17]->name() == "p17");
        REQUIRE(properties[18]->name() == "p18");
        REQUIRE(properties[19]->name() == "p19");
        REQUIRE(properties[20]->name() == "p20");
        REQUIRE(properties[21]->name() == "p21");
        REQUIRE(properties[22]->name() == "p22");
        REQUIRE(properties[23]->name() == "p23");
        REQUIRE(properties[24]->name() == "p24");
    }

    SECTION("readable")
    {
        MyClass object;

        REQUIRE(properties[ 1]->isReadable() == true);
        REQUIRE(properties[ 2]->isReadable() == true);
        REQUIRE(properties[ 3]->isReadable() == true);
        REQUIRE(properties[ 4]->isReadable() == true);
        REQUIRE(properties[ 5]->isReadable() == true);
        REQUIRE(properties[ 6]->isReadable() == true);
        REQUIRE(properties[ 7]->isReadable() == true);
        REQUIRE(properties[ 8]->isReadable() == true);
        REQUIRE(properties[ 9]->isReadable() == true);
        REQUIRE(properties[10]->isReadable() == true);
        REQUIRE(properties[11]->isReadable() == true);
        REQUIRE(properties[12]->isReadable() == true);
        REQUIRE(properties[13]->isReadable() == true);
        REQUIRE(properties[14]->isReadable() == true);
        REQUIRE(properties[15]->isReadable() == true);
        REQUIRE(properties[16]->isReadable() == true);
        REQUIRE(properties[17]->isReadable() == true);
        REQUIRE(properties[18]->isReadable() == true);
        REQUIRE(properties[19]->isReadable() == true);
        REQUIRE(properties[20]->isReadable() == true);
        REQUIRE(properties[21]->isReadable() == true);
        REQUIRE(properties[22]->isReadable() == true);
        REQUIRE(properties[23]->isReadable() == true);
        REQUIRE(properties[24]->isReadable() == true);
    }

    SECTION("writable")
    {
        MyClass object;

        REQUIRE(properties[ 1]->isWritable() == false);
        REQUIRE(properties[ 2]->isWritable() == false);
        REQUIRE(properties[ 3]->isWritable() == true);
        REQUIRE(properties[ 4]->isWritable() == true);
        REQUIRE(properties[ 5]->isWritable() == true);
        REQUIRE(properties[ 6]->isWritable() == false);
        REQUIRE(properties[ 7]->isWritable() == true);
        REQUIRE(properties[ 8]->isWritable() == false);
        REQUIRE(properties[ 9]->isWritable() == true);
        REQUIRE(properties[10]->isWritable() == false);
        REQUIRE(properties[11]->isWritable() == false);
        REQUIRE(properties[12]->isWritable() == true);
        REQUIRE(properties[13]->isWritable() == true);
        REQUIRE(properties[14]->isWritable() == true);
        REQUIRE(properties[15]->isWritable() == false);
        REQUIRE(properties[16]->isWritable() == false);
        REQUIRE(properties[17]->isWritable() == true);
        REQUIRE(properties[18]->isWritable() == false);
        REQUIRE(properties[19]->isWritable() == true);
        REQUIRE(properties[20]->isWritable() == true);
        REQUIRE(properties[21]->isWritable() == true);
        REQUIRE(properties[22]->isWritable() == false);
        REQUIRE(properties[23]->isWritable() == false);
        REQUIRE(properties[24]->isWritable() == true);
    }


    SECTION("get")
    {
        MyClass object;

        REQUIRE(properties[ 1]->get(object) == ponder::Value(object.p1));
        REQUIRE(properties[ 2]->get(object) == ponder::Value(object.p2));
        REQUIRE(properties[ 3]->get(object) == ponder::Value(object.p3));
        IS_TRUE(properties[ 4]->get(object) == ponder::Value(object.p4));
        REQUIRE(properties[ 5]->get(object) == ponder::Value(object.p5));
        REQUIRE(properties[ 6]->get(object) == ponder::Value(object.p6));
        REQUIRE(properties[ 7]->get(object) == ponder::Value(object.p7));
        REQUIRE(properties[ 8]->get(object) == ponder::Value(object.p8));
        IS_TRUE(properties[ 9]->get(object) == ponder::Value(*object.p9) );
        REQUIRE(properties[10]->get(object) == ponder::Value(object.p10));
        REQUIRE(properties[11]->get(object) == ponder::Value(object.p11));
        REQUIRE(properties[12]->get(object) == ponder::Value(object.p12));
        REQUIRE(properties[13]->get(object) == ponder::Value(object.p13));
        REQUIRE(properties[14]->get(object) == ponder::Value(object.inner.p14));
        REQUIRE(properties[15]->get(object) == ponder::Value(object.inner.p15));
        REQUIRE(properties[16]->get(object) == ponder::Value(object.inner.p16));
        IS_TRUE(properties[17]->get(object) == ponder::Value(object.inner.p17));
        REQUIRE(properties[18]->get(object) == ponder::Value(object.p18));
        REQUIRE(properties[19]->get(object) == ponder::Value(object.p19));
        REQUIRE(properties[20]->get(object) == ponder::Value(object.p20));
        REQUIRE(properties[21]->get(object) == ponder::Value(object.p21));
        IS_TRUE(properties[22]->get(object) == ponder::Value(object.p22));
        IS_TRUE(properties[23]->get(object) == ponder::Value(object.getP23("str")) );
        IS_TRUE(properties[24]->get(object) == ponder::Value(object.p24) );
    }


    SECTION("set")
    {
        MyClass object;

        REQUIRE_THROWS_AS(properties[ 1]->set(object, false), ponder::ForbiddenWrite);
        REQUIRE_THROWS_AS(properties[ 2]->set(object, -2), ponder::ForbiddenWrite);
        REQUIRE_THROWS_AS(properties[ 6]->set(object, -6), ponder::ForbiddenWrite);
        REQUIRE_THROWS_AS(properties[ 8]->set(object, Two),  ponder::ForbiddenWrite);
        REQUIRE_THROWS_AS(properties[10]->set(object, false), ponder::ForbiddenWrite);
        REQUIRE_THROWS_AS(properties[11]->set(object, -11), ponder::ForbiddenWrite);
        REQUIRE_THROWS_AS(properties[15]->set(object, -15), ponder::ForbiddenWrite);
        REQUIRE_THROWS_AS(properties[16]->set(object, "-16"), ponder::ForbiddenWrite);
        REQUIRE_THROWS_AS(properties[18]->set(object, false), ponder::ForbiddenWrite);
        REQUIRE_THROWS_AS(properties[23]->set(object, "-23"), ponder::ForbiddenWrite);

        properties[ 3]->set(object, "-3");
        properties[ 4]->set(object, MyType(-4));
        properties[ 5]->set(object, true);
        properties[ 7]->set(object, "-7");
        properties[ 9]->set(object, MyType(-9));
        properties[12]->set(object, "-12");
        properties[13]->set(object, Zero);
        properties[14]->set(object, false);
        properties[17]->set(object, MyType(-17));
        properties[19]->set(object, -19);
        properties[20]->set(object, -20.);
        properties[21]->set(object, -21);
        properties[24]->set(object, MyType(-24));

        REQUIRE(object.p3 ==        "-3");
        REQUIRE(object.p4 ==        MyType(-4));
        REQUIRE(object.p5 ==        true);
        REQUIRE(object.p7 ==        "-7");
        REQUIRE(*object.p9 ==       MyType(-9));
        REQUIRE(object.p12 ==       "-12");
        REQUIRE(object.p13 ==       Zero);
        REQUIRE(object.inner.p14 == false);
        REQUIRE(object.inner.p17 == MyType(-17));
        REQUIRE(object.p19 ==       -19);
        REQUIRE(object.p20 ==       -20.);
        REQUIRE(object.p21 ==       -21);
        REQUIRE(object.p24 ==       MyType(-24));
    }
}


