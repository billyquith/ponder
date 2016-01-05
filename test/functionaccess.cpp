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

#include "functionaccess.hpp"
#include <ponder/classget.hpp>
#include <ponder/function.hpp>
#include <boost/test/unit_test.hpp>

using namespace FunctionAccessTest;

//-----------------------------------------------------------------------------
struct FunctionAccessFixture
{
    FunctionAccessFixture()
        : object_t(true)
        , object_f(false)
    {
        metaclass = &ponder::classByType<MyClass>();
    }

    MyClass object_t;
    MyClass object_f;
    const ponder::Class* metaclass;
};

//-----------------------------------------------------------------------------
//                         Tests for ponder::Function callable
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
