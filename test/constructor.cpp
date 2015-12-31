/****************************************************************************
**
** This file is part of the CAMP library.
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

#include "constructor.hpp"
#include <ponder/classget.hpp>
#include <boost/test/unit_test.hpp>

using namespace ConstructorTest;

//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(invalidConstructions)
{
    BOOST_CHECK_EQUAL(metaclass->construct(ponder::Args("hello")),         ponder::UserObject::nothing);
    BOOST_CHECK_EQUAL(metaclass->construct(ponder::Args(MyType(10))),      ponder::UserObject::nothing);
    BOOST_CHECK_EQUAL(metaclass->construct(ponder::Args(two, MyType(10))), ponder::UserObject::nothing);
    BOOST_CHECK_EQUAL(metaclass->construct(ponder::Args(5., "hello")),     ponder::UserObject::nothing);
}

//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
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
