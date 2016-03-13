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

#include "functionmapping.hpp"
#include <ponder/class.hpp>
#include <ponder/classget.hpp>
#include "catch.hpp"

using namespace FunctionMappingTest;

struct FunctionMappingFixture
{
    FunctionMappingFixture()
    {
        metaclass = &ponder::classByType<MyClass>();
    }

    MyClass object;
    const ponder::Class* metaclass;
};


//-----------------------------------------------------------------------------
//                         Tests for camp_ext::QtMapper (functions)
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(FUNCTIONMAPPING, FunctionMappingFixture)

BOOST_AUTO_TEST_CASE(name)
{
    BOOST_CHECK(metaclass->hasFunction("f1"));
    BOOST_CHECK(metaclass->hasFunction("f2"));
    BOOST_CHECK(metaclass->hasFunction("f3"));
    BOOST_CHECK(metaclass->hasFunction("f4"));
    BOOST_CHECK(metaclass->hasFunction("f5"));
    BOOST_CHECK(metaclass->hasFunction("f6"));
}

BOOST_AUTO_TEST_CASE(returnType)
{
    BOOST_CHECK_EQUAL(metaclass->function("f1").returnType(), ponder::noType);
    BOOST_CHECK_EQUAL(metaclass->function("f2").returnType(), ponder::boolType);
    BOOST_CHECK_EQUAL(metaclass->function("f3").returnType(), ponder::intType);
    BOOST_CHECK_EQUAL(metaclass->function("f4").returnType(), ponder::intType);
    BOOST_CHECK_EQUAL(metaclass->function("f5").returnType(), ponder::realType);
    BOOST_CHECK_EQUAL(metaclass->function("f6").returnType(), ponder::stringType);
}

BOOST_AUTO_TEST_CASE(argumentsCount)
{
    BOOST_CHECK_EQUAL(metaclass->function("f1").argCount(), 0);
    BOOST_CHECK_EQUAL(metaclass->function("f2").argCount(), 1);
    BOOST_CHECK_EQUAL(metaclass->function("f3").argCount(), 2);
    BOOST_CHECK_EQUAL(metaclass->function("f4").argCount(), 3);
    BOOST_CHECK_EQUAL(metaclass->function("f5").argCount(), 4);
    BOOST_CHECK_EQUAL(metaclass->function("f6").argCount(), 5);
}

BOOST_AUTO_TEST_CASE(call)
{
    BOOST_CHECK_EQUAL(metaclass->function("f1").call(object), ponder::Value::nothing);
    BOOST_CHECK_EQUAL(metaclass->function("f2").call(object, ponder::Args(true)).to<bool>(), true);
    BOOST_CHECK_EQUAL(metaclass->function("f3").call(object, ponder::Args(-1, -9)).to<int>(), -10);
    BOOST_CHECK_EQUAL(metaclass->function("f4").call(object, ponder::Args(1, 4, 15)).to<unsigned long>(), 20);
    BOOST_CHECK_CLOSE(metaclass->function("f5").call(object, ponder::Args(1., 2., 3., .5)).to<double>(), 6.5, 1E-5);
    BOOST_CHECK_EQUAL(metaclass->function("f6").call(object, ponder::Args("h", "e", "l", "l", "o")).to<QString>(), "hello");
}

BOOST_AUTO_TEST_SUITE_END()
