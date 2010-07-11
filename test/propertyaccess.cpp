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

#include "propertyaccess.hpp"
#include <camp/classget.hpp>
#include <camp/property.hpp>
#include <camp/errors.hpp>
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
