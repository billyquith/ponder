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

#include "class.hpp"
#include <camp/classget.hpp>
#include <camp/errors.hpp>
#include <boost/test/unit_test.hpp>

using namespace ClassTest;

//-----------------------------------------------------------------------------
//                         Tests for camp::Class
//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(CLASS)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(declare)
{
    std::size_t count = camp::classCount();

    camp::Class::declare<MyTempClass>("ClassTest::MyTempClass");

    BOOST_CHECK_EQUAL(camp::classCount(), count + 1);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(declareExceptions)
{
    // to make sure it is declared
    camp::classByType<MyClass>();

    BOOST_CHECK_THROW(camp::Class::declare<MyClass>("ClassTest::MyUndeclaredClass"), camp::ClassAlreadyCreated);
    BOOST_CHECK_THROW(camp::Class::declare<MyUndeclaredClass>("ClassTest::MyClass"), camp::ClassAlreadyCreated);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(get)
{
    MyClass object;
    MyUndeclaredClass object2;

    BOOST_CHECK_EQUAL(camp::classByName("ClassTest::MyClass").name(), "ClassTest::MyClass");
    BOOST_CHECK_EQUAL(camp::classByType<MyClass>().name(),            "ClassTest::MyClass");
    BOOST_CHECK_EQUAL(camp::classByObject(object).name(),             "ClassTest::MyClass");
    BOOST_CHECK_EQUAL(camp::classByObject(&object).name(),            "ClassTest::MyClass");
    BOOST_CHECK_EQUAL(camp::classByTypeSafe<MyUndeclaredClass>(),     static_cast<camp::Class*>(0));

    BOOST_CHECK_THROW(camp::classByName("ClassTest::MyUndeclaredClass"), camp::ClassNotFound);
    BOOST_CHECK_THROW(camp::classByType<MyUndeclaredClass>(),            camp::ClassNotFound);
    BOOST_CHECK_THROW(camp::classByObject(object2),                      camp::ClassNotFound);
    BOOST_CHECK_THROW(camp::classByObject(&object2),                     camp::ClassNotFound);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(comparisons)
{
    const camp::Class& class1 = camp::classByType<MyClass>();
    const camp::Class& class2 = camp::classByType<MyClass2>();

    BOOST_CHECK(class1 == class1);
    BOOST_CHECK(class1 != class2);
    BOOST_CHECK(class2 != class1);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(properties)
{
    const camp::Class& metaclass = camp::classByType<MyClass>();

    BOOST_CHECK_EQUAL(metaclass.propertyCount(), 1U);
    BOOST_CHECK_EQUAL(metaclass.hasProperty("prop"), true);
    BOOST_CHECK_EQUAL(metaclass.hasProperty("xxxx"), false);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(functions)
{
    const camp::Class& metaclass = camp::classByType<MyClass>();

    BOOST_CHECK_EQUAL(metaclass.functionCount(), 1U);
    BOOST_CHECK_EQUAL(metaclass.hasFunction("func"), true);
    BOOST_CHECK_EQUAL(metaclass.hasFunction("xxxx"), false);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(inheritance)
{
    const camp::Class& derived = camp::classByType<Derived>();

    BOOST_CHECK_EQUAL(derived.baseCount(), 1U);
    BOOST_CHECK_EQUAL(derived.base(0).name(), "ClassTest::Base");
    BOOST_CHECK_THROW(derived.base(1), camp::OutOfRange);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(rtti)
{
    Base* base    = new Base;
    Base* derived = new Derived;
    Base* nortti  = new DerivedNoRtti;
    Base* nortti2 = new Derived2NoRtti;

    BOOST_CHECK_EQUAL(camp::classByObject(base).name(),     "ClassTest::Base");    // base is really a base
    BOOST_CHECK_EQUAL(camp::classByObject(*base).name(),    "ClassTest::Base");
    BOOST_CHECK_EQUAL(camp::classByObject(derived).name(),  "ClassTest::Derived"); // CAMP finds its real type thanks to CAMP_RTTI
    BOOST_CHECK_EQUAL(camp::classByObject(*derived).name(), "ClassTest::Derived");
    BOOST_CHECK_EQUAL(camp::classByObject(nortti).name(),   "ClassTest::Base");    // CAMP fails to find its derived type without CAMP_RTTI
    BOOST_CHECK_EQUAL(camp::classByObject(*nortti).name(),  "ClassTest::Base");
    BOOST_CHECK_EQUAL(camp::classByObject(nortti2).name(),  "ClassTest::Derived"); // CAMP finds the closest derived type which has CAMP_RTTI
    BOOST_CHECK_EQUAL(camp::classByObject(*nortti2).name(), "ClassTest::Derived");

    delete nortti2;
    delete nortti;
    delete derived;
    delete base;
}

BOOST_AUTO_TEST_SUITE_END()
