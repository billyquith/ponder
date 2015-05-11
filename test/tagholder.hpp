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

#ifndef CAMPTEST_TAGHOLDER_HPP
#define CAMPTEST_TAGHOLDER_HPP

#include <camp/camptype.hpp>
#include <camp/class.hpp>
#include <camp/classbuilder.hpp>
#include <string>

namespace TagHolderTest
{
    struct MyClass
    {
        MyClass(int x) : prop(x) {}
        std::string func() {return "func";}
        int prop;
    };

    struct MyType
    {
        MyType(int x) : prop(x) {}
        int prop;
    };

    enum MyEnum
    {
        One = 1,
        Ten = 10
    };

    MyType object1(1);
    MyType object2(2);

    void declare()
    {
        camp::Enum::declare<MyEnum>("TagHolderTest::MyEnum");

        camp::Class::declare<MyType>("TagHolderTest::MyType");

        camp::Class::declare<MyClass>("TagHolderTest::MyClass")
            .tag("a")
            .tag("b", 0)
            .tag(true)
            .tag(10)
            .tag(5.25)
            .tag("hello")
            .tag(Ten)
            .tag(object1)
            .tag("static1", true)
            .tag("static2", 10)
            .tag("static3", 5.25)
            .tag("static4", "hello")
            .tag("static5", Ten)
            .tag("static6", object1)
            .tag("dynamic1", &MyClass::prop)
            .tag("dynamic2", &MyClass::func)
            .property("prop", &MyClass::prop)
                .tag("a")
                .tag("b", 0)
            .function("func", &MyClass::func)
                .tag("a")
                .tag("b", 0);
    }
}

CAMP_AUTO_TYPE(TagHolderTest::MyClass, &TagHolderTest::declare)
CAMP_AUTO_TYPE(TagHolderTest::MyType,  &TagHolderTest::declare)
CAMP_AUTO_TYPE(TagHolderTest::MyEnum,  &TagHolderTest::declare)

#endif // CAMPTEST_TAGHOLDER_HPP
