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
#include <ponder/pondertype.hpp>
#include <ponder/class.hpp>
#include <ponder/enum.hpp>
#include <ponder/classbuilder.hpp>
#include <boost/test/unit_test.hpp>
#include <string>

namespace ConstructorTest
{
    enum MyEnum
    {
        zero  = 0,
        one   = 1,
        two   = 2,
        three = 3,
        four  = 4,
        five  = 5
    };
    
    struct MyType
    {
        MyType(int x_) : x(x_) {}
        int x;
    };
    
    struct MyBase1
    {
        MyBase1() : base1("base1") {}
        virtual ~MyBase1() {}
        std::string base1;
    };
    
    struct MyBase2
    {
        MyBase2() : base2("base2") {}
        virtual ~MyBase2() {}
        std::string base2;
    };
    
    struct MyClass : MyBase1, MyBase2
    {
        MyClass() : l(0), r(0.), s("0"), e(zero), u(0) {}
        MyClass(long l_) : l(l_), r(1.), s("1"), e(one), u(1) {}
        MyClass(long l_, double r_) : l(l_), r(r_), s("2"), e(two), u(2) {}
        MyClass(long l_, double r_, std::string s_) : l(l_), r(r_), s(s_), e(three), u(3) {}
        MyClass(long l_, double r_, std::string s_, MyEnum e_) : l(l_), r(r_), s(s_), e(e_), u(4) {}
        MyClass(long l_, double r_, std::string s_, MyEnum e_, MyType t_) : l(l_), r(r_), s(s_), e(e_), u(t_) {}
        
        long l;
        double r;
        std::string s;
        MyEnum e;
        MyType u;
    };
    
    void declare()
    {
        ponder::Enum::declare<MyEnum>("ConstructorTest::MyEnum")
        .value("zero",  zero)
        .value("one",   one)
        .value("two",   two)
        .value("three", three)
        .value("four",  four)
        .value("five",  five);
        
        ponder::Class::declare<MyType>("ConstructorTest::MyType");
        
        ponder::Class::declare<MyBase1>("ConstructorTest::MyBase1");
        ponder::Class::declare<MyBase2>("ConstructorTest::MyBase2");
        
        ponder::Class::declare<MyClass>("ConstructorTest::MyClass")
        .base<MyBase1>()
        .base<MyBase2>()
        .constructor()
        .constructor<long>()
        .constructor<long, double>()
        .constructor<long, double, std::string>()
        .constructor<long, double, std::string, MyEnum>()
        
        // trying types that don't exactly match those declared
        .constructor<unsigned short, float, std::string, MyEnum, int>();
    }
}

PONDER_AUTO_TYPE(ConstructorTest::MyEnum, &ConstructorTest::declare)
PONDER_AUTO_TYPE(ConstructorTest::MyType, &ConstructorTest::declare)
PONDER_AUTO_TYPE(ConstructorTest::MyBase1, &ConstructorTest::declare)
PONDER_AUTO_TYPE(ConstructorTest::MyBase2, &ConstructorTest::declare)
PONDER_AUTO_TYPE(ConstructorTest::MyClass, &ConstructorTest::declare)


using namespace ConstructorTest;


struct ConstructorFixture
{
    ConstructorFixture()
    {
        metaclass = &ponder::classByType<MyClass>();
    }

    const ponder::Class* metaclass;
};

//-----------------------------------------------------------------------------
//                         Tests for ponder::Constructor
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(CONSTRUCTOR, ConstructorFixture)


BOOST_AUTO_TEST_CASE(invalidConstructions)
{
    BOOST_CHECK_EQUAL(metaclass->construct(ponder::Args("hello")),         ponder::UserObject::nothing);
    BOOST_CHECK_EQUAL(metaclass->construct(ponder::Args(MyType(10))),      ponder::UserObject::nothing);
    BOOST_CHECK_EQUAL(metaclass->construct(ponder::Args(two, MyType(10))), ponder::UserObject::nothing);
    BOOST_CHECK_EQUAL(metaclass->construct(ponder::Args(5., "hello")),     ponder::UserObject::nothing);
}


