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

#ifndef CAMPTEST_ENUMPROPERTY_HPP
#define CAMPTEST_ENUMPROPERTY_HPP

#include <camp/camptype.hpp>
#include <camp/enum.hpp>
#include <camp/class.hpp>
#include <boost/array.hpp>
#include <list>
#include <vector>

namespace EnumPropertyTest
{
    enum MyEnum
    {
        Zero = 0,
        One  = 1
    };

    struct MyClass
    {
        MyClass(MyEnum x_) : x(x_)
        {
        }

        MyEnum x;
    };

    void declare()
    {
        camp::Enum::declare<MyEnum>("EnumPropertyTest::MyEnum");

        camp::Class::declare<MyClass>("EnumPropertyTest::MyClass")
            .property("x", &MyClass::x);
    }
}

CAMP_AUTO_TYPE(EnumPropertyTest::MyEnum, &EnumPropertyTest::declare);
CAMP_AUTO_TYPE(EnumPropertyTest::MyClass, &EnumPropertyTest::declare);

#endif // CAMPTEST_ENUMPROPERTY_HPP
