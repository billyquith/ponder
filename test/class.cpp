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

#include "class.hpp"
#include <ponder/classget.hpp>
#include <boost/test/unit_test.hpp>

using namespace ClassTest;

//-----------------------------------------------------------------------------
//                         Tests for ponder::Class
//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(CLASS)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(declare)
{
    std::size_t count = ponder::classCount();

    ponder::Class::declare<MyTempClass>("ClassTest::MyTempClass");

    BOOST_CHECK_EQUAL(ponder::classCount(), count + 1);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(declareExceptions)
{
    // to make sure it is declared
    ponder::classByType<MyClass>();

    BOOST_CHECK_THROW(ponder::Class::declare<MyClass>(), ponder::ClassAlreadyCreated);
    BOOST_CHECK_THROW(ponder::Class::declare<MyUndeclaredClass>("ClassTest::MyClass"), ponder::ClassAlreadyCreated);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(get)
{
    MyClass object;
    MyUndeclaredClass object2;

    BOOST_CHECK_EQUAL(ponder::classByName("ClassTest::MyClass").name(), "ClassTest::MyClass");
    BOOST_CHECK_EQUAL(ponder::classByType<MyClass>().name(),            "ClassTest::MyClass");
    BOOST_CHECK_EQUAL(ponder::classByObject(object).name(),             "ClassTest::MyClass");
    BOOST_CHECK_EQUAL(ponder::classByObject(&object).name(),            "ClassTest::MyClass");
    BOOST_CHECK_EQUAL(ponder::classByTypeSafe<MyUndeclaredClass>(),     static_cast<ponder::Class*>(0));

    BOOST_CHECK_THROW(ponder::classByName("ClassTest::MyUndeclaredClass"), ponder::ClassNotFound);
    BOOST_CHECK_THROW(ponder::classByType<MyUndeclaredClass>(),            ponder::ClassNotFound);
    BOOST_CHECK_THROW(ponder::classByObject(object2),                      ponder::ClassNotFound);
    BOOST_CHECK_THROW(ponder::classByObject(&object2),                     ponder::ClassNotFound);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(comparisons)
{
    const ponder::Class& class1 = ponder::classByType<MyClass>();
    const ponder::Class& class2 = ponder::classByType<MyClass2>();

    BOOST_CHECK(class1 == class1);
    BOOST_CHECK(class1 != class2);
    BOOST_CHECK(class2 != class1);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(properties)
{
    const ponder::Class& metaclass = ponder::classByType<MyClass>();

    BOOST_CHECK_EQUAL(metaclass.propertyCount(), 1U);
    BOOST_CHECK_EQUAL(metaclass.hasProperty("prop"), true);
    BOOST_CHECK_EQUAL(metaclass.hasProperty("xxxx"), false);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(functions)
{
    const ponder::Class& metaclass = ponder::classByType<MyClass>();

    BOOST_CHECK_EQUAL(metaclass.functionCount(), 1U);
    BOOST_CHECK_EQUAL(metaclass.hasFunction("func"), true);
    BOOST_CHECK_EQUAL(metaclass.hasFunction("xxxx"), false);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(inheritance)
{
    const ponder::Class& derived = ponder::classByType<Derived>();

    BOOST_CHECK_EQUAL(derived.baseCount(), 1U);
    BOOST_CHECK_EQUAL(derived.base(0).name(), "ClassTest::Base");
    BOOST_CHECK_THROW(derived.base(1), ponder::OutOfRange);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(rtti)
{
    Base* base    = new Base;
    Base* derived = new Derived;
    Base* nortti  = new DerivedNoRtti;
    Base* nortti2 = new Derived2NoRtti;

    BOOST_CHECK_EQUAL(ponder::classByObject(base).name(),     "ClassTest::Base");    // base is really a base
    BOOST_CHECK_EQUAL(ponder::classByObject(*base).name(),    "ClassTest::Base");
    BOOST_CHECK_EQUAL(ponder::classByObject(derived).name(),  "ClassTest::Derived"); // CAMP finds its real type thanks to PONDER_RTTI
    BOOST_CHECK_EQUAL(ponder::classByObject(*derived).name(), "ClassTest::Derived");
    BOOST_CHECK_EQUAL(ponder::classByObject(nortti).name(),   "ClassTest::Base");    // CAMP fails to find its derived type without PONDER_RTTI
    BOOST_CHECK_EQUAL(ponder::classByObject(*nortti).name(),  "ClassTest::Base");
    BOOST_CHECK_EQUAL(ponder::classByObject(nortti2).name(),  "ClassTest::Derived"); // CAMP finds the closest derived type which has PONDER_RTTI
    BOOST_CHECK_EQUAL(ponder::classByObject(*nortti2).name(), "ClassTest::Derived");

    delete nortti2;
    delete nortti;
    delete derived;
    delete base;
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(typeNames)
{
    BOOST_CHECK(strcmp(ponder::detail::typeAsString(ponder::noType), "none")==0);
    BOOST_CHECK(strcmp(ponder::detail::typeAsString(ponder::realType), "real")==0);
    BOOST_CHECK(strcmp(ponder::detail::typeAsString(ponder::userType), "user")==0);
}

BOOST_AUTO_TEST_SUITE_END()
