/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
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

#ifndef PONDERTEST_ENUM_HPP
#define PONDERTEST_ENUM_HPP

#include <ponder/pondertype.hpp>
#include <ponder/enum.hpp>

namespace EnumTest
{
    enum MyTempEnum
    {
    };

    enum MyUndeclaredEnum
    {
    };

    enum MyEnum
    {
        Zero = 0,
        One  = 1,
        Two  = 2
    };

    enum MyEnum2
    {
    };

    enum class MyEnumClass
    {
        Red,
        Green,
        Blue
    };

    void declare()
    {
        ponder::Enum::declare<MyEnum>("EnumTest::MyEnum")
            .value("Zero", Zero)
            .value("One", One)
            .value("Two", Two);

        ponder::Enum::declare<MyEnum2>("EnumTest::MyEnum2");
        
        ponder::Enum::declare<MyEnumClass>("EnumTest::MyEnumClass")
            .value("Red", MyEnumClass::Red)
            .value("Green", MyEnumClass::Green)
            .value("Blue", MyEnumClass::Blue);
    }
}

PONDER_TYPE(EnumTest::MyUndeclaredEnum /* never declared */)
PONDER_TYPE(EnumTest::MyTempEnum /* declared during tests */)
PONDER_AUTO_TYPE(EnumTest::MyEnum, &EnumTest::declare)
PONDER_AUTO_TYPE(EnumTest::MyEnum2, &EnumTest::declare)
PONDER_AUTO_TYPE(EnumTest::MyEnumClass, &EnumTest::declare)

#endif // PONDERTEST_ENUM_HPP
