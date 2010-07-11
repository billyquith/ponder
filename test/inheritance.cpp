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

#include "inheritance.hpp"
#include <camp/class.hpp>
#include <camp/classget.hpp>
#include <camp/errors.hpp>
#include <boost/test/unit_test.hpp>

using namespace InheritanceTest;

//-----------------------------------------------------------------------------
struct InheritanceFixture
{
    InheritanceFixture()
    {
        class1 = &camp::classByType<MyClass1>();
        class2 = &camp::classByType<MyClass2>();
        class3 = &camp::classByType<MyClass3>();
        class4 = &camp::classByType<MyClass4>();
    }

    const camp::Class* class1;
    const camp::Class* class2;
    const camp::Class* class3;
    const camp::Class* class4;
};

//-----------------------------------------------------------------------------
//                         Tests for inherited metaclasses
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(INHERITANCE, InheritanceFixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(regularFunctions)
{
    MyClass1 object1;
    MyClass2 object2;
    MyClass3 object3;
    MyClass4 object4;

    BOOST_CHECK_EQUAL(class1->function("f1").call(object1), camp::Value(1));
    BOOST_CHECK_EQUAL(class3->function("f1").call(object3), camp::Value(1));
    BOOST_CHECK_EQUAL(class4->function("f1").call(object4), camp::Value(1));

    BOOST_CHECK_EQUAL(class2->function("f2").call(object2), camp::Value(2));
    BOOST_CHECK_EQUAL(class3->function("f2").call(object3), camp::Value(2));
    BOOST_CHECK_EQUAL(class4->function("f2").call(object4), camp::Value(2));

    BOOST_CHECK_EQUAL(class3->function("f3").call(object3), camp::Value(3));
    BOOST_CHECK_EQUAL(class4->function("f3").call(object4), camp::Value(3));

    BOOST_CHECK_EQUAL(class4->function("f4").call(object4), camp::Value(4));
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(virtualFunctions)
{
    MyClass1 object1;
    MyClass2 object2;
    MyClass3 object3;
    MyClass4 object4;

    BOOST_CHECK_EQUAL(class2->function("virtual").call(object2), camp::Value(20));
    BOOST_CHECK_EQUAL(class3->function("virtual").call(object3), camp::Value(30));
    BOOST_CHECK_EQUAL(class4->function("virtual").call(object4), camp::Value(40));

    BOOST_CHECK_EQUAL(class2->function("virtual").call(object2), camp::Value(20));
    BOOST_CHECK_EQUAL(class2->function("virtual").call(object3), camp::Value(30));
    BOOST_CHECK_EQUAL(class2->function("virtual").call(object4), camp::Value(40));
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(properties)
{
    MyClass1 object1;
    MyClass2 object2;
    MyClass3 object3;
    MyClass4 object4;

    BOOST_CHECK_EQUAL(class1->property("p1").get(object1), camp::Value(10));
    BOOST_CHECK_EQUAL(class3->property("p1").get(object3), camp::Value(10));
    BOOST_CHECK_EQUAL(class4->property("p1").get(object4), camp::Value(10));

    BOOST_CHECK_EQUAL(class2->property("p2").get(object2), camp::Value(20));
    BOOST_CHECK_EQUAL(class3->property("p2").get(object3), camp::Value(20));
    BOOST_CHECK_EQUAL(class4->property("p2").get(object4), camp::Value(20));

    BOOST_CHECK_EQUAL(class3->property("p3").get(object3), camp::Value(30));
    BOOST_CHECK_EQUAL(class4->property("p3").get(object4), camp::Value(30));

    BOOST_CHECK_EQUAL(class4->property("p4").get(object4), camp::Value(40));
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(castWithPointerOffset)
{
    MyClass3 object3;
    MyClass4 object4;

    MyClass2* base3 = &object3;
    MyClass3* base4 = &object4;

    BOOST_CHECK_EQUAL(class2->property("p2").get(base3), camp::Value(20));
    BOOST_CHECK_EQUAL(class2->property("p2").get(base4), camp::Value(20));

    BOOST_CHECK_EQUAL(class3->property("p3").get(base3), camp::Value(30));
    BOOST_CHECK_EQUAL(class3->property("p3").get(base4), camp::Value(30));

    BOOST_CHECK_EQUAL(class4->property("p4").get(base4), camp::Value(40));
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(overriddenFunction)
{
    MyClass1 object1;
    MyClass2 object2;
    MyClass3 object3;
    MyClass4 object4;

    BOOST_CHECK_EQUAL(class1->function("overridden").call(object1), camp::Value(1));
    BOOST_CHECK_EQUAL(class2->function("overridden").call(object2), camp::Value(2));
    BOOST_CHECK_EQUAL(class3->function("overridden").call(object3), camp::Value(3));
    BOOST_CHECK_EQUAL(class4->function("overridden").call(object4), camp::Value(4));

    BOOST_CHECK_EQUAL(class4->function("overridden").call(object1), camp::Value(4));
    BOOST_CHECK_EQUAL(class4->function("overridden").call(object2), camp::Value(4));
    BOOST_CHECK_EQUAL(class4->function("overridden").call(object3), camp::Value(4));
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(overriddenProperty)
{
    MyClass1 object1;
    MyClass2 object2;
    MyClass3 object3;
    MyClass4 object4;

    // Here, the result should always depend on the metaclass, not the object
    // (just like in C++ where non-virtual functions are resolved using the static type)

    BOOST_CHECK_EQUAL(class1->property("overridden").get(object1), camp::Value(10));
    BOOST_CHECK_EQUAL(class2->property("overridden").get(object2), camp::Value(20));
    BOOST_CHECK_EQUAL(class3->property("overridden").get(object3), camp::Value(30));
    BOOST_CHECK_EQUAL(class4->property("overridden").get(object4), camp::Value(40));

    BOOST_CHECK_EQUAL(class1->property("overridden").get(object3), camp::Value(10));
    BOOST_CHECK_EQUAL(class2->property("overridden").get(object3), camp::Value(20));

    BOOST_CHECK_EQUAL(class1->property("overridden").get(object4), camp::Value(10));
    BOOST_CHECK_EQUAL(class2->property("overridden").get(object4), camp::Value(20));
    BOOST_CHECK_EQUAL(class3->property("overridden").get(object4), camp::Value(30));
}

BOOST_AUTO_TEST_SUITE_END()
