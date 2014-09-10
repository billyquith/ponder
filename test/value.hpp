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
