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

#ifndef CAMPTEST_TAGHOLDER_HPP
#define CAMPTEST_TAGHOLDER_HPP

#include <camp/camptype.hpp>


namespace TagHolderTest
{
    struct MyClass
    {
        MyClass(int x) : prop(x) {}
        std::string func() {return "func";}
        int prop;
    };

    struct MyType
    {
        MyType(int x) : prop(x) {}
        int prop;
    };

    enum MyEnum
    {
        One = 1,
        Ten = 10
    };
}

CAMP_TYPE(TagHolderTest::MyClass);
CAMP_TYPE(TagHolderTest::MyType);
CAMP_TYPE(TagHolderTest::MyEnum);

#endif // CAMPTEST_TAGHOLDER_HPP
