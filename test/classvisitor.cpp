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


#include "classvisitor.hpp"
#include <camp/class.hpp>
#include <camp/classget.hpp>
#include <boost/test/unit_test.hpp>

using namespace ClassVisitorTest;

//-----------------------------------------------------------------------------
struct ClassVisitorFixture
{
    ClassVisitorFixture()
    {
        camp::Enum::declare<MyEnum>("MyEnum");

        camp::Class::declare<MyType>("MyType");

        camp::Class::declare<MyClass>("MyClass")
            .property("simple", &MyClass::simpleProp)
            .property("array", &MyClass::arrayProp)
            .property("enum", &MyClass::enumProp)
            .property("user", &MyClass::userProp)
            .function("function", &MyClass::function);
    }

    ~ClassVisitorFixture()
    {
        camp::Class::undeclare<MyClass>();
        camp::Class::undeclare<MyType>();
        camp::Enum::undeclare<MyEnum>();
    }
};


//-----------------------------------------------------------------------------
//                         Tests for camp::ClassVisitor
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(CLASSVISITOR, ClassVisitorFixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(visit)
{
    MyClassVisitor visitor;
    camp::classByType<MyClass>().visit(visitor);

    BOOST_CHECK(visitor.simpleVisited);
    BOOST_CHECK(visitor.arrayVisited);
    BOOST_CHECK(visitor.enumVisited);
    BOOST_CHECK(visitor.userVisited);
    BOOST_CHECK(visitor.functionVisited);
}

BOOST_AUTO_TEST_SUITE_END()
