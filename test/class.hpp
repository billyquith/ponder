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

#ifndef CAMPTEST_CLASS_HPP
#define CAMPTEST_CLASS_HPP

#include <camp/camptype.hpp>
#include <camp/class.hpp>

namespace ClassTest
{
    struct MyTempClass
    {
    };

    struct MyUndeclaredClass
    {
    };

    struct MyClass
    {
        void func() {}
        int prop;
    };

    struct MyClass2
    {
    };

    struct Base
    {
        CAMP_RTTI();
    };

    struct Derived : Base
    {
        CAMP_RTTI();
    };

    struct DerivedNoRtti : Base
    {
    };

    struct Derived2NoRtti : Derived
    {
    };

    void declare()
    {
        camp::Class::declare<MyClass>("ClassTest::MyClass")
            .property("prop", &MyClass::prop)
            .function("func", &MyClass::func);

        camp::Class::declare<MyClass2>("ClassTest::MyClass2");

        camp::Class::declare<Base>("ClassTest::Base");

        camp::Class::declare<Derived>("ClassTest::Derived")
            .base<Base>();

        camp::Class::declare<DerivedNoRtti>("ClassTest::DerivedNoRtti")
            .base<Base>();

        camp::Class::declare<Derived2NoRtti>("ClassTest::Derived2NoRtti")
            .base<Derived>();
    }
}

CAMP_TYPE(ClassTest::MyUndeclaredClass /* never declared */);
CAMP_TYPE(ClassTest::MyTempClass /* declared in a test */);
CAMP_AUTO_TYPE(ClassTest::MyClass, &ClassTest::declare);
CAMP_AUTO_TYPE(ClassTest::MyClass2, &ClassTest::declare);
CAMP_AUTO_TYPE(ClassTest::Base, &ClassTest::declare);
CAMP_AUTO_TYPE(ClassTest::Derived, &ClassTest::declare);
CAMP_AUTO_TYPE(ClassTest::DerivedNoRtti, &ClassTest::declare);
CAMP_AUTO_TYPE(ClassTest::Derived2NoRtti, &ClassTest::declare);

#endif // CAMPTEST_CLASS_HPP
