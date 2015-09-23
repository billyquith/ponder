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
#include <camp/classget.hpp>
#include <boost/test/unit_test.hpp>

using namespace ClassTest;

static void foo() {}
static int bar(float) {return 0;}
struct Callable {
    void operator () () {}
};
struct NonCallable {
    int x;
};
struct Methods
{
    void foo() {}
};

//-----------------------------------------------------------------------------
//                         Tests for camp::Class
//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(CLASS)

BOOST_AUTO_TEST_CASE(isCallable)
{
    foo(); bar(0.f); // to stop warning

    static_assert(std::is_function<decltype(foo)>::value, "std::is_function failed");
    static_assert(std::is_function<void ()>::value, "std::is_function failed");
    static_assert(!std::is_function<Callable>::value, "std::is_function failed");
    static_assert(!std::is_function<NonCallable>::value, "std::is_function failed");
    
    typedef void (*foo_t)();
    static_assert(std::is_void< std::result_of<decltype(foo)& ()>::type >::value, "std::result_of failed");
    static_assert(std::is_void< std::result_of<decltype(&foo) ()>::type >::value, "std::result_of failed");
    static_assert(std::is_void< std::result_of<foo_t ()>::type >::value, "std::result_of failed");

    typedef int (*bar_t)(float);
    static_assert(std::is_same< std::result_of<decltype(bar)& (float)>::type, int >::value, "std::result_of failed");
    static_assert(std::is_same< std::result_of<decltype(&bar) (float)>::type, int >::value, "std::result_of failed");
    static_assert(std::is_same< std::result_of<bar_t (float)>::type, int >::value, "std::result_of failed");
}

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

    BOOST_CHECK_THROW(camp::Class::declare<MyClass>(), camp::ClassAlreadyCreated);
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
