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

#include "enumobject.hpp"
#include <camp/enumget.hpp>
#include <camp/enumobject.hpp>
#include <camp/errors.hpp>
#include <boost/test/unit_test.hpp>

using namespace EnumObjectTest;

struct EnumObjectFixture
{
    EnumObjectFixture()
        : zero(Zero)
        , one(One)
        , two(Two)
    {
    }

    camp::EnumObject zero;
    camp::EnumObject one;
    camp::EnumObject two;
};

//-----------------------------------------------------------------------------
//                         Tests for camp::EnumObject
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(ENUMOBJECT, EnumObjectFixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(error)
{
    // The meta-enum of MyUndeclaredEnum is *not* declared

    BOOST_CHECK_THROW(camp::EnumObject obj(Undeclared), camp::EnumNotFound);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(value)
{
    BOOST_CHECK_EQUAL(zero.value(), Zero);
    BOOST_CHECK_EQUAL(one.value(),  One);
    BOOST_CHECK_EQUAL(two.value(),  Two);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(name)
{
    BOOST_CHECK_EQUAL(zero.name(), "Zero");
    BOOST_CHECK_EQUAL(one.name(),  "One");
    BOOST_CHECK_EQUAL(two.name(),  "Two");
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(getEnum)
{
    BOOST_CHECK(zero.getEnum() == camp::enumByType<MyEnum>());
    BOOST_CHECK(one.getEnum()  == camp::enumByType<MyEnum>());
    BOOST_CHECK(two.getEnum()  == camp::enumByType<MyEnum>());
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(equal)
{
    // Setup
    camp::EnumObject zero2(Zero2);
    camp::EnumObject one2(One2);
    camp::EnumObject two2(Two2);

    // Tests
    BOOST_CHECK_EQUAL(zero == camp::EnumObject(Zero), true);
    BOOST_CHECK_EQUAL(one  == camp::EnumObject(One),  true);
    BOOST_CHECK_EQUAL(two  == camp::EnumObject(Two),  true);

    BOOST_CHECK_EQUAL(zero == one,  false);
    BOOST_CHECK_EQUAL(one  == two,  false);
    BOOST_CHECK_EQUAL(two  == zero, false);

    BOOST_CHECK_EQUAL(zero == zero2, false); // same value and name, different metaenum
    BOOST_CHECK_EQUAL(one  == one2,  false);
    BOOST_CHECK_EQUAL(two  == two2,  false);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(lessThan)
{
    BOOST_CHECK_EQUAL(zero < zero, false);
    BOOST_CHECK_EQUAL(zero < one,  true);
    BOOST_CHECK_EQUAL(zero < two,  true);

    BOOST_CHECK_EQUAL(one < zero, false);
    BOOST_CHECK_EQUAL(one < one,  false);
    BOOST_CHECK_EQUAL(one < two,  true);

    BOOST_CHECK_EQUAL(two < zero, false);
    BOOST_CHECK_EQUAL(two < one,  false);
    BOOST_CHECK_EQUAL(two < two,  false);
}

BOOST_AUTO_TEST_SUITE_END()
