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
#include <camp/class.hpp>
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
    BOOST_CHECK_EQUAL(camp::classCount(), 0U);

    camp::Class::declare<MyClass>("MyClass");
    BOOST_CHECK_EQUAL(camp::classCount(), 1U);

    camp::Class::undeclare<MyClass>();
    BOOST_CHECK_EQUAL(camp::classCount(), 0U);

    camp::Class::declare<MyClass>("MyClass");
    BOOST_CHECK_EQUAL(camp::classCount(), 1U);

    camp::Class::undeclare("MyClass");
    BOOST_CHECK_EQUAL(camp::classCount(), 0U);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(declareExceptions)
{
    // Setup
    camp::Class::declare<MyClass>("MyClass");

    // Tests
    BOOST_CHECK_THROW(camp::Class::declare<MyClass>("MyClass2"), camp::ClassAlreadyCreated);
    BOOST_CHECK_THROW(camp::Class::declare<MyClass2>("MyClass"), camp::ClassAlreadyCreated);

    // Cleanup
    camp::Class::undeclare<MyClass>();
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(get)
{
    // Setup
    camp::Class::declare<MyClass>("MyClass");
    MyClass object;
    MyClass2 object2;

    // Tests
    BOOST_CHECK_EQUAL(camp::classByIndex(0).name(),        "MyClass");
    BOOST_CHECK_EQUAL(camp::classByName("MyClass").name(), "MyClass");
    BOOST_CHECK_EQUAL(camp::classByType<MyClass>().name(), "MyClass");
    BOOST_CHECK_EQUAL(camp::classByObject(object).name(),  "MyClass");
    BOOST_CHECK_EQUAL(camp::classByObject(&object).name(), "MyClass");
    BOOST_CHECK_EQUAL(camp::classByTypeSafe<MyClass2>(),   static_cast<camp::Class*>(0));

    BOOST_CHECK_THROW(camp::classByIndex(1).name(),  camp::OutOfRange);
    BOOST_CHECK_THROW(camp::classByName("MyClass2"), camp::ClassNotFound);
    BOOST_CHECK_THROW(camp::classByType<MyClass2>(), camp::ClassNotFound);
    BOOST_CHECK_THROW(camp::classByObject(object2),  camp::ClassNotFound);
    BOOST_CHECK_THROW(camp::classByObject(&object2), camp::ClassNotFound);

    // Cleanup
    camp::Class::undeclare<MyClass>();
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(comparisons)
{
    // Setup
    camp::Class::declare<MyClass>("MyClass");
    camp::Class::declare<MyClass2>("MyClass2");
    const camp::Class& class1 = camp::classByType<MyClass>();
    const camp::Class& class2 = camp::classByType<MyClass2>();

    // Tests
    BOOST_CHECK(class1 == class1);
    BOOST_CHECK(class1 != class2);
    BOOST_CHECK(class2 != class1);

    // Cleanup
    camp::Class::undeclare<MyClass>();
    camp::Class::undeclare<MyClass2>();
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(properties)
{
    // Setup
    camp::Class::declare<MyClass>("MyClass")
        .property("prop", &MyClass::prop);
    const camp::Class& metaclass = camp::classByType<MyClass>();

    // Tests
    BOOST_CHECK_EQUAL(metaclass.propertyCount(), 1U);
    BOOST_CHECK_EQUAL(metaclass.hasProperty("prop"), true);
    BOOST_CHECK_EQUAL(metaclass.hasProperty("xxxx"), false);

    // Cleanup
    camp::Class::undeclare<MyClass>();
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(functions)
{
    // Setup
    camp::Class::declare<MyClass>("Dummy")
        .function("func", &MyClass::func);
    const camp::Class& metaclass = camp::classByType<MyClass>();

    // Tests
    BOOST_CHECK_EQUAL(metaclass.functionCount(), 1U);
    BOOST_CHECK_EQUAL(metaclass.hasFunction("func"), true);
    BOOST_CHECK_EQUAL(metaclass.hasFunction("xxxx"), false);

    // Cleanup
    camp::Class::undeclare<MyClass>();
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(inheritance)
{
    // Setup
    camp::Class::declare<Base>("Base");
    camp::Class::declare<Derived>("Derived")
        .base<Base>();
    const camp::Class& derived = camp::classByType<Derived>();

    // Tests
    BOOST_CHECK_EQUAL(derived.baseCount(), 1U);
    BOOST_CHECK_EQUAL(derived.base(0).name(), "Base");
    BOOST_CHECK_THROW(derived.base(1), camp::OutOfRange);

    // Cleanup
    camp::Class::undeclare<Derived>();
    camp::Class::undeclare<Base>();
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(rtti)
{
    // Setup
    camp::Class::declare<Base>("Base");
    camp::Class::declare<Derived>("Derived")
        .base<Base>();
    camp::Class::declare<DerivedNoRtti>("DerivedNoRtti")
        .base<Base>();
    camp::Class::declare<Derived2NoRtti>("Derived2NoRtti")
        .base<Derived>();
    Base* base    = new Base;
    Base* derived = new Derived;
    Base* nortti  = new DerivedNoRtti;
    Base* nortti2 = new Derived2NoRtti;

    // Tests
    BOOST_CHECK_EQUAL(camp::classByObject(base).name(),     "Base");    // base is really a base
    BOOST_CHECK_EQUAL(camp::classByObject(*base).name(),    "Base");
    BOOST_CHECK_EQUAL(camp::classByObject(derived).name(),  "Derived"); // CAMP finds its real type thanks to CAMP_RTTI
    BOOST_CHECK_EQUAL(camp::classByObject(*derived).name(), "Derived");
    BOOST_CHECK_EQUAL(camp::classByObject(nortti).name(),   "Base");    // CAMP fails to find its derived type without CAMP_RTTI
    BOOST_CHECK_EQUAL(camp::classByObject(*nortti).name(),  "Base");
    BOOST_CHECK_EQUAL(camp::classByObject(nortti2).name(),  "Derived"); // CAMP finds the closest derived type which has CAMP_RTTI
    BOOST_CHECK_EQUAL(camp::classByObject(*nortti2).name(), "Derived");

    // Cleanup
    delete nortti2;
    delete nortti;
    delete derived;
    delete base;
    camp::Class::undeclare<Derived2NoRtti>();
    camp::Class::undeclare<DerivedNoRtti>();
    camp::Class::undeclare<Derived>();
    camp::Class::undeclare<Base>();
}

BOOST_AUTO_TEST_SUITE_END()
