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

#ifndef CAMPTEST_INHERITANCE_HPP
#define CAMPTEST_INHERITANCE_HPP

#include <camp/camptype.hpp>
#include <camp/class.hpp>

namespace InheritanceTest
{
    struct MyClass1
    {
        MyClass1() : p1(10), po1(10) {}
        virtual ~MyClass1() {}
        int p1;
        int f1() const {return 1;}
        int po1;
        int fo1() {return 1;}
        CAMP_RTTI();
    };

    struct MyClass2
    {
        MyClass2() : p2(20), po2(20) {}
        virtual ~MyClass2() {}
        int p2;
        int f2() const {return 2;}
        virtual int fv() const {return p2;}
        int po2;
        int fo2() {return 2;}
        CAMP_RTTI();
    };

    struct MyClass3 : public MyClass1, public MyClass2
    {
        MyClass3() : p3(30), po3(30) {}
        virtual ~MyClass3() {}
        int p3;
        int f3() const {return 3;}
        virtual int fv() const {return p3;}
        int po3;
        int fo3() {return 3;}
        CAMP_RTTI();
    };

    struct MyClass4 : public MyClass3
    {
        MyClass4() : p4(40), po4(40) {}
        virtual ~MyClass4() {}
        int p4;
        int f4() const {return 4;}
        virtual int fv() const {return p4;}
        int po4;
        int fo4() {return 4;}
        CAMP_RTTI();
    };

    void declare()
    {
        camp::Class::declare<MyClass1>("InheritanceTest::MyClass1")
            .function("f1", &MyClass1::f1)
            .property("p1", &MyClass1::p1)
            .function("overridden", &MyClass1::fo1)
            .property("overridden", &MyClass1::po1);

        camp::Class::declare<MyClass2>("InheritanceTest::MyClass2")
            .function("f2", &MyClass2::f2)
            .property("p2", &MyClass2::p2)
            .function("virtual", &MyClass2::fv)
            .function("overridden", &MyClass2::fo2)
            .property("overridden", &MyClass2::po2);

        camp::Class::declare<MyClass3>("InheritanceTest::MyClass3")
            .base<MyClass1>()
            .base<MyClass2>()
            .function("f3", &MyClass3::f3)
            .property("p3", &MyClass3::p3)
            .function("overridden", &MyClass3::fo3)
            .property("overridden", &MyClass3::po3);

        camp::Class::declare<MyClass4>("InheritanceTest::MyClass4")
            .base<MyClass3>()
            .function("f4", &MyClass4::f4)
            .property("p4", &MyClass4::p4)
            .function("overridden", &MyClass4::fo4)
            .property("overridden", &MyClass4::po4);
    }
}

CAMP_AUTO_TYPE(InheritanceTest::MyClass1, &InheritanceTest::declare);
CAMP_AUTO_TYPE(InheritanceTest::MyClass2, &InheritanceTest::declare);
CAMP_AUTO_TYPE(InheritanceTest::MyClass3, &InheritanceTest::declare);
CAMP_AUTO_TYPE(InheritanceTest::MyClass4, &InheritanceTest::declare);

#endif // CAMPTEST_INHERITANCE_HPP
