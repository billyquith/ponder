/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2018 Nick Trout.
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

#include <ponder/classget.hpp>
#include <ponder/class.hpp>
#include <ponder/enum.hpp>
#include <ponder/classvisitor.hpp>
#include <ponder/simpleproperty.hpp>
#include <ponder/arrayproperty.hpp>
#include <ponder/enumproperty.hpp>
#include <ponder/userproperty.hpp>
#include <ponder/function.hpp>
#include <ponder/classbuilder.hpp>
#include "test.hpp"

namespace ClassVisitorTest
{
    class MyClassVisitor : public ponder::ClassVisitor
    {
    public:
        
        MyClassVisitor()
        : simpleVisited(false)
        , arrayVisited(false)
        , enumVisited(false)
        , userVisited(false)
        , functionVisited(false)
        {}
        
        void visit(const ponder::SimpleProperty& property) override
        {
            simpleVisited = true;
        }
        
        void visit(const ponder::ArrayProperty& property) override
        {
            arrayVisited = true;
        }
        
        void visit(const ponder::EnumProperty& property) override
        {
            enumVisited = true;
        }
        
        void visit(const ponder::UserProperty& property) override
        {
            userVisited = true;
        }
        
        void visit(const ponder::Function& function) override
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
        ponder::String arrayProp[5];
        MyEnum enumProp;
        MyType userProp;
        void function() {}
    };
    
    void declare()
    {
        ponder::Enum::declare<MyEnum>();
        
        ponder::Class::declare<MyType>();
        
        ponder::Class::declare<MyClass>()
            .property("simple", &MyClass::simpleProp)
            .property("array", &MyClass::arrayProp)
            .property("enum", &MyClass::enumProp)
            .property("user", &MyClass::userProp)
            .function("function", &MyClass::function)
            ;
    }
}

PONDER_AUTO_TYPE(ClassVisitorTest::MyEnum, &ClassVisitorTest::declare)
PONDER_AUTO_TYPE(ClassVisitorTest::MyType, &ClassVisitorTest::declare)
PONDER_AUTO_TYPE(ClassVisitorTest::MyClass, &ClassVisitorTest::declare)

using namespace ClassVisitorTest;

//-----------------------------------------------------------------------------
//                         Tests for ponder::ClassVisitor
//-----------------------------------------------------------------------------


TEST_CASE("Classes can have visitors")
{
    MyClassVisitor visitor;
    ponder::classByType<MyClass>().visit(visitor);

    REQUIRE(visitor.simpleVisited);
    REQUIRE(visitor.arrayVisited);
    REQUIRE(visitor.enumVisited);
    REQUIRE(visitor.userVisited);
    REQUIRE(visitor.functionVisited);
}
