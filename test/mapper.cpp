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

#include "mapper.hpp"
#include <ponder/classget.hpp>
#include <boost/test/unit_test.hpp>

using namespace MapperTest;

//-----------------------------------------------------------------------------
struct MapperFixture
{
    MapperFixture()
    {
        metaclass = &ponder::classByType<MyClass>();
    }

    const ponder::Class* metaclass;
};

//-----------------------------------------------------------------------------
//                         Tests for mappers
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(MAPPER, MapperFixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(count)
{
    BOOST_CHECK_EQUAL(metaclass->propertyCount(), static_cast<std::size_t>(MyClass::propertyCount));
    BOOST_CHECK_EQUAL(metaclass->functionCount(), static_cast<std::size_t>(MyClass::functionCount));
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(types)
{
    BOOST_CHECK_EQUAL(metaclass->property(0).type(), ponder::intType);
    BOOST_CHECK_EQUAL(metaclass->function(0).returnType(), ponder::stringType);
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

    BOOST_CHECK_EQUAL(metaclass->property("prop0").get(object), ponder::Value(object.prop("prop0")));
    BOOST_CHECK_EQUAL(metaclass->property("prop1").get(object), ponder::Value(object.prop("prop1")));
    BOOST_CHECK_EQUAL(metaclass->property("prop2").get(object), ponder::Value(object.prop("prop2")));
    BOOST_CHECK_EQUAL(metaclass->property("prop3").get(object), ponder::Value(object.prop("prop3")));
    BOOST_CHECK_EQUAL(metaclass->property("prop4").get(object), ponder::Value(object.prop("prop4")));
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

    BOOST_CHECK_EQUAL(metaclass->function("func0").call(object), ponder::Value(object.func("func0")));
    BOOST_CHECK_EQUAL(metaclass->function("func1").call(object), ponder::Value(object.func("func1")));
    BOOST_CHECK_EQUAL(metaclass->function("func2").call(object), ponder::Value(object.func("func2")));
}

BOOST_AUTO_TEST_SUITE_END()
