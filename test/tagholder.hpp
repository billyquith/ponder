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

#ifndef CAMPTEST_TAGHOLDER_HPP
#define CAMPTEST_TAGHOLDER_HPP

#include <camp/camptype.hpp>
#include <camp/class.hpp>
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
