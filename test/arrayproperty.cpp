/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
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

#include <ponder/classget.hpp>
#include <ponder/errors.hpp>
#include <ponder/arrayproperty.hpp>
#include <ponder/pondertype.hpp>
#include <ponder/class.hpp>
#include <ponder/classbuilder.hpp>
#include <boost/test/unit_test.hpp>
#include <list>
#include <vector>

namespace ArrayPropertyTest
{
    struct MyType
    {
        MyType(int x_) : x(x_)
        {
        }
        
        bool operator ==(const MyType& other) const
        {
            return x == other.x;
        }
        
        int x;
    };
    
    struct MyClass
    {
        MyClass()
        {
            bools[0] = true;
            bools[1] = false;
            
            ints[0] = -10;
            ints[1] = 10;
            ints[2] = 100;
            
            strings.push_back("string 0");
            strings.push_back("string 1");
            strings.push_back("string 2");
            strings.push_back("string 3");
            
            objects.push_back(MyType(0));
            objects.push_back(MyType(1));
            objects.push_back(MyType(2));
            objects.push_back(MyType(3));
            objects.push_back(MyType(4));
        }
        
        bool bools[2];
        std::array<int, 3> ints;
        std::vector<std::string> strings;
        std::list<MyType> objects;
    };
    
    void declare()
    {
        ponder::Class::declare<MyType>("ArrayPropertyTest::MyType");
        
        ponder::Class::declare<MyClass>("ArrayPropertyTest::MyClass")
        .property("bools", &MyClass::bools)
        .property("ints", &MyClass::ints)
        .property("strings", &MyClass::strings)
        .property("objects", &MyClass::objects);
    }
}

PONDER_AUTO_TYPE(ArrayPropertyTest::MyType, &ArrayPropertyTest::declare)
PONDER_AUTO_TYPE(ArrayPropertyTest::MyClass, &ArrayPropertyTest::declare)

using namespace ArrayPropertyTest;

//-----------------------------------------------------------------------------
struct ArrayPropertyFixture
{
    ArrayPropertyFixture()
    {
        const ponder::Class& metaclass = ponder::classByType<MyClass>();
        bools   = &static_cast<const ponder::ArrayProperty&>(metaclass.property("bools"));
        ints    = &static_cast<const ponder::ArrayProperty&>(metaclass.property("ints"));
        strings = &static_cast<const ponder::ArrayProperty&>(metaclass.property("strings"));
        objects = &static_cast<const ponder::ArrayProperty&>(metaclass.property("objects"));
    }

    const ponder::ArrayProperty* bools;
    const ponder::ArrayProperty* ints;
    const ponder::ArrayProperty* strings;
    const ponder::ArrayProperty* objects;
    MyClass object;
};

