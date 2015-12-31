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

#ifndef PONDERTEST_VALUE_HPP
#define PONDERTEST_VALUE_HPP

#include <ponder/pondertype.hpp>
#include <ponder/class.hpp>
#include <ponder/enum.hpp>
#include <ponder/type.hpp>
#include <ponder/valuevisitor.hpp>
#include <ponder/enumobject.hpp>
#include <ponder/userobject.hpp>
#include <ponder/classbuilder.hpp>

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

    struct Visitor : public ponder::ValueVisitor<ponder::Type>
    {
         ponder::Type operator()(ponder::NoType)
         {
             return ponder::noType;
         }

         ponder::Type operator()(bool)
         {
             return ponder::boolType;
         }
         
         ponder::Type operator()(long)
         {
             return ponder::intType;
         }
         
         ponder::Type operator()(double)
         {
             return ponder::realType;
         }
         
         ponder::Type operator()(const std::string&)
         {
             return ponder::stringType;
         }
         
         ponder::Type operator()(const ponder::EnumObject&)
         {
             return ponder::enumType;
         }
         
         ponder::Type operator()(const ponder::UserObject&)
         {
             return ponder::userType;
         }
    };

    void declare()
    {
        ponder::Enum::declare<MyEnum>("ValueTest::MyEnum")
            .value("One", One)
            .value("Two", Two);
        ponder::Class::declare<MyClass>("ValueTest::MyClass");
    }
}

PONDER_AUTO_TYPE(ValueTest::MyClass, &ValueTest::declare)
PONDER_AUTO_TYPE(ValueTest::MyEnum, &ValueTest::declare)

#endif // PONDERTEST_VALUE_HPP
