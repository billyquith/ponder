/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
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
#include <ponder/pondertype.hpp>
#include <ponder/class.hpp>
#include <ponder/enum.hpp>
#include <ponder/classbuilder.hpp>
#include <boost/test/unit_test.hpp>
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
    
    bool operator==(const MyType& left, const MyType& right) {return left.x == right.x;}
    bool operator<(const MyType& left, const MyType& right) {return left.x < right.x;}
    std::ostream& operator<<(std::ostream& stream, const MyType& object) {return stream << object.x;}
    
    struct MyClass
    {
        MyClass()
        : p1(true)
        , p2(2)
        , p3("3")
        , p4(MyType(4))
        , p5(false)
        , p6(6)
        , p7_impl("7"), p7(&p7_impl)
        , p8_impl(One), p8(&p8_impl)
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
        {
        }
        
        // ***** properties used in non-member functions *****
        bool p1;
        int p2;
        std::string p3;
        MyType p4;
        
        // ***** properties used as direct pointer to members *****
        bool p5;
        const int p6;
        std::string p7_impl; std::string* p7;
        MyEnum p8_impl; const MyEnum* p8;
        std::shared_ptr<MyType> p9;
        
        // ***** member functions *****
        bool p10; bool getP10() {return p10;}
        int p11; int getP11() const {return p11;}
        std::string p12; std::string& getP12() {return p12;}
        MyEnum p13; MyEnum getP13() const {return p13;} void setP13(MyEnum e) {p13 = e;}
        
        // ***** nested properties *****
        struct Inner
        {
            Inner() : p14(false), p15(15), p16("16"), p17(MyType(17)) {}
            bool p14;
            const int p15;
            std::string p16; std::string getP16() const {return p16;}
            MyType p17; const MyType& getP17() const {return p17;} void setP17(MyType t) {p17 = t;}
        };
        Inner inner;
        Inner& getInner() {return inner;}
        
        // ***** properties used with std::function *****
        bool p18;
        int p19; int& getP19() {return p19;}
        double p20; double getP20() const {return p20;} void setP20(double d) {p20 = d;}
        
        // ***** properties used with std::bind *****
        int p21;
        MyEnum p22; MyEnum* getP22() {return &p22;}
        std::string getP23(const std::string& str) const {return str + "23";}
        MyType p24; const MyType& getP24() const {return p24;} void setP24(MyType t) {p24 = t;}
    };
    
    bool getP1(const MyClass& object) {return object.p1;}
    
    const int& getP2(MyClass& object) {return object.p2;}
    
    std::string& getP3(MyClass& object) {return object.p3;}
    
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
        // TOFIX .property("p7", &MyClass::p7) // pointer to read-write pointer member
        // TOFIX .property("p8", &MyClass::p8) // pointer to const pointer member
        .property("p9", &MyClass::p9) // pointer to read-write smart pointer member
        
        // ***** members functions *****
        .property("p10", &MyClass::getP10)                   // read-only getter (return by value)
        .property("p11", &MyClass::getP11)                   // read-only getter (const)
        .property("p12", &MyClass::getP12)                   // read-write getter
        .property("p13", &MyClass::getP13, &MyClass::setP13) // read-only getter + write-only setter
        
        // ***** nested functions *****
        .property("p14", &MyClass::Inner::p14, &MyClass::getInner)    // pointer to read-write member
        .property("p15", &MyClass::Inner::p15, &MyClass::inner)       // Pointer to read-only member
        .property("p16", &MyClass::Inner::getP16, &MyClass::getInner) // read-only getter
        .property("p17", &MyClass::Inner::getP17,
                  &MyClass::Inner::setP17, &MyClass::inner)    // read-only getter + write-only setter
        
        // ***** std::function *****
        .property("p18", std::function<bool (MyClass&)>(&MyClass::p18))    // pointer to read-write member
        .property("p19", std::function<int& (MyClass&)>(&MyClass::getP19)) // read-write getter
        .property("p20", std::function<double (MyClass&)>(&MyClass::getP20),
                  std::function<void (MyClass&, double)>(&MyClass::setP20)) // read-only getter + write-only setter
        
        // ***** std::bind *****
        .property("p21", std::bind(&getP21, _1))                 // non-member read-write getter
        // TOFIX .property("p22", std::bind(&MyClass::getP22, _1))        // read-write getter to pointer
        .property("p23", std::bind(&MyClass::getP23, _1, "str")) // read-only getter + extra parameter
        .property("p24", std::bind(&MyClass::getP24, _1),
                  std::bind(&MyClass::setP24, _1, _2))    // read-only getter + write-only setter
        ;
    }
}

