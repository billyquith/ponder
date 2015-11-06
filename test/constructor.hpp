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

#ifndef CAMPTEST_CONSTRUCTOR_HPP
#define CAMPTEST_CONSTRUCTOR_HPP

#include <camp/camptype.hpp>
#include <camp/class.hpp>
#include <camp/enum.hpp>
#include <camp/classbuilder.hpp>
#include <string>

namespace ConstructorTest
{
    enum MyEnum
    {
        zero  = 0,
        one   = 1,
        two   = 2,
        three = 3,
        four  = 4,
        five  = 5
    };

    struct MyType
    {
        MyType(int x_) : x(x_) {}
        int x;
    };

    struct MyBase1
    {
        MyBase1() : base1("base1") {}
        virtual ~MyBase1() {}
        std::string base1;
    };

    struct MyBase2
    {
        MyBase2() : base2("base2") {}
        virtual ~MyBase2() {}
        std::string base2;
    };

    struct MyClass : MyBase1, MyBase2
    {
        MyClass() : l(0), r(0.), s("0"), e(zero), u(0) {}
        MyClass(long l_) : l(l_), r(1.), s("1"), e(one), u(1) {}
        MyClass(long l_, double r_) : l(l_), r(r_), s("2"), e(two), u(2) {}
        MyClass(long l_, double r_, std::string s_) : l(l_), r(r_), s(s_), e(three), u(3) {}
        MyClass(long l_, double r_, std::string s_, MyEnum e_) : l(l_), r(r_), s(s_), e(e_), u(4) {}
        MyClass(long l_, double r_, std::string s_, MyEnum e_, MyType t_) : l(l_), r(r_), s(s_), e(e_), u(t_) {}

        long l;
        double r;
        std::string s;
        MyEnum e;
        MyType u;
    };

    void declare()
    {
        camp::Enum::declare<MyEnum>("ConstructorTest::MyEnum")
            .value("zero",  zero)
            .value("one",   one)
            .value("two",   two)
            .value("three", three)
            .value("four",  four)
            .value("five",  five);

        camp::Class::declare<MyType>("ConstructorTest::MyType");

        camp::Class::declare<MyBase1>("ConstructorTest::MyBase1");
        camp::Class::declare<MyBase2>("ConstructorTest::MyBase2");

        camp::Class::declare<MyClass>("ConstructorTest::MyClass")
            .base<MyBase1>()
            .base<MyBase2>()
            .constructor()
            .constructor<long>()
            .constructor<long, double>()
            .constructor<long, double, std::string>()
            .constructor<long, double, std::string, MyEnum>()

            // trying types that don't exactly match those declared
            .constructor<unsigned short, float, std::string, MyEnum, int>();
    }
}

CAMP_AUTO_TYPE(ConstructorTest::MyEnum, &ConstructorTest::declare)
CAMP_AUTO_TYPE(ConstructorTest::MyType, &ConstructorTest::declare)
CAMP_AUTO_TYPE(ConstructorTest::MyBase1, &ConstructorTest::declare)
CAMP_AUTO_TYPE(ConstructorTest::MyBase2, &ConstructorTest::declare)
CAMP_AUTO_TYPE(ConstructorTest::MyClass, &ConstructorTest::declare)

#endif // CAMPTEST_CONSTRUCTOR_HPP
