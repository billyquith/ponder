/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2016 Billy Quith.
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

#include <ponder/classbuilder.hpp>
#include "catch.hpp"

class MyClass {
    int a, b;
    float f;
    std::string s;
public:
    MyClass() : a(-1), b(-1), f(0.f) {}
    MyClass(int a_) : a(a_), b(-1), f(0.f) {}

    static void declare();
};

void MyClass::declare()
{
    ponder::Class::declare<MyClass>()
        .constructor()
        .constructor<int>()
        .property("a", &MyClass::a)
        .property("b", &MyClass::b)
        ;
}

PONDER_AUTO_TYPE(MyClass, &MyClass::declare)

//-----------------------------------------------------------------------------
//                         Tests for ponder object creation
//-----------------------------------------------------------------------------

TEST_CASE("Ponder can create user objects")
{
    SECTION("can retrieve memory size")
    {
        REQUIRE(ponder::classByType<MyClass>().sizeOf() == sizeof(MyClass));
        REQUIRE(ponder::classByName("MyClass").sizeOf() == sizeof(MyClass));
    }

    SECTION("constructor with no args")
    {
        ponder::UserObject obj(ponder::classByType<MyClass>().construct());
        REQUIRE((obj != ponder::UserObject::nothing));
        REQUIRE(obj.get("a") == ponder::Value(-1));
    }
}

TEST_CASE("Ponder can be used as a factory")
{
    SECTION("constructor with no args")
    {
//        MyClass* c = ponder::classByType<MyClass>().create();
//        REQUIRE(c != nullptr);
//        REQUIRE(get("a") == ponder::Value(-1));
    }
}