PONDER_AUTO_TYPE(PropertyTest::MyEnum,  &PropertyTest::declare)
PONDER_AUTO_TYPE(PropertyTest::MyType,  &PropertyTest::declare)
PONDER_AUTO_TYPE(PropertyTest::MyClass, &PropertyTest::declare)

using namespace PropertyTest;

//-----------------------------------------------------------------------------
struct PropertyFixture
{
    PropertyFixture()
    {
        const ponder::Class& metaclass = ponder::classByType<MyClass>();
        for (int i = 1; i < 25; ++i)
        {
            if (i != 7 && i != 8 && i != 22) // remove when fixed
                properties[i] = &metaclass.property("p" + std::to_string(i));
        }
    }

    const ponder::Property* properties[25];
};

//-----------------------------------------------------------------------------
//                         Tests for ponder::Property
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(PROPERTY, PropertyFixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(type)
{
    BOOST_CHECK_EQUAL(properties[1]->type(),  ponder::boolType);
    BOOST_CHECK_EQUAL(properties[2]->type(),  ponder::intType);
    BOOST_CHECK_EQUAL(properties[3]->type(),  ponder::stringType);
    BOOST_CHECK_EQUAL(properties[4]->type(),  ponder::userType);
    BOOST_CHECK_EQUAL(properties[5]->type(),  ponder::boolType);
    BOOST_CHECK_EQUAL(properties[6]->type(),  ponder::intType);
    //BOOST_CHECK_EQUAL(properties[7]->type(),  ponder::stringType);
    //BOOST_CHECK_EQUAL(properties[8]->type(),  ponder::enumType);
    BOOST_CHECK_EQUAL(properties[9]->type(),  ponder::userType);
    BOOST_CHECK_EQUAL(properties[10]->type(), ponder::boolType);
    BOOST_CHECK_EQUAL(properties[11]->type(), ponder::intType);
    BOOST_CHECK_EQUAL(properties[12]->type(), ponder::stringType);
    BOOST_CHECK_EQUAL(properties[13]->type(), ponder::enumType);
    BOOST_CHECK_EQUAL(properties[14]->type(), ponder::boolType);
    BOOST_CHECK_EQUAL(properties[15]->type(), ponder::intType);
    BOOST_CHECK_EQUAL(properties[16]->type(), ponder::stringType);
    BOOST_CHECK_EQUAL(properties[17]->type(), ponder::userType);
    BOOST_CHECK_EQUAL(properties[18]->type(), ponder::boolType);
    BOOST_CHECK_EQUAL(properties[19]->type(), ponder::intType);
    BOOST_CHECK_EQUAL(properties[20]->type(), ponder::realType);
    BOOST_CHECK_EQUAL(properties[21]->type(), ponder::intType);
    //BOOST_CHECK_EQUAL(properties[22]->type(), ponder::enumType);
    BOOST_CHECK_EQUAL(properties[23]->type(), ponder::stringType);
    BOOST_CHECK_EQUAL(properties[24]->type(), ponder::userType);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(name)
{
    BOOST_CHECK_EQUAL(properties[1]->name(),  "p1");
    BOOST_CHECK_EQUAL(properties[2]->name(),  "p2");
    BOOST_CHECK_EQUAL(properties[3]->name(),  "p3");
    BOOST_CHECK_EQUAL(properties[4]->name(),  "p4");
    BOOST_CHECK_EQUAL(properties[5]->name(),  "p5");
    BOOST_CHECK_EQUAL(properties[6]->name(),  "p6");
    //BOOST_CHECK_EQUAL(properties[7]->name(),  "p7");
    //BOOST_CHECK_EQUAL(properties[8]->name(),  "p8");
    BOOST_CHECK_EQUAL(properties[9]->name(),  "p9");
    BOOST_CHECK_EQUAL(properties[10]->name(), "p10");
    BOOST_CHECK_EQUAL(properties[11]->name(), "p11");
    BOOST_CHECK_EQUAL(properties[12]->name(), "p12");
    BOOST_CHECK_EQUAL(properties[13]->name(), "p13");
    BOOST_CHECK_EQUAL(properties[14]->name(), "p14");
    BOOST_CHECK_EQUAL(properties[15]->name(), "p15");
    BOOST_CHECK_EQUAL(properties[16]->name(), "p16");
    BOOST_CHECK_EQUAL(properties[17]->name(), "p17");
    BOOST_CHECK_EQUAL(properties[18]->name(), "p18");
    BOOST_CHECK_EQUAL(properties[19]->name(), "p19");
    BOOST_CHECK_EQUAL(properties[20]->name(), "p20");
    BOOST_CHECK_EQUAL(properties[21]->name(), "p21");
    //BOOST_CHECK_EQUAL(properties[22]->name(), "p22");
    BOOST_CHECK_EQUAL(properties[23]->name(), "p23");
    BOOST_CHECK_EQUAL(properties[24]->name(), "p24");
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(readable)
{
    MyClass object;

    BOOST_CHECK_EQUAL(properties[1]->readable(object),  true);
    BOOST_CHECK_EQUAL(properties[2]->readable(object),  true);
    BOOST_CHECK_EQUAL(properties[3]->readable(object),  true);
    BOOST_CHECK_EQUAL(properties[4]->readable(object),  true);
    BOOST_CHECK_EQUAL(properties[5]->readable(object),  true);
    BOOST_CHECK_EQUAL(properties[6]->readable(object),  true);
    //BOOST_CHECK_EQUAL(properties[7]->readable(object),  true);
    //BOOST_CHECK_EQUAL(properties[8]->readable(object),  true);
    BOOST_CHECK_EQUAL(properties[9]->readable(object),  true);
    BOOST_CHECK_EQUAL(properties[10]->readable(object), true);
    BOOST_CHECK_EQUAL(properties[11]->readable(object), true);
    BOOST_CHECK_EQUAL(properties[12]->readable(object), true);
    BOOST_CHECK_EQUAL(properties[13]->readable(object), true);
    BOOST_CHECK_EQUAL(properties[14]->readable(object), true);
    BOOST_CHECK_EQUAL(properties[15]->readable(object), true);
    BOOST_CHECK_EQUAL(properties[16]->readable(object), true);
    BOOST_CHECK_EQUAL(properties[17]->readable(object), true);
    BOOST_CHECK_EQUAL(properties[18]->readable(object), true);
    BOOST_CHECK_EQUAL(properties[19]->readable(object), true);
    BOOST_CHECK_EQUAL(properties[20]->readable(object), true);
    BOOST_CHECK_EQUAL(properties[21]->readable(object), true);
    //BOOST_CHECK_EQUAL(properties[22]->readable(object), true);
    BOOST_CHECK_EQUAL(properties[23]->readable(object), true);
    BOOST_CHECK_EQUAL(properties[24]->readable(object), true);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(writable)
{
    MyClass object;

    BOOST_CHECK_EQUAL(properties[1]->writable(object),  false);
    BOOST_CHECK_EQUAL(properties[2]->writable(object),  false);
    BOOST_CHECK_EQUAL(properties[3]->writable(object),  true);
    BOOST_CHECK_EQUAL(properties[4]->writable(object),  true);
    BOOST_CHECK_EQUAL(properties[5]->writable(object),  true);
    BOOST_CHECK_EQUAL(properties[6]->writable(object),  false);
    //BOOST_CHECK_EQUAL(properties[7]->writable(object),  true);
    //BOOST_CHECK_EQUAL(properties[8]->writable(object),  false);
    BOOST_CHECK_EQUAL(properties[9]->writable(object),  true);
    BOOST_CHECK_EQUAL(properties[10]->writable(object), false);
    BOOST_CHECK_EQUAL(properties[11]->writable(object), false);
    BOOST_CHECK_EQUAL(properties[12]->writable(object), true);
    BOOST_CHECK_EQUAL(properties[13]->writable(object), true);
    BOOST_CHECK_EQUAL(properties[14]->writable(object), true);
    BOOST_CHECK_EQUAL(properties[15]->writable(object), false);
    BOOST_CHECK_EQUAL(properties[16]->writable(object), false);
    BOOST_CHECK_EQUAL(properties[17]->writable(object), true);
    BOOST_CHECK_EQUAL(properties[18]->writable(object), false);
    BOOST_CHECK_EQUAL(properties[19]->writable(object), true);
    BOOST_CHECK_EQUAL(properties[20]->writable(object), true);
    BOOST_CHECK_EQUAL(properties[21]->writable(object), true);
    //BOOST_CHECK_EQUAL(properties[22]->writable(object), true);
    BOOST_CHECK_EQUAL(properties[23]->writable(object), false);
    BOOST_CHECK_EQUAL(properties[24]->writable(object), true);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(get)
{
    MyClass object;

    BOOST_CHECK_EQUAL(properties[1]->get(object),  ponder::Value(object.p1));
    BOOST_CHECK_EQUAL(properties[2]->get(object),  ponder::Value(object.p2));
    BOOST_CHECK_EQUAL(properties[3]->get(object),  ponder::Value(object.p3));
    BOOST_CHECK_EQUAL(properties[4]->get(object),  ponder::Value(object.p4));
    BOOST_CHECK_EQUAL(properties[5]->get(object),  ponder::Value(object.p5));
    BOOST_CHECK_EQUAL(properties[6]->get(object),  ponder::Value(object.p6));
    //BOOST_CHECK_EQUAL(properties[7]->get(object),  ponder::Value(object.p7_impl));
    //BOOST_CHECK_EQUAL(properties[8]->get(object),  ponder::Value(object.p8_impl));
    BOOST_CHECK_EQUAL(properties[9]->get(object),  ponder::Value(*object.p9));
    BOOST_CHECK_EQUAL(properties[10]->get(object), ponder::Value(object.p10));
    BOOST_CHECK_EQUAL(properties[11]->get(object), ponder::Value(object.p11));
    BOOST_CHECK_EQUAL(properties[12]->get(object), ponder::Value(object.p12));
    BOOST_CHECK_EQUAL(properties[13]->get(object), ponder::Value(object.p13));
    BOOST_CHECK_EQUAL(properties[14]->get(object), ponder::Value(object.inner.p14));
    BOOST_CHECK_EQUAL(properties[15]->get(object), ponder::Value(object.inner.p15));
    BOOST_CHECK_EQUAL(properties[16]->get(object), ponder::Value(object.inner.p16));
    BOOST_CHECK_EQUAL(properties[17]->get(object), ponder::Value(object.inner.p17));
    BOOST_CHECK_EQUAL(properties[18]->get(object), ponder::Value(object.p18));
    BOOST_CHECK_EQUAL(properties[19]->get(object), ponder::Value(object.p19));
    BOOST_CHECK_EQUAL(properties[20]->get(object), ponder::Value(object.p20));
    BOOST_CHECK_EQUAL(properties[21]->get(object), ponder::Value(object.p21));
    //BOOST_CHECK_EQUAL(properties[22]->get(object), ponder::Value(object.p22));
    BOOST_CHECK_EQUAL(properties[23]->get(object), ponder::Value(object.getP23("str")));
    BOOST_CHECK_EQUAL(properties[24]->get(object), ponder::Value(object.p24));
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(set)
{
    MyClass object;

    BOOST_CHECK_THROW(properties[1]->set(object, false),  ponder::ForbiddenWrite);
    BOOST_CHECK_THROW(properties[2]->set(object, -2),     ponder::ForbiddenWrite);
    BOOST_CHECK_THROW(properties[6]->set(object, -6),     ponder::ForbiddenWrite);
    //BOOST_CHECK_THROW(properties[8]->set(object, Two),  ponder::ForbiddenWrite);
    BOOST_CHECK_THROW(properties[10]->set(object, false), ponder::ForbiddenWrite);
    BOOST_CHECK_THROW(properties[11]->set(object, -11),   ponder::ForbiddenWrite);
    BOOST_CHECK_THROW(properties[15]->set(object, -15),   ponder::ForbiddenWrite);
    BOOST_CHECK_THROW(properties[16]->set(object, "-16"), ponder::ForbiddenWrite);
    BOOST_CHECK_THROW(properties[18]->set(object, false), ponder::ForbiddenWrite);
    BOOST_CHECK_THROW(properties[23]->set(object, "-23"), ponder::ForbiddenWrite);

    properties[3]->set(object,  "-3");
    properties[4]->set(object,  MyType(-4));
    properties[5]->set(object,  true);
    //properties[7]->set(object,  "-7");
    properties[9]->set(object,  MyType(-9));
    properties[12]->set(object, "-12");
    properties[13]->set(object, Zero);
    properties[14]->set(object, false);
    properties[17]->set(object, MyType(-17));
    properties[19]->set(object, -19);
    properties[20]->set(object, -20.);
    properties[21]->set(object, -21);
    //properties[22]->set(object, One);
    properties[24]->set(object, MyType(-24));

    BOOST_CHECK_EQUAL(object.p3,        "-3");
    BOOST_CHECK_EQUAL(object.p4,        MyType(-4));
    BOOST_CHECK_EQUAL(object.p5,        true);
    //BOOST_CHECK_EQUAL(object.p7,        "-7");
    BOOST_CHECK_EQUAL(*object.p9,       MyType(-9));
    BOOST_CHECK_EQUAL(object.p12,       "-12");
    BOOST_CHECK_EQUAL(object.p13,       Zero);
    BOOST_CHECK_EQUAL(object.inner.p14, false);
    BOOST_CHECK_EQUAL(object.inner.p17, MyType(-17));
    BOOST_CHECK_EQUAL(object.p19,       -19);
    BOOST_CHECK_EQUAL(object.p20,       -20.);
    BOOST_CHECK_EQUAL(object.p21,       -21);
    //BOOST_CHECK_EQUAL(object.p22,       One);
    BOOST_CHECK_EQUAL(object.p24,       MyType(-24));
}

BOOST_AUTO_TEST_SUITE_END()
