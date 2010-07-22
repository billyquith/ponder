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
