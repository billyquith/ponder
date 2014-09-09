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

#ifndef CAMPTEST_ENUM_HPP
#define CAMPTEST_ENUM_HPP

#include <camp/camptype.hpp>
#include <camp/enum.hpp>

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

    void declare()
    {
        camp::Enum::declare<MyEnum>("EnumTest::MyEnum")
            .value("Zero", Zero)
            .value("One", One)
            .value("Two", Two);

        camp::Enum::declare<MyEnum2>("EnumTest::MyEnum2");
    }
}

CAMP_TYPE(EnumTest::MyUndeclaredEnum /* never declared */)
CAMP_TYPE(EnumTest::MyTempEnum /* declared during tests */)
CAMP_AUTO_TYPE(EnumTest::MyEnum, &EnumTest::declare)
CAMP_AUTO_TYPE(EnumTest::MyEnum2, &EnumTest::declare)

#endif // CAMPTEST_ENUM_HPP
