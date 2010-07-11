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

#include "functionmapping.hpp"
#include <camp/class.hpp>
#include <camp/classget.hpp>
#include <boost/test/unit_test.hpp>

using namespace FunctionMappingTest;

//-----------------------------------------------------------------------------
struct FunctionMappingFixture
{
    FunctionMappingFixture()
    {
        metaclass = &camp::classByType<MyClass>();
    }

    MyClass object;
    const camp::Class* metaclass;
};


//-----------------------------------------------------------------------------
//                         Tests for camp_ext::QtMapper (functions)
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(FUNCTIONMAPPING, FunctionMappingFixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(name)
{
    BOOST_CHECK(metaclass->hasFunction("f1"));
    BOOST_CHECK(metaclass->hasFunction("f2"));
    BOOST_CHECK(metaclass->hasFunction("f3"));
    BOOST_CHECK(metaclass->hasFunction("f4"));
    BOOST_CHECK(metaclass->hasFunction("f5"));
    BOOST_CHECK(metaclass->hasFunction("f6"));
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(returnType)
{
    BOOST_CHECK_EQUAL(metaclass->function("f1").returnType(), camp::noType);
    BOOST_CHECK_EQUAL(metaclass->function("f2").returnType(), camp::boolType);
    BOOST_CHECK_EQUAL(metaclass->function("f3").returnType(), camp::intType);
    BOOST_CHECK_EQUAL(metaclass->function("f4").returnType(), camp::intType);
    BOOST_CHECK_EQUAL(metaclass->function("f5").returnType(), camp::realType);
    BOOST_CHECK_EQUAL(metaclass->function("f6").returnType(), camp::stringType);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(argumentsCount)
{
    BOOST_CHECK_EQUAL(metaclass->function("f1").argCount(), 0);
    BOOST_CHECK_EQUAL(metaclass->function("f2").argCount(), 1);
    BOOST_CHECK_EQUAL(metaclass->function("f3").argCount(), 2);
    BOOST_CHECK_EQUAL(metaclass->function("f4").argCount(), 3);
    BOOST_CHECK_EQUAL(metaclass->function("f5").argCount(), 4);
    BOOST_CHECK_EQUAL(metaclass->function("f6").argCount(), 5);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(call)
{
    BOOST_CHECK_EQUAL(metaclass->function("f1").call(object), camp::Value::nothing);
    BOOST_CHECK_EQUAL(metaclass->function("f2").call(object, camp::Args(true)).to<bool>(), true);
    BOOST_CHECK_EQUAL(metaclass->function("f3").call(object, camp::Args(-1, -9)).to<int>(), -10);
    BOOST_CHECK_EQUAL(metaclass->function("f4").call(object, camp::Args(1, 4, 15)).to<unsigned long>(), 20);
    BOOST_CHECK_CLOSE(metaclass->function("f5").call(object, camp::Args(1., 2., 3., .5)).to<double>(), 6.5, 1E-5);
    BOOST_CHECK_EQUAL(metaclass->function("f6").call(object, camp::Args("h", "e", "l", "l", "o")).to<QString>(), "hello");
}

BOOST_AUTO_TEST_SUITE_END()
