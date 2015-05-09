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

#include "propertyaccess.hpp"
#include <camp/classget.hpp>
#include <camp/property.hpp>
#include <boost/test/unit_test.hpp>

using namespace PropertyAccessTest;

//-----------------------------------------------------------------------------
struct PropertyAccessFixture
{
    PropertyAccessFixture()
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
//                         Tests for camp::Property readable/writable
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(PROPERTYACCESS, PropertyAccessFixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(readableImplicit)
{
    BOOST_CHECK_EQUAL(metaclass->property("p8").readable(object_t), true);
    BOOST_CHECK_EQUAL(metaclass->property("p8").readable(object_f), true);
    BOOST_CHECK_EQUAL(metaclass->property("p9").readable(object_t), true);
    BOOST_CHECK_EQUAL(metaclass->property("p9").readable(object_f), true);
    BOOST_CHECK_EQUAL(metaclass->property("p10").readable(object_t), true);
    BOOST_CHECK_EQUAL(metaclass->property("p10").readable(object_f), true);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(readableStatic)
{
    BOOST_CHECK_EQUAL(metaclass->property("p0").readable(object_t), false);
    BOOST_CHECK_EQUAL(metaclass->property("p0").readable(object_f), false);
    BOOST_CHECK_EQUAL(metaclass->property("p1").readable(object_t), true);
    BOOST_CHECK_EQUAL(metaclass->property("p1").readable(object_f), true);
    BOOST_CHECK_EQUAL(metaclass->property("p2").readable(object_t), false);
    BOOST_CHECK_EQUAL(metaclass->property("p2").readable(object_f), false);
    BOOST_CHECK_EQUAL(metaclass->property("p3").readable(object_t), true);
    BOOST_CHECK_EQUAL(metaclass->property("p3").readable(object_f), true);
    BOOST_CHECK_EQUAL(metaclass->property("p4").readable(object_t), false);
    BOOST_CHECK_EQUAL(metaclass->property("p4").readable(object_f), false);
    BOOST_CHECK_EQUAL(metaclass->property("p5").readable(object_t), true);
    BOOST_CHECK_EQUAL(metaclass->property("p5").readable(object_f), true);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(readableDynamic)
{
    BOOST_CHECK_EQUAL(metaclass->property("p6").readable(object_t), true);
    BOOST_CHECK_EQUAL(metaclass->property("p6").readable(object_f), false);
    BOOST_CHECK_EQUAL(metaclass->property("p7").readable(object_t), true);
    BOOST_CHECK_EQUAL(metaclass->property("p7").readable(object_f), false);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(writableImplicit)
{
    BOOST_CHECK_EQUAL(metaclass->property("p8").writable(object_t), false);
    BOOST_CHECK_EQUAL(metaclass->property("p8").writable(object_f), false);
    BOOST_CHECK_EQUAL(metaclass->property("p9").writable(object_t), true);
    BOOST_CHECK_EQUAL(metaclass->property("p9").writable(object_f), true);
    BOOST_CHECK_EQUAL(metaclass->property("p10").writable(object_t), true);
    BOOST_CHECK_EQUAL(metaclass->property("p10").writable(object_f), true);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(writableStatic)
{
    BOOST_CHECK_EQUAL(metaclass->property("p0").writable(object_t), true);
    BOOST_CHECK_EQUAL(metaclass->property("p0").writable(object_f), true);
    BOOST_CHECK_EQUAL(metaclass->property("p1").writable(object_t), false);
    BOOST_CHECK_EQUAL(metaclass->property("p1").writable(object_f), false);
    BOOST_CHECK_EQUAL(metaclass->property("p2").writable(object_t), false);
    BOOST_CHECK_EQUAL(metaclass->property("p2").writable(object_f), false);
}

BOOST_AUTO_TEST_SUITE_END()
