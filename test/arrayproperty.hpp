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

#ifndef CAMPTEST_ARRAYPROPERTY_HPP
#define CAMPTEST_ARRAYPROPERTY_HPP

#include <camp/camptype.hpp>
#include <camp/class.hpp>
#include <boost/array.hpp>
#include <list>
#include <vector>

namespace ArrayPropertyTest
{
    struct MyType
    {
        MyType(int x_) : x(x_)
        {
        }

        bool operator ==(const MyType& other) const
        {
            return x == other.x;
        }

        int x;
    };

    struct MyClass
    {
        MyClass()
        {
            bools[0] = true;
            bools[1] = false;

            ints[0] = -10;
            ints[1] = 10;
            ints[2] = 100;

            strings.push_back("string 0");
            strings.push_back("string 1");
            strings.push_back("string 2");
            strings.push_back("string 3");

            objects.push_back(MyType(0));
            objects.push_back(MyType(1));
            objects.push_back(MyType(2));
            objects.push_back(MyType(3));
            objects.push_back(MyType(4));
        }

        bool bools[2];
        boost::array<int, 3> ints;
        std::vector<std::string> strings;
        std::list<MyType> objects;
    };

    void declare()
    {
        camp::Class::declare<MyType>("ArrayPropertyTest::MyType");

        camp::Class::declare<MyClass>("ArrayPropertyTest::MyClass")
            .property("bools", &MyClass::bools)
            .property("ints", &MyClass::ints)
            .property("strings", &MyClass::strings)
            .property("objects", &MyClass::objects);
    }
}

CAMP_AUTO_TYPE(ArrayPropertyTest::MyType, &ArrayPropertyTest::declare)
CAMP_AUTO_TYPE(ArrayPropertyTest::MyClass, &ArrayPropertyTest::declare)

#endif // CAMPTEST_ARRAYPROPERTY_HPP
