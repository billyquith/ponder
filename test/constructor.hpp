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

#ifndef CAMPTEST_CONSTRUCTOR_HPP
#define CAMPTEST_CONSTRUCTOR_HPP

#include <camp/camptype.hpp>
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
}

CAMP_TYPE(ConstructorTest::MyEnum);
CAMP_TYPE(ConstructorTest::MyType);
CAMP_TYPE(ConstructorTest::MyBase1);
CAMP_TYPE(ConstructorTest::MyBase2);
CAMP_TYPE(ConstructorTest::MyClass);

#endif // CAMPTEST_CONSTRUCTOR_HPP
