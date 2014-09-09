/****************************************************************************
**
** This file is part of the CAMP library.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
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
