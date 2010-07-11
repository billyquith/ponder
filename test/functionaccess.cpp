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

#include "functionaccess.hpp"
#include <camp/classget.hpp>
#include <camp/function.hpp>
#include <camp/errors.hpp>
#include <boost/test/unit_test.hpp>

using namespace FunctionAccessTest;

//-----------------------------------------------------------------------------
struct FunctionAccessFixture
{
    FunctionAccessFixture()
        : object_t(true)
        , object_f(false)
    {
        metaclass = &camp::classByType<MyClass>();
    }

    MyClass object_t;
    MyClass object_f;
    const camp::Class* metaclass;
};

//-----------------------------------------------------------------------------
//                         Tests for camp::Function callable
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(FUNCTIONACCESS, FunctionAccessFixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(callableStatic)
{
    BOOST_CHECK_EQUAL(metaclass->function("f0").callable(object_t), false);
    BOOST_CHECK_EQUAL(metaclass->function("f0").callable(object_f), false);
    BOOST_CHECK_EQUAL(metaclass->function("f1").callable(object_t), true);
    BOOST_CHECK_EQUAL(metaclass->function("f1").callable(object_f), true);
    BOOST_CHECK_EQUAL(metaclass->function("f2").callable(object_t), true);
    BOOST_CHECK_EQUAL(metaclass->function("f2").callable(object_f), true);
    BOOST_CHECK_EQUAL(metaclass->function("f3").callable(object_t), false);
    BOOST_CHECK_EQUAL(metaclass->function("f3").callable(object_f), false);
    BOOST_CHECK_EQUAL(metaclass->function("f4").callable(object_t), true);
    BOOST_CHECK_EQUAL(metaclass->function("f4").callable(object_f), true);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(callableDynamic)
{
    BOOST_CHECK_EQUAL(metaclass->function("f5").callable(object_t), true);
    BOOST_CHECK_EQUAL(metaclass->function("f5").callable(object_f), false);
    BOOST_CHECK_EQUAL(metaclass->function("f6").callable(object_t), true);
    BOOST_CHECK_EQUAL(metaclass->function("f6").callable(object_f), false);
}

BOOST_AUTO_TEST_SUITE_END()
