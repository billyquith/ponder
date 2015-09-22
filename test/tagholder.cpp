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

#include "tagholder.hpp"
#include <camp/classget.hpp>
#include <boost/test/unit_test.hpp>

using namespace TagHolderTest;

//-----------------------------------------------------------------------------
struct TagHolderFixture
{
    TagHolderFixture()
    {
        metaclass = &camp::classByType<MyClass>();
    }

    const camp::Class* metaclass;
};

//-----------------------------------------------------------------------------
//                         Tests for camp::TagHolder
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(TAGHOLDER, TagHolderFixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(declare)
{
    BOOST_CHECK_EQUAL(metaclass->tagCount(), 16);
    BOOST_CHECK_EQUAL(metaclass->property("prop").tagCount(), 2);
    BOOST_CHECK_EQUAL(metaclass->function("func").tagCount(), 2);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(get)
{
    BOOST_CHECK_EQUAL(metaclass->hasTag("a"), true);
    BOOST_CHECK_EQUAL(metaclass->hasTag("b"), true);
    BOOST_CHECK_EQUAL(metaclass->hasTag("x"), false);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(staticTags)
{
    BOOST_CHECK_EQUAL(metaclass->hasTag(true),    true);
    BOOST_CHECK_EQUAL(metaclass->hasTag(10),      true);
    BOOST_CHECK_EQUAL(metaclass->hasTag(5.25),    true);
    BOOST_CHECK_EQUAL(metaclass->hasTag("hello"), true);
    BOOST_CHECK_EQUAL(metaclass->hasTag(Ten),     true);
    BOOST_CHECK_EQUAL(metaclass->hasTag(object1), true);

    BOOST_CHECK_EQUAL(metaclass->hasTag(false),   false);
    BOOST_CHECK_EQUAL(metaclass->hasTag(20),      false);
    BOOST_CHECK_EQUAL(metaclass->hasTag(8.78),    false);
    BOOST_CHECK_EQUAL(metaclass->hasTag("hi"),    false);
    BOOST_CHECK_EQUAL(metaclass->hasTag(One),     false);
    BOOST_CHECK_EQUAL(metaclass->hasTag(object2), false);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(staticValues)
{
    BOOST_CHECK_EQUAL(metaclass->tag("static1"), camp::Value(true));
    BOOST_CHECK_EQUAL(metaclass->tag("static2"), camp::Value(10));
    BOOST_CHECK_EQUAL(metaclass->tag("static3"), camp::Value(5.25));
    BOOST_CHECK_EQUAL(metaclass->tag("static4"), camp::Value("hello"));
    BOOST_CHECK_EQUAL(metaclass->tag("static5"), camp::Value(Ten));
    BOOST_CHECK_EQUAL(metaclass->tag("static6"), camp::Value(object1));
    BOOST_CHECK_EQUAL(metaclass->tag("xxxxxxx"), camp::Value::nothing);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(dynamicValues)
{
    BOOST_CHECK_EQUAL(metaclass->tag("dynamic1", MyClass(10)), camp::Value(10));
    BOOST_CHECK_EQUAL(metaclass->tag("dynamic2", MyClass(10)), camp::Value("func"));
}

BOOST_AUTO_TEST_SUITE_END()