BOOST_AUTO_TEST_CASE(zeroArg)
{
    ponder::UserObject object = metaclass->construct();

    BOOST_CHECK(object != ponder::UserObject::nothing);

    MyClass* instance = object.get<MyClass*>();

    BOOST_CHECK_EQUAL(instance->l, 0);
    BOOST_CHECK_CLOSE(instance->r, 0., 1E-5);
    BOOST_CHECK_EQUAL(instance->s, "0");
    BOOST_CHECK_EQUAL(instance->e, zero);
    BOOST_CHECK_EQUAL(instance->u.x, 0);

    metaclass->destroy(object);
}


BOOST_AUTO_TEST_CASE(oneArg)
{
    ponder::UserObject object = metaclass->construct(ponder::Args(1)).get<MyClass*>();

    BOOST_CHECK(object != ponder::UserObject::nothing);

    MyClass* instance = object.get<MyClass*>();

    BOOST_CHECK_EQUAL(instance->l, 1);
    BOOST_CHECK_CLOSE(instance->r, 1., 1E-5);
    BOOST_CHECK_EQUAL(instance->s, "1");
    BOOST_CHECK_EQUAL(instance->e, one);
    BOOST_CHECK_EQUAL(instance->u.x, 1);

    metaclass->destroy(object);
}


BOOST_AUTO_TEST_CASE(twoArgs)
{
    ponder::UserObject object = metaclass->construct(ponder::Args(2, 2.));

    BOOST_CHECK(object != ponder::UserObject::nothing);

    MyClass* instance = object.get<MyClass*>();

    BOOST_CHECK_EQUAL(instance->l, 2);
    BOOST_CHECK_CLOSE(instance->r, 2., 1E-5);
    BOOST_CHECK_EQUAL(instance->s, "2");
    BOOST_CHECK_EQUAL(instance->e, two);
    BOOST_CHECK_EQUAL(instance->u.x, 2);

    metaclass->destroy(object);
}


BOOST_AUTO_TEST_CASE(threeArgs)
{
    ponder::UserObject object = metaclass->construct(ponder::Args(3, 3., "3"));

    BOOST_CHECK(object != ponder::UserObject::nothing);

    MyClass* instance = object.get<MyClass*>();

    BOOST_CHECK_EQUAL(instance->l, 3);
    BOOST_CHECK_CLOSE(instance->r, 3., 1E-5);
    BOOST_CHECK_EQUAL(instance->s, "3");
    BOOST_CHECK_EQUAL(instance->e, three);
    BOOST_CHECK_EQUAL(instance->u.x, 3);

    metaclass->destroy(object);
}


BOOST_AUTO_TEST_CASE(fourArgs)
{
    ponder::UserObject object = metaclass->construct(ponder::Args(4, 4., "4", four));

    BOOST_CHECK(object != ponder::UserObject::nothing);

    MyClass* instance = object.get<MyClass*>();

    BOOST_CHECK_EQUAL(instance->l, 4);
    BOOST_CHECK_CLOSE(instance->r, 4., 1E-5);
    BOOST_CHECK_EQUAL(instance->s, "4");
    BOOST_CHECK_EQUAL(instance->e, four);
    BOOST_CHECK_EQUAL(instance->u.x, 4);

    metaclass->destroy(object);
}


BOOST_AUTO_TEST_CASE(fiveArgs)
{
    ponder::UserObject object = metaclass->construct(ponder::Args(5, 5., "5", five, 5));

    BOOST_CHECK(object != ponder::UserObject::nothing);

    MyClass* instance = object.get<MyClass*>();

    BOOST_CHECK_EQUAL(instance->l, 5);
    BOOST_CHECK_CLOSE(instance->r, 5., 1E-5);
    BOOST_CHECK_EQUAL(instance->s, "5");
    BOOST_CHECK_EQUAL(instance->e, five);
    BOOST_CHECK_EQUAL(instance->u.x, 5);

    metaclass->destroy(object);
}

BOOST_AUTO_TEST_SUITE_END()
