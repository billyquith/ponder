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

#include <ponder/enumget.hpp>
#include <ponder/errors.hpp>
#include <ponder/enum.hpp>
#include "test.hpp"

namespace EnumClassTest
{
    enum class MyExplicitylyDeclaredEnum
    {
    };
    
    enum class MyUndeclaredEnum
    {
    };
    
    enum class MyEnum
    {
        Zero = 0,
        One  = 1,
        Two  = 2
    };
    
    enum class MyEnum2
    {
    };
    
    enum class MyEnumClass
    {
        Red,
        Green,
        Blue
    };
    
    void declare()
    {
        ponder::Enum::declare<MyEnum>("EnumClassTest::MyEnum")
            .value("Zero", MyEnum::Zero)
            .value("One", MyEnum::One)
            .value("Two", MyEnum::Two);
        
        ponder::Enum::declare<MyEnum2>("EnumClassTest::MyEnum2");
    }
}

PONDER_TYPE(EnumClassTest::MyUndeclaredEnum /* never declared */)
PONDER_TYPE(EnumClassTest::MyExplicitylyDeclaredEnum /* declared during tests */)
PONDER_AUTO_TYPE(EnumClassTest::MyEnum, &EnumClassTest::declare)
PONDER_AUTO_TYPE(EnumClassTest::MyEnum2, &EnumClassTest::declare)

using namespace EnumClassTest;


//-----------------------------------------------------------------------------
//                   Tests for ponder::Enum for enum class
//-----------------------------------------------------------------------------

TEST_CASE("Enums classes need to be declared")
{
    SECTION("explicit declaration")
    {
        const std::size_t count = ponder::enumCount();
        
        ponder::Enum::declare<MyExplicitylyDeclaredEnum>(
            "EnumClassTest::MyExplicitylyDeclaredEnum");

        REQUIRE(ponder::enumCount() == count + 1);        
    }
    
    SECTION("duplicates are errors")
    {        
        ponder::enumByType<MyEnum>(); // to make sure it is declared

        // duplicate by type
        REQUIRE_THROWS_AS(ponder::Enum::declare<MyEnum>(), ponder::EnumAlreadyCreated);
        
        // duplicate by name
        REQUIRE_THROWS_AS(ponder::Enum::declare<MyUndeclaredEnum>("EnumClassTest::MyEnum"),
                          ponder::EnumAlreadyCreated);
    }
    
    SECTION("can be compared")
    {
        IS_TRUE(ponder::enumByType<MyEnum>()  == ponder::enumByType<MyEnum>());
        IS_TRUE(ponder::enumByType<MyEnum>()  != ponder::enumByType<MyEnum2>());
        IS_TRUE(ponder::enumByType<MyEnum2>() != ponder::enumByType<MyEnum>());
    }
}


TEST_CASE("Enum class metadata can be retrieved")
{
    MyEnum object = MyEnum();
    MyUndeclaredEnum object2 = MyUndeclaredEnum();

    SECTION("by name")
    {
        REQUIRE(ponder::enumByName("EnumClassTest::MyEnum").name() == "EnumClassTest::MyEnum");
        
        REQUIRE_THROWS_AS(ponder::enumByName("EnumClassTest::MyUndeclaredEnum"),
                          ponder::EnumNotFound);
    }
    
    SECTION("by type")
    {
        REQUIRE(ponder::enumByType<MyEnum>().name() == "EnumClassTest::MyEnum");

        REQUIRE(ponder::enumByTypeSafe<MyUndeclaredEnum>() == static_cast<ponder::Enum*>(0));      
        
        REQUIRE_THROWS_AS(ponder::enumByType<MyUndeclaredEnum>(), ponder::EnumNotFound);        
    }
    
    SECTION("by instance")
    {
        REQUIRE(ponder::enumByObject(object).name() == "EnumClassTest::MyEnum");
        REQUIRE(ponder::enumByObject(&object).name() == "EnumClassTest::MyEnum");
        
        REQUIRE_THROWS_AS(ponder::enumByObject(object2), ponder::EnumNotFound);
        REQUIRE_THROWS_AS(ponder::enumByObject(&object2), ponder::EnumNotFound);
   }
}


TEST_CASE("Enum class values can be read")
{
    const ponder::Enum* metaenum = &ponder::enumByType<MyEnum>();

    REQUIRE(metaenum->size() == 3U);

    SECTION("as pairs")
    {
        REQUIRE(metaenum->pair(0).name == "One");
        REQUIRE(metaenum->pair(0).valueAs<MyEnum>() == MyEnum::One);
        REQUIRE(metaenum->pair(1).name == "Two");
        REQUIRE(metaenum->pair(1).valueAs<MyEnum>() == MyEnum::Two);
        REQUIRE(metaenum->pair(2).name == "Zero");
        REQUIRE(metaenum->pair(2).valueAs<MyEnum>() == MyEnum::Zero);
        
        REQUIRE_THROWS_AS(metaenum->pair(3), ponder::OutOfRange);
    }
    
    SECTION("names")
    {
        REQUIRE(metaenum->hasName("Zero") == true);
        REQUIRE(metaenum->hasName("One") == true);
        REQUIRE(metaenum->hasName("Two") == true);
        REQUIRE(metaenum->hasName("Hundred") == false);

        REQUIRE(metaenum->name(MyEnum::Zero) == "Zero");
        REQUIRE(metaenum->name(MyEnum::One) == "One");
        REQUIRE(metaenum->name(MyEnum::Two) == "Two");
        
        REQUIRE_THROWS_AS(metaenum->name(100), ponder::EnumValueNotFound);
    }    
    
    SECTION("values")
    {
        REQUIRE(metaenum->hasValue(MyEnum::Zero) == true);
        REQUIRE(metaenum->hasValue(MyEnum::One) == true);
        REQUIRE(metaenum->hasValue(MyEnum::Two) == true);
        REQUIRE(metaenum->hasValue(100) == false);

        REQUIRE(metaenum->value<MyEnum>("Zero") == MyEnum::Zero);
        REQUIRE(metaenum->value<MyEnum>("One") == MyEnum::One);
        REQUIRE(metaenum->value<MyEnum>("Two") == MyEnum::Two);
        
        REQUIRE_THROWS_AS(metaenum->value("xxx"), ponder::EnumNameNotFound);
    }
}
