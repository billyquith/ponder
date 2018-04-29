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

#include <ponder/classget.hpp>
#include <ponder/class.hpp>
#include <ponder/classbuilder.hpp>
#include "test.hpp"
#include <string>

// TODO: We may readd tags at a later date. We'll leave this here as a reference for now.

//namespace TagHolderTest
//{
//    struct MyClass
//    {
//        MyClass(int x) : prop(x) {}
//        ponder::String func() {return "func";}
//        int prop;
//    };
//
//    struct MyType
//    {
//        MyType(int x) : prop(x) {}
//        int prop;
//    };
//
//    enum MyEnum
//    {
//        One = 1,
//        Ten = 10
//    };
//
//    MyType object1(1);
//    MyType object2(2);
//
//    void declare()
//    {
//        ponder::Enum::declare<MyEnum>("TagHolderTest::MyEnum");
//
//        ponder::Class::declare<MyType>("TagHolderTest::MyType");
//
//        ponder::Class::declare<MyClass>("TagHolderTest::MyClass")
//            .tag("a")
//            .tag("b", 0)
//            .tag(true)
//            .tag(10)
//            .tag(5.25)
//            .tag("hello")
//            .tag(Ten)
//            .tag(object1)
//            .tag("static1", true)
//            .tag("static2", 10)
//            .tag("static3", 5.25)
//            .tag("static4", "hello")
//            .tag("static5", Ten)
//            .tag("static6", object1)
//            .tag("dynamic1", &MyClass::prop)
//            .tag("dynamic2", &MyClass::func)
//            .property("prop", &MyClass::prop)
//            .tag("a")
//            .tag("b", 0)
//            .function("func", &MyClass::func)
//            .tag("a")
//            .tag("b", 0);
//    }
//}
//
//PONDER_AUTO_TYPE(TagHolderTest::MyClass, &TagHolderTest::declare)
//PONDER_AUTO_TYPE(TagHolderTest::MyType,  &TagHolderTest::declare)
//PONDER_AUTO_TYPE(TagHolderTest::MyEnum,  &TagHolderTest::declare)
//
//using namespace TagHolderTest;
//
////-----------------------------------------------------------------------------
////                         Tests for ponder::TagHolder
////-----------------------------------------------------------------------------
//
//TEST_CASE("Objects can be assigned tags")
//{
//    const ponder::Class* metaclass = &ponder::classByType<MyClass>();
//
//    REQUIRE(metaclass->tagCount() == 16);
//    REQUIRE(metaclass->property("prop").tagCount() == 2);
//    REQUIRE(metaclass->function("func").tagCount() == 2);
//
//    SECTION("can check tags attached")
//    {
//        REQUIRE(metaclass->hasTag("a") == true);
//        REQUIRE(metaclass->hasTag("b") == true);
//        REQUIRE(metaclass->hasTag("x") == false);
//    }
//
//    SECTION("can have values")
//    {
//        REQUIRE(metaclass->hasTag(true) ==      true);
//        REQUIRE(metaclass->hasTag(10) ==        true);
//        REQUIRE(metaclass->hasTag(5.25) ==      true);
//        REQUIRE(metaclass->hasTag("hello") ==   true);
//        REQUIRE(metaclass->hasTag(Ten) ==       true);
//        REQUIRE(metaclass->hasTag(object1) ==   true);
//
//        REQUIRE(metaclass->hasTag(false) ==     false);
//        REQUIRE(metaclass->hasTag(20) ==        false);
//        REQUIRE(metaclass->hasTag(8.78) ==      false);
//        REQUIRE(metaclass->hasTag("hi") ==      false);
//        REQUIRE(metaclass->hasTag(One) ==       false);
//        REQUIRE(metaclass->hasTag(object2) ==   false);
//    }
//
//    SECTION("can have named values")
//    {
//        REQUIRE(metaclass->tag("static1") == ponder::Value(true));
//        REQUIRE(metaclass->tag("static2") == ponder::Value(10));
//        REQUIRE(metaclass->tag("static3") == ponder::Value(5.25));
//        REQUIRE(metaclass->tag("static4") == ponder::Value("hello"));
//        IS_TRUE( metaclass->tag("static5") == ponder::Value(Ten) );
//        IS_TRUE( metaclass->tag("static6") == ponder::Value(object1) );
//        IS_TRUE( metaclass->tag("xxxxxxx") == ponder::Value::nothing );
//    }
//
//    SECTION("can have dynamic values")
//    {
//        REQUIRE(metaclass->tag("dynamic1", MyClass(10)) == ponder::Value(10));
//        REQUIRE(metaclass->tag("dynamic2", MyClass(10)) == ponder::Value("func"));
//    }
//}


