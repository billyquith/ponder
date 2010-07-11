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

#ifndef CAMPTEST_USERPROPERTY_HPP
#define CAMPTEST_USERPROPERTY_HPP

#include <camp/camptype.hpp>
#include <camp/class.hpp>

namespace UserPropertyTest
{
    struct MyType
    {
        MyType(int x_) : x(x_) {}
        int x;
    };

    struct MyClass
    {
        MyClass(int x) : prop(x) {}
        MyType prop;
    };

    void declare()
    {
        camp::Class::declare<MyType>("UserPropertyTest::MyType");

        camp::Class::declare<MyClass>("UserPropertyTest::MyClass")
            .property("prop", &MyClass::prop);
    }
}

CAMP_AUTO_TYPE(UserPropertyTest::MyType, &UserPropertyTest::declare);
CAMP_AUTO_TYPE(UserPropertyTest::MyClass, &UserPropertyTest::declare);

#endif // CAMPTEST_USERPROPERTY_HPP
