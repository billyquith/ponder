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

#ifndef CAMPTEST_VALUE_HPP
#define CAMPTEST_VALUE_HPP

#include <camp/camptype.hpp>
#include <camp/class.hpp>
#include <camp/enum.hpp>
#include <camp/type.hpp>
#include <camp/valuevisitor.hpp>
#include <camp/enumobject.hpp>
#include <camp/userobject.hpp>

namespace ValueTest
{
    struct MyClass
    {
        MyClass(int x_) : x(x_) {}
        int x;
    };

    bool operator==(const MyClass& left, const MyClass& right)
    {
        return left.x == right.x;
    }

    bool operator<(const MyClass& left, const MyClass& right)
    {
        return left.x < right.x;
    }

    std::ostream& operator<<(std::ostream& stream, const MyClass& object)
    {
        return stream << object.x;
    }

    enum MyEnum
    {
        One = 1,
        Two = 2
    };

    struct Visitor : public camp::ValueVisitor<camp::Type>
    {
         camp::Type operator()(camp::NoType)
         {
             return camp::noType;
         }

         camp::Type operator()(bool)
         {
             return camp::boolType;
         }
         
         camp::Type operator()(long)
         {
             return camp::intType;
         }
         
         camp::Type operator()(double)
         {
             return camp::realType;
         }
         
         camp::Type operator()(const std::string&)
         {
             return camp::stringType;
         }
         
         camp::Type operator()(const camp::EnumObject&)
         {
             return camp::enumType;
         }
         
         camp::Type operator()(const camp::UserObject&)
         {
             return camp::userType;
         }
    };

    void declare()
    {
        camp::Enum::declare<MyEnum>("ValueTest::MyEnum")
            .value("One", One)
            .value("Two", Two);
        camp::Class::declare<MyClass>("ValueTest::MyClass");
    }
}

CAMP_AUTO_TYPE(ValueTest::MyClass, &ValueTest::declare)
CAMP_AUTO_TYPE(ValueTest::MyEnum, &ValueTest::declare)

#endif // CAMPTEST_VALUE_HPP