//-----------------------------------------------------------------------------
//                         Tests for ponder::ArrayProperty
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(ARRAYPROPERTY, ArrayPropertyFixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(type)
{
    BOOST_CHECK_EQUAL(bools->type(),   ponder::arrayType);
    BOOST_CHECK_EQUAL(ints->type(),    ponder::arrayType);
    BOOST_CHECK_EQUAL(strings->type(), ponder::arrayType);
    BOOST_CHECK_EQUAL(objects->type(), ponder::arrayType);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(elementType)
{
    BOOST_CHECK_EQUAL(bools->elementType(),   ponder::boolType);
    BOOST_CHECK_EQUAL(ints->elementType(),    ponder::intType);
    BOOST_CHECK_EQUAL(strings->elementType(), ponder::stringType);
    BOOST_CHECK_EQUAL(objects->elementType(), ponder::userType);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(dynamic)
{
    BOOST_CHECK_EQUAL(bools->dynamic(),   false);
    BOOST_CHECK_EQUAL(ints->dynamic(),    false);
    BOOST_CHECK_EQUAL(strings->dynamic(), true);
    BOOST_CHECK_EQUAL(objects->dynamic(), true);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(size)
{
#ifndef _WIN32
    BOOST_CHECK_EQUAL(bools->size(object),   std::extent<typeof object.bools>::value); // TODO - Compilation fails on Windows.
#endif
    BOOST_CHECK_EQUAL(ints->size(object),    object.ints.size());
    BOOST_CHECK_EQUAL(strings->size(object), object.strings.size());
    BOOST_CHECK_EQUAL(objects->size(object), object.objects.size());
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(get)
{
    BOOST_CHECK_EQUAL(bools->get(object, 0), ponder::Value(object.bools[0]));
    BOOST_CHECK_EQUAL(bools->get(object, 1), ponder::Value(object.bools[1]));
    BOOST_CHECK_THROW(bools->get(object, 2), ponder::OutOfRange);

    BOOST_CHECK_EQUAL(ints->get(object, 0), ponder::Value(object.ints[0]));
    BOOST_CHECK_EQUAL(ints->get(object, 1), ponder::Value(object.ints[1]));
    BOOST_CHECK_EQUAL(ints->get(object, 2), ponder::Value(object.ints[2]));
    BOOST_CHECK_THROW(ints->get(object, 3), ponder::OutOfRange);

    BOOST_CHECK_EQUAL(strings->get(object, 0), ponder::Value(object.strings[0]));
    BOOST_CHECK_EQUAL(strings->get(object, 1), ponder::Value(object.strings[1]));
    BOOST_CHECK_EQUAL(strings->get(object, 2), ponder::Value(object.strings[2]));
    BOOST_CHECK_EQUAL(strings->get(object, 3), ponder::Value(object.strings[3]));
    BOOST_CHECK_THROW(strings->get(object, 4), ponder::OutOfRange);

    std::list<MyType>::const_iterator it = object.objects.begin();
    BOOST_CHECK_EQUAL(objects->get(object, 0), ponder::Value(*std::next(it, 0)));
    BOOST_CHECK_EQUAL(objects->get(object, 1), ponder::Value(*std::next(it, 1)));
    BOOST_CHECK_EQUAL(objects->get(object, 2), ponder::Value(*std::next(it, 2)));
    BOOST_CHECK_EQUAL(objects->get(object, 3), ponder::Value(*std::next(it, 3)));
    BOOST_CHECK_EQUAL(objects->get(object, 4), ponder::Value(*std::next(it, 4)));
    BOOST_CHECK_THROW(objects->get(object, 5), ponder::OutOfRange);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(set)
{
    bools->set(object, 1, true);
    ints->set(object, 1, 20);
    strings->set(object, 1, "hello");
    objects->set(object, 1, MyType(8));

    BOOST_CHECK_EQUAL(object.bools[1],   true);
    BOOST_CHECK_EQUAL(object.ints[1],    20);
    BOOST_CHECK_EQUAL(object.strings[1], "hello");
    BOOST_CHECK(*std::next(object.objects.begin(), 1) == MyType(8));

    BOOST_CHECK_THROW(bools->set(object, 10, true),        ponder::OutOfRange);
    BOOST_CHECK_THROW(ints->set(object, 10, 1),            ponder::OutOfRange);
    BOOST_CHECK_THROW(strings->set(object, 10, "hi"),      ponder::OutOfRange);
    BOOST_CHECK_THROW(objects->set(object, 10, MyType(9)), ponder::OutOfRange);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(insert)
{
    BOOST_CHECK_THROW(bools->insert(object, 0, true), ponder::ForbiddenWrite);
    BOOST_CHECK_THROW(ints->insert(object, 0, true),  ponder::ForbiddenWrite);

    std::size_t stringsSize = object.strings.size();
    std::size_t objectsSize = object.objects.size();

    strings->insert(object, 1, "bonjour");
    objects->insert(object, 1, MyType(10));

    BOOST_CHECK_EQUAL(object.strings.size(), stringsSize + 1);
    BOOST_CHECK_EQUAL(object.objects.size(), objectsSize + 1);

    BOOST_CHECK_EQUAL(object.strings[1], "bonjour");
    BOOST_CHECK(*std::next(object.objects.begin(), 1) == MyType(10));
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(remove)
{
    BOOST_CHECK_THROW(bools->remove(object, 0), ponder::ForbiddenWrite);
    BOOST_CHECK_THROW(ints->remove(object, 0),  ponder::ForbiddenWrite);

    std::string string1 = object.strings[1];
    MyType      object1 = *std::next(object.objects.begin(), 1);

    std::size_t stringsSize = object.strings.size();
    std::size_t objectsSize = object.objects.size();

    strings->remove(object, 0);
    objects->remove(object, 0);

    BOOST_CHECK_EQUAL(object.strings.size(), stringsSize - 1);
    BOOST_CHECK_EQUAL(object.objects.size(), objectsSize - 1);

    BOOST_CHECK(object.strings.front() == string1);
    BOOST_CHECK(object.objects.front() == object1);
}

BOOST_AUTO_TEST_SUITE_END()
