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

#ifndef CAMPTEST_CLASSVISITOR_HPP
#define CAMPTEST_CLASSVISITOR_HPP

#include <camp/camptype.hpp>
#include <camp/class.hpp>
#include <camp/enum.hpp>
#include <camp/classvisitor.hpp>
#include <camp/simpleproperty.hpp>
#include <camp/arrayproperty.hpp>
#include <camp/enumproperty.hpp>
#include <camp/userproperty.hpp>
#include <camp/function.hpp>

namespace ClassVisitorTest
{
    class MyClassVisitor : public camp::ClassVisitor
    {
    public:

        MyClassVisitor()
            : simpleVisited(false)
            , arrayVisited(false)
            , enumVisited(false)
            , userVisited(false)
            , functionVisited(false)
        {
        }

        virtual void visit(const camp::SimpleProperty& property)
        {
            simpleVisited = true;
        }

        virtual void visit(const camp::ArrayProperty& property)
        {
            arrayVisited = true;
        }

        virtual void visit(const camp::EnumProperty& property)
        {
            enumVisited = true;
        }

        virtual void visit(const camp::UserProperty& property)
        {
            userVisited = true;
        }

        virtual void visit(const camp::Function& function)
        {
            functionVisited = true;
        }

        bool simpleVisited;
        bool arrayVisited;
        bool enumVisited;
        bool userVisited;
        bool functionVisited;
    };

    enum MyEnum
    {
    };

    struct MyType
    {
    };

    struct MyClass
    {
        int simpleProp;
        std::string arrayProp[5];
        MyEnum enumProp;
        MyType userProp;
        void function() {}
    };

    void declare()
    {
        camp::Enum::declare<MyEnum>("ClassVisitorTest::MyEnum");

        camp::Class::declare<MyType>("ClassVisitorTest::MyType");

        camp::Class::declare<MyClass>("ClassVisitorTest::MyClass")
            .property("simple", &MyClass::simpleProp)
            .property("array", &MyClass::arrayProp)
            .property("enum", &MyClass::enumProp)
            .property("user", &MyClass::userProp)
            .function("function", &MyClass::function);
    }
}

CAMP_AUTO_TYPE(ClassVisitorTest::MyEnum, &ClassVisitorTest::declare)
CAMP_AUTO_TYPE(ClassVisitorTest::MyType, &ClassVisitorTest::declare)
CAMP_AUTO_TYPE(ClassVisitorTest::MyClass, &ClassVisitorTest::declare)

#endif // CAMPTEST_CLASSVISITOR_HPP
