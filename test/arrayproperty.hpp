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

#ifndef PONDERTEST_ARRAYPROPERTY_HPP
#define PONDERTEST_ARRAYPROPERTY_HPP

#include <ponder/pondertype.hpp>
#include <ponder/class.hpp>
#include <ponder/classbuilder.hpp>
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
        std::array<int, 3> ints;
        std::vector<std::string> strings;
        std::list<MyType> objects;
    };

    void declare()
    {
        ponder::Class::declare<MyType>("ArrayPropertyTest::MyType");
        
        ponder::Class::declare<MyClass>("ArrayPropertyTest::MyClass")
            .property("bools", &MyClass::bools)
            .property("ints", &MyClass::ints)
            .property("strings", &MyClass::strings)
            .property("objects", &MyClass::objects);
    }
}

PONDER_AUTO_TYPE(ArrayPropertyTest::MyType, &ArrayPropertyTest::declare)
PONDER_AUTO_TYPE(ArrayPropertyTest::MyClass, &ArrayPropertyTest::declare)

#endif // PONDERTEST_ARRAYPROPERTY_HPP
