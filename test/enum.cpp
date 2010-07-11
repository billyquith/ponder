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

#include "enum.hpp"
#include <camp/enumget.hpp>
#include <camp/errors.hpp>
#include <boost/test/unit_test.hpp>

using namespace EnumTest;

//-----------------------------------------------------------------------------
struct EnumFixture
{
    EnumFixture()
    {
        metaenum = &camp::enumByType<MyEnum>();
    }

    const camp::Enum* metaenum;
};

//-----------------------------------------------------------------------------
//                         Tests for camp::Enum
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(ENUM, EnumFixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(declare)
{
    std::size_t count = camp::enumCount();

    camp::Enum::declare<MyTempEnum>("EnumTest::MyTempEnum");

    BOOST_CHECK_EQUAL(camp::enumCount(), count + 1);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(declareExceptions)
{
    // to make sure it is declared
    camp::enumByType<MyEnum>();

    BOOST_CHECK_THROW(camp::Enum::declare<MyEnum>("EnumTest::MyUndeclaredEnum"), camp::EnumAlreadyCreated);
    BOOST_CHECK_THROW(camp::Enum::declare<MyUndeclaredEnum>("EnumTest::MyEnum"), camp::EnumAlreadyCreated);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(get)
{
    MyEnum object = MyEnum();
    MyUndeclaredEnum object2 = MyUndeclaredEnum();

    BOOST_CHECK_EQUAL(camp::enumByName("EnumTest::MyEnum").name(), "EnumTest::MyEnum");
    BOOST_CHECK_EQUAL(camp::enumByType<MyEnum>().name(),           "EnumTest::MyEnum");
    BOOST_CHECK_EQUAL(camp::enumByObject(object).name(),           "EnumTest::MyEnum");
    BOOST_CHECK_EQUAL(camp::enumByObject(&object).name(),          "EnumTest::MyEnum");
    BOOST_CHECK_EQUAL(camp::enumByTypeSafe<MyUndeclaredEnum>(),    static_cast<camp::Enum*>(0));

    BOOST_CHECK_THROW(camp::enumByName("EnumTest::MyUndeclaredEnum"), camp::EnumNotFound);
    BOOST_CHECK_THROW(camp::enumByType<MyUndeclaredEnum>(),           camp::EnumNotFound);
    BOOST_CHECK_THROW(camp::enumByObject(object2),                    camp::EnumNotFound);
    BOOST_CHECK_THROW(camp::enumByObject(&object2),                   camp::EnumNotFound);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(comparisons)
{
    BOOST_CHECK(camp::enumByType<MyEnum>()  == camp::enumByType<MyEnum>());
    BOOST_CHECK(camp::enumByType<MyEnum>()  != camp::enumByType<MyEnum2>());
    BOOST_CHECK(camp::enumByType<MyEnum2>() != camp::enumByType<MyEnum>());
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
    BOOST_CHECK_THROW(metaenum->pair(3), camp::OutOfRange);
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
    BOOST_CHECK_THROW(metaenum->value("xxx"),  camp::EnumNameNotFound);
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
    BOOST_CHECK_THROW(metaenum->name(100),  camp::EnumValueNotFound);
}

BOOST_AUTO_TEST_SUITE_END()
