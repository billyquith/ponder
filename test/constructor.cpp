/****************************************************************************
**
** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** CAMP is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** 
** CAMP is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public License
** along with CAMP.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/


#include "constructor.hpp"
#include <camp/class.hpp>
#include <camp/classget.hpp>
#include <camp/errors.hpp>
#include <boost/test/unit_test.hpp>

using namespace ConstructorTest;

//-----------------------------------------------------------------------------
struct ConstructorFixture
{
    ConstructorFixture()
    {
        camp::Enum::declare<MyEnum>("MyEnum")
            .value("zero",  zero)
            .value("one",   one)
            .value("two",   two)
            .value("three", three)
            .value("four",  four)
            .value("five",  five);

        camp::Class::declare<MyType>("MyType");

        camp::Class::declare<MyBase1>("MyBase1");
        camp::Class::declare<MyBase2>("MyBase2");

        camp::Class::declare<MyClass>("MyClass")
            .base<MyBase1>()
            .base<MyBase2>()
            .constructor0()
            .constructor1<long>()
            .constructor2<long, double>()
            .constructor3<long, double, std::string>()
            .constructor4<long, double, std::string, MyEnum>()

            // trying types that don't exactly match those declared
            .constructor5<unsigned short, float, std::string, MyEnum, int>()
            ;

        metaclass = &camp::classByType<MyClass>();
    }

    ~ConstructorFixture()
    {
        camp::Class::undeclare<MyClass>();
        camp::Class::undeclare<MyBase2>();
        camp::Class::undeclare<MyBase1>();
        camp::Class::undeclare<MyType>();
        camp::Enum::undeclare<MyEnum>();
    }

    const camp::Class* metaclass;
};


//-----------------------------------------------------------------------------
//                         Tests for camp::Constructor
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(CONSTRUCTOR, ConstructorFixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(invalidConstructions)
{
    BOOST_CHECK_THROW(metaclass->construct<MyClass>(camp::Args("hello")),         camp::ConstructorNotFound);
    BOOST_CHECK_THROW(metaclass->construct<MyClass>(camp::Args(MyType(10))),      camp::ConstructorNotFound);
    BOOST_CHECK_THROW(metaclass->construct<MyClass>(camp::Args(two, MyType(10))), camp::ConstructorNotFound);
    BOOST_CHECK_THROW(metaclass->construct<MyClass>(camp::Args(5., "hello")),     camp::ConstructorNotFound);

    BOOST_CHECK_THROW(metaclass->construct<MyType>(), camp::ClassUnrelated);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(zeroArg)
{
    MyClass* object = metaclass->construct<MyClass>();

    BOOST_CHECK(object != 0);

    BOOST_CHECK_EQUAL(object->l, 0);
    BOOST_CHECK_CLOSE(object->r, 0., 1E-5);
    BOOST_CHECK_EQUAL(object->s, "0");
    BOOST_CHECK_EQUAL(object->e, zero);
    BOOST_CHECK_EQUAL(object->u.x, 0);

    metaclass->destroy(object);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(oneArg)
{
    MyClass* object = metaclass->construct<MyClass>(camp::Args(1));

    BOOST_CHECK(object != 0);

    BOOST_CHECK_EQUAL(object->l, 1);
    BOOST_CHECK_CLOSE(object->r, 1., 1E-5);
    BOOST_CHECK_EQUAL(object->s, "1");
    BOOST_CHECK_EQUAL(object->e, one);
    BOOST_CHECK_EQUAL(object->u.x, 1);

    metaclass->destroy(object);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(twoArgs)
{
    MyClass* object = metaclass->construct<MyClass>(camp::Args(2, 2.));

    BOOST_CHECK(object != 0);

    BOOST_CHECK_EQUAL(object->l, 2);
    BOOST_CHECK_CLOSE(object->r, 2., 1E-5);
    BOOST_CHECK_EQUAL(object->s, "2");
    BOOST_CHECK_EQUAL(object->e, two);
    BOOST_CHECK_EQUAL(object->u.x, 2);

    metaclass->destroy(object);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(threeArgs)
{
    MyClass* object = metaclass->construct<MyClass>(camp::Args(3, 3., "3"));

    BOOST_CHECK(object != 0);

    BOOST_CHECK_EQUAL(object->l, 3);
    BOOST_CHECK_CLOSE(object->r, 3., 1E-5);
    BOOST_CHECK_EQUAL(object->s, "3");
    BOOST_CHECK_EQUAL(object->e, three);
    BOOST_CHECK_EQUAL(object->u.x, 3);

    metaclass->destroy(object);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(fourArgs)
{
    MyClass* object = metaclass->construct<MyClass>(camp::Args(4, 4., "4", four));

    BOOST_CHECK(object != 0);

    BOOST_CHECK_EQUAL(object->l, 4);
    BOOST_CHECK_CLOSE(object->r, 4., 1E-5);
    BOOST_CHECK_EQUAL(object->s, "4");
    BOOST_CHECK_EQUAL(object->e, four);
    BOOST_CHECK_EQUAL(object->u.x, 4);

    metaclass->destroy(object);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(fiveArgs)
{
    MyClass* object = metaclass->construct<MyClass>(camp::Args(5, 5., "5", five, 5));

    BOOST_CHECK(object != 0);

    BOOST_CHECK_EQUAL(object->l, 5);
    BOOST_CHECK_CLOSE(object->r, 5., 1E-5);
    BOOST_CHECK_EQUAL(object->s, "5");
    BOOST_CHECK_EQUAL(object->e, five);
    BOOST_CHECK_EQUAL(object->u.x, 5);

    metaclass->destroy(object);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(constructAsBase)
{
    MyBase1* object1 = metaclass->construct<MyBase1>();
    MyBase2* object2 = metaclass->construct<MyBase2>();

    // Make sure that the pointers are pointing to the right location in memory
    BOOST_CHECK_EQUAL(object1->base1, "base1");
    BOOST_CHECK_EQUAL(object2->base2, "base2");

    metaclass->destroy(object1);
    metaclass->destroy(object2);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(constructAsVoid)
{
    MyClass* object = static_cast<MyClass*>(metaclass->construct<void>());

    BOOST_CHECK_EQUAL(object->l, 0);
    BOOST_CHECK_CLOSE(object->r, 0., 1E-5);
    BOOST_CHECK_EQUAL(object->s, "0");
    BOOST_CHECK_EQUAL(object->e, zero);
    BOOST_CHECK_EQUAL(object->u.x, 0);

    metaclass->destroy(object);
}

BOOST_AUTO_TEST_SUITE_END()
