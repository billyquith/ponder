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

#include "mapper.hpp"
#include <camp/classget.hpp>
#include <boost/test/unit_test.hpp>

using namespace MapperTest;

//-----------------------------------------------------------------------------
struct MapperFixture
{
    MapperFixture()
    {
        metaclass = &camp::classByType<MyClass>();
    }

    const camp::Class* metaclass;
};

//-----------------------------------------------------------------------------
//                         Tests for mappers
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(MAPPER, MapperFixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(count)
{
    BOOST_CHECK_EQUAL(metaclass->propertyCount(), MyClass::propertyCount);
    BOOST_CHECK_EQUAL(metaclass->functionCount(), MyClass::functionCount);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(types)
{
    BOOST_CHECK_EQUAL(metaclass->property(0).type(), camp::intType);
    BOOST_CHECK_EQUAL(metaclass->function(0).returnType(), camp::stringType);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(argCount)
{
    BOOST_CHECK_EQUAL(metaclass->function(0).argCount(), 0);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(propertyGet)
{
    MyClass object;

    BOOST_CHECK_EQUAL(metaclass->property("prop0").get(object), camp::Value(object.prop("prop0")));
    BOOST_CHECK_EQUAL(metaclass->property("prop1").get(object), camp::Value(object.prop("prop1")));
    BOOST_CHECK_EQUAL(metaclass->property("prop2").get(object), camp::Value(object.prop("prop2")));
    BOOST_CHECK_EQUAL(metaclass->property("prop3").get(object), camp::Value(object.prop("prop3")));
    BOOST_CHECK_EQUAL(metaclass->property("prop4").get(object), camp::Value(object.prop("prop4")));
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(propertySet)
{
    MyClass object;

    metaclass->property("prop0").set(object, 0);
    metaclass->property("prop1").set(object, 100);
    metaclass->property("prop2").set(object, 200);
    metaclass->property("prop3").set(object, 300);
    metaclass->property("prop4").set(object, 400);

    BOOST_CHECK_EQUAL(object.prop("prop0"), 0);
    BOOST_CHECK_EQUAL(object.prop("prop1"), 100);
    BOOST_CHECK_EQUAL(object.prop("prop2"), 200);
    BOOST_CHECK_EQUAL(object.prop("prop3"), 300);
    BOOST_CHECK_EQUAL(object.prop("prop4"), 400);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(functionCall)
{
    MyClass object;

    BOOST_CHECK_EQUAL(metaclass->function("func0").call(object), camp::Value(object.func("func0")));
    BOOST_CHECK_EQUAL(metaclass->function("func1").call(object), camp::Value(object.func("func1")));
    BOOST_CHECK_EQUAL(metaclass->function("func2").call(object), camp::Value(object.func("func2")));
}

BOOST_AUTO_TEST_SUITE_END()
