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

CAMP_AUTO_TYPE(ClassVisitorTest::MyEnum, &ClassVisitorTest::declare);
CAMP_AUTO_TYPE(ClassVisitorTest::MyType, &ClassVisitorTest::declare);
CAMP_AUTO_TYPE(ClassVisitorTest::MyClass, &ClassVisitorTest::declare);

#endif // CAMPTEST_CLASSVISITOR_HPP
