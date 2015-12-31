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

#include "enumobject.hpp"
#include <ponder/errors.hpp>
#include <ponder/enumget.hpp>
#include <ponder/enumobject.hpp>
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

    ponder::EnumObject zero;
    ponder::EnumObject one;
    ponder::EnumObject two;
};

//-----------------------------------------------------------------------------
//                         Tests for ponder::EnumObject
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(ENUMOBJECT, EnumObjectFixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(error)
{
    // The meta-enum of MyUndeclaredEnum is *not* declared

    BOOST_CHECK_THROW(ponder::EnumObject obj(Undeclared), ponder::EnumNotFound);
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
    BOOST_CHECK(zero.getEnum() == ponder::enumByType<MyEnum>());
    BOOST_CHECK(one.getEnum()  == ponder::enumByType<MyEnum>());
    BOOST_CHECK(two.getEnum()  == ponder::enumByType<MyEnum>());
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(equal)
{
    // Setup
    ponder::EnumObject zero2(Zero2);
    ponder::EnumObject one2(One2);
    ponder::EnumObject two2(Two2);

    // Tests
    BOOST_CHECK_EQUAL(zero == ponder::EnumObject(Zero), true);
    BOOST_CHECK_EQUAL(one  == ponder::EnumObject(One),  true);
    BOOST_CHECK_EQUAL(two  == ponder::EnumObject(Two),  true);

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
