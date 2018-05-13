/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2018 Nick Trout.
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

#include <ponder/errors.hpp>
#include <ponder/enumget.hpp>
#include <ponder/enumobject.hpp>
#include <ponder/enum.hpp>
#include "test.hpp"

namespace EnumClassObjectTest
{
    enum class MyUndeclaredEnum
    {
        Undeclared
    };
    
    enum class MyEnum
    {
        Zero = 0,
        One  = 1,
        Two  = 2
    };
    
    enum class MyEnum2
    {
        Zero2 = 0,
        One2  = 1,
        Two2  = 2
    };
    
    void declare()
    {
        ponder::Enum::declare<MyEnum>("EnumClassObjectTest::MyEnum")
            .value("Zero", MyEnum::Zero)
            .value("One",  MyEnum::One)
            .value("Two",  MyEnum::Two);
        
        // same names as MyEnum
        ponder::Enum::declare<MyEnum2>("EnumClassObjectTest::MyEnum2")
            .value("Zero", MyEnum2::Zero2)
            .value("One",  MyEnum2::One2)
            .value("Two",  MyEnum2::Two2);
    }
}

PONDER_TYPE(EnumClassObjectTest::MyUndeclaredEnum)
PONDER_AUTO_TYPE(EnumClassObjectTest::MyEnum, &EnumClassObjectTest::declare)
PONDER_AUTO_TYPE(EnumClassObjectTest::MyEnum2, &EnumClassObjectTest::declare)

using namespace EnumClassObjectTest;

//-----------------------------------------------------------------------------
//                Tests for ponder::EnumObject for enum class
//-----------------------------------------------------------------------------

TEST_CASE("Enum class objects")
{
    ponder::EnumObject zero(MyEnum::Zero);
    ponder::EnumObject one(MyEnum::One);
    ponder::EnumObject two(MyEnum::Two);

    SECTION("has names")
    {
        REQUIRE(zero.name() == "Zero");
        REQUIRE(one.name() == "One");
        REQUIRE(two.name() == "Two");
    }

    SECTION("has values")
    {
        REQUIRE(zero.value<MyEnum>() == MyEnum::Zero);
        REQUIRE(one.value<MyEnum>() == MyEnum::One);
        REQUIRE(two.value<MyEnum>() == MyEnum::Two);
    }
    
    SECTION("wraps an Enum class type")
    {
        IS_TRUE(zero.getEnum() == ponder::enumByType<MyEnum>());
        IS_TRUE(one.getEnum()  == ponder::enumByType<MyEnum>());
        IS_TRUE(two.getEnum()  == ponder::enumByType<MyEnum>());
    }
    
    SECTION("can be compared ==")
    {
        ponder::EnumObject zero2(MyEnum2::Zero2);
        ponder::EnumObject one2(MyEnum2::One2);
        ponder::EnumObject two2(MyEnum2::Two2);

        REQUIRE(zero == ponder::EnumObject(MyEnum::Zero));
        REQUIRE(one  == ponder::EnumObject(MyEnum::One));
        REQUIRE(two  == ponder::EnumObject(MyEnum::Two));

        REQUIRE((zero == one) == false);
        REQUIRE((one  == two) == false);
        REQUIRE((two  == zero) == false);

        REQUIRE((zero == zero2) == false); // same value and name, different metaenum
        REQUIRE((one  == one2) == false);
        REQUIRE((two  == two2) == false);
    }
    
    SECTION("can be compared <")
    {
        REQUIRE((zero < zero) == false);
        REQUIRE(zero < one);
        REQUIRE(zero < two);

        REQUIRE((one < zero) == false);
        REQUIRE((one < one) == false);
        REQUIRE(one < two);

        REQUIRE((two < zero) == false);
        REQUIRE((two < one) == false);
        REQUIRE((two < two) == false);
    }
    
    SECTION("must be declared")
    {
        // The meta-enum of MyUndeclaredEnum is *not* declared
        // Note: use lambda here as Catch2 test needs to be an expression.
        REQUIRE_THROWS_AS([](){
            ponder::EnumObject obj(MyUndeclaredEnum::Undeclared);
        }(), ponder::EnumNotFound);
    }    
}
    









