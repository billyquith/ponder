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

#include "enum.hpp"
#include <ponder/enumget.hpp>
#include <ponder/errors.hpp>
#include <boost/test/unit_test.hpp>

using namespace EnumTest;

//-----------------------------------------------------------------------------
struct EnumFixture
{
    EnumFixture()
    {
        metaenum = &ponder::enumByType<MyEnum>();
    }

    const ponder::Enum* metaenum;
};

//-----------------------------------------------------------------------------
//                         Tests for ponder::Enum
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(ENUM, EnumFixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(declare)
{
    std::size_t count = ponder::enumCount();

    ponder::Enum::declare<MyTempEnum>("EnumTest::MyTempEnum");

    BOOST_CHECK_EQUAL(ponder::enumCount(), count + 1);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(declareExceptions)
{
    // to make sure it is declared
    ponder::enumByType<MyEnum>();

    BOOST_CHECK_THROW(ponder::Enum::declare<MyEnum>(), ponder::EnumAlreadyCreated);
    BOOST_CHECK_THROW(ponder::Enum::declare<MyUndeclaredEnum>("EnumTest::MyEnum"), ponder::EnumAlreadyCreated);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(get)
{
    MyEnum object = MyEnum();
    MyUndeclaredEnum object2 = MyUndeclaredEnum();

    BOOST_CHECK_EQUAL(ponder::enumByName("EnumTest::MyEnum").name(), "EnumTest::MyEnum");
    BOOST_CHECK_EQUAL(ponder::enumByType<MyEnum>().name(),           "EnumTest::MyEnum");
    BOOST_CHECK_EQUAL(ponder::enumByObject(object).name(),           "EnumTest::MyEnum");
    BOOST_CHECK_EQUAL(ponder::enumByObject(&object).name(),          "EnumTest::MyEnum");
    BOOST_CHECK_EQUAL(ponder::enumByTypeSafe<MyUndeclaredEnum>(),    static_cast<ponder::Enum*>(0));

    BOOST_CHECK_THROW(ponder::enumByName("EnumTest::MyUndeclaredEnum"), ponder::EnumNotFound);
    BOOST_CHECK_THROW(ponder::enumByType<MyUndeclaredEnum>(),           ponder::EnumNotFound);
    BOOST_CHECK_THROW(ponder::enumByObject(object2),                    ponder::EnumNotFound);
    BOOST_CHECK_THROW(ponder::enumByObject(&object2),                   ponder::EnumNotFound);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(comparisons)
{
    BOOST_CHECK(ponder::enumByType<MyEnum>()  == ponder::enumByType<MyEnum>());
    BOOST_CHECK(ponder::enumByType<MyEnum>()  != ponder::enumByType<MyEnum2>());
    BOOST_CHECK(ponder::enumByType<MyEnum2>() != ponder::enumByType<MyEnum>());
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(pairs)
{
    BOOST_CHECK_EQUAL(metaenum->size(), 3U);

    BOOST_CHECK_EQUAL(metaenum->pair(0).name, "Zero");
    BOOST_CHECK_EQUAL(metaenum->pair(0).value, Zero);
    BOOST_CHECK_EQUAL(metaenum->pair(1).name, "One");
    BOOST_CHECK_EQUAL(metaenum->pair(1).value, One);
    BOOST_CHECK_EQUAL(metaenum->pair(2).name, "Two");
    BOOST_CHECK_EQUAL(metaenum->pair(2).value, Two);
    BOOST_CHECK_THROW(metaenum->pair(3), ponder::OutOfRange);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(values)
{
    BOOST_CHECK_EQUAL(metaenum->hasValue(Zero), true);
    BOOST_CHECK_EQUAL(metaenum->hasValue(One),  true);
    BOOST_CHECK_EQUAL(metaenum->hasValue(Two),  true);
    BOOST_CHECK_EQUAL(metaenum->hasValue(100),  false);

    BOOST_CHECK_EQUAL(metaenum->value("Zero"), Zero);
    BOOST_CHECK_EQUAL(metaenum->value("One"),  One);
    BOOST_CHECK_EQUAL(metaenum->value("Two"),  Two);
    BOOST_CHECK_THROW(metaenum->value("xxx"),  ponder::EnumNameNotFound);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(names)
{
    BOOST_CHECK_EQUAL(metaenum->hasName("Zero"),    true);
    BOOST_CHECK_EQUAL(metaenum->hasName("One"),     true);
    BOOST_CHECK_EQUAL(metaenum->hasName("Two"),     true);
    BOOST_CHECK_EQUAL(metaenum->hasName("Hundred"), false);

    BOOST_CHECK_EQUAL(metaenum->name(Zero), "Zero");
    BOOST_CHECK_EQUAL(metaenum->name(One),  "One");
    BOOST_CHECK_EQUAL(metaenum->name(Two),  "Two");
    BOOST_CHECK_THROW(metaenum->name(100),  ponder::EnumValueNotFound);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(enumClass)
{
    const ponder::Enum *clsenum = &ponder::enumByType<MyEnumClass>();
    BOOST_CHECK_EQUAL(clsenum->hasValue(MyEnumClass::Red), true);
    BOOST_CHECK_EQUAL(clsenum->hasValue(MyEnumClass::Green), true);
    BOOST_CHECK_EQUAL(clsenum->hasValue(MyEnumClass::Blue), true);
    BOOST_CHECK_EQUAL(clsenum->hasValue(100),  false);
    
    BOOST_CHECK_EQUAL(clsenum->value<MyEnumClass>("Red") == MyEnumClass::Red, true);
    BOOST_CHECK_EQUAL(clsenum->value<MyEnumClass>("Green") == MyEnumClass::Green, true);
    BOOST_CHECK_EQUAL(clsenum->value<MyEnumClass>("Blue") == MyEnumClass::Blue, true);
    BOOST_CHECK_EQUAL(clsenum->value<MyEnumClass>("Blue") == MyEnumClass::Red, false);
    BOOST_CHECK_THROW(clsenum->value("xxx"),  ponder::EnumNameNotFound);
}

BOOST_AUTO_TEST_SUITE_END()
