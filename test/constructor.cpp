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
#include <camp/classget.hpp>
#include <camp/errors.hpp>
#include <boost/test/unit_test.hpp>

using namespace ConstructorTest;

//-----------------------------------------------------------------------------
struct ConstructorFixture
{
    ConstructorFixture()
    {
        metaclass = &camp::classByType<MyClass>();
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
    BOOST_CHECK_EQUAL(metaclass->construct(camp::Args("hello")),         camp::UserObject::nothing);
    BOOST_CHECK_EQUAL(metaclass->construct(camp::Args(MyType(10))),      camp::UserObject::nothing);
    BOOST_CHECK_EQUAL(metaclass->construct(camp::Args(two, MyType(10))), camp::UserObject::nothing);
    BOOST_CHECK_EQUAL(metaclass->construct(camp::Args(5., "hello")),     camp::UserObject::nothing);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(zeroArg)
{
    camp::UserObject object = metaclass->construct();

    BOOST_CHECK(object != camp::UserObject::nothing);

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
    camp::UserObject object = metaclass->construct(camp::Args(1)).get<MyClass*>();

    BOOST_CHECK(object != camp::UserObject::nothing);

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
    camp::UserObject object = metaclass->construct(camp::Args(2, 2.));

    BOOST_CHECK(object != camp::UserObject::nothing);

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
    camp::UserObject object = metaclass->construct(camp::Args(3, 3., "3"));

    BOOST_CHECK(object != camp::UserObject::nothing);

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
    camp::UserObject object = metaclass->construct(camp::Args(4, 4., "4", four));

    BOOST_CHECK(object != camp::UserObject::nothing);

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
    camp::UserObject object = metaclass->construct(camp::Args(5, 5., "5", five, 5));

    BOOST_CHECK(object != camp::UserObject::nothing);

    MyClass* instance = object.get<MyClass*>();

    BOOST_CHECK_EQUAL(instance->l, 5);
    BOOST_CHECK_CLOSE(instance->r, 5., 1E-5);
    BOOST_CHECK_EQUAL(instance->s, "5");
    BOOST_CHECK_EQUAL(instance->e, five);
    BOOST_CHECK_EQUAL(instance->u.x, 5);

    metaclass->destroy(object);
}

BOOST_AUTO_TEST_SUITE_END()
