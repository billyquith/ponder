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

#include "arrayproperty.hpp"
#include <camp/classget.hpp>
#include <camp/errors.hpp>
#include <camp/arrayproperty.hpp>
#include <boost/test/unit_test.hpp>

using namespace ArrayPropertyTest;

//-----------------------------------------------------------------------------
struct ArrayPropertyFixture
{
    ArrayPropertyFixture()
    {
        const camp::Class& metaclass = camp::classByType<MyClass>();
        bools   = &static_cast<const camp::ArrayProperty&>(metaclass.property("bools"));
        ints    = &static_cast<const camp::ArrayProperty&>(metaclass.property("ints"));
        strings = &static_cast<const camp::ArrayProperty&>(metaclass.property("strings"));
        objects = &static_cast<const camp::ArrayProperty&>(metaclass.property("objects"));
    }

    const camp::ArrayProperty* bools;
    const camp::ArrayProperty* ints;
    const camp::ArrayProperty* strings;
    const camp::ArrayProperty* objects;
    MyClass object;
};

//-----------------------------------------------------------------------------
//                         Tests for camp::ArrayProperty
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(ARRAYPROPERTY, ArrayPropertyFixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(type)
{
    BOOST_CHECK_EQUAL(bools->type(),   camp::arrayType);
    BOOST_CHECK_EQUAL(ints->type(),    camp::arrayType);
    BOOST_CHECK_EQUAL(strings->type(), camp::arrayType);
    BOOST_CHECK_EQUAL(objects->type(), camp::arrayType);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(elementType)
{
    BOOST_CHECK_EQUAL(bools->elementType(),   camp::boolType);
    BOOST_CHECK_EQUAL(ints->elementType(),    camp::intType);
    BOOST_CHECK_EQUAL(strings->elementType(), camp::stringType);
    BOOST_CHECK_EQUAL(objects->elementType(), camp::userType);
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
    BOOST_CHECK_EQUAL(bools->size(object),   std::extent<typeof object.bools>::value);
    BOOST_CHECK_EQUAL(ints->size(object),    object.ints.size());
    BOOST_CHECK_EQUAL(strings->size(object), object.strings.size());
    BOOST_CHECK_EQUAL(objects->size(object), object.objects.size());
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(get)
{
    BOOST_CHECK_EQUAL(bools->get(object, 0), camp::Value(object.bools[0]));
    BOOST_CHECK_EQUAL(bools->get(object, 1), camp::Value(object.bools[1]));
    BOOST_CHECK_THROW(bools->get(object, 2), camp::OutOfRange);

    BOOST_CHECK_EQUAL(ints->get(object, 0), camp::Value(object.ints[0]));
    BOOST_CHECK_EQUAL(ints->get(object, 1), camp::Value(object.ints[1]));
    BOOST_CHECK_EQUAL(ints->get(object, 2), camp::Value(object.ints[2]));
    BOOST_CHECK_THROW(ints->get(object, 3), camp::OutOfRange);

    BOOST_CHECK_EQUAL(strings->get(object, 0), camp::Value(object.strings[0]));
    BOOST_CHECK_EQUAL(strings->get(object, 1), camp::Value(object.strings[1]));
    BOOST_CHECK_EQUAL(strings->get(object, 2), camp::Value(object.strings[2]));
    BOOST_CHECK_EQUAL(strings->get(object, 3), camp::Value(object.strings[3]));
    BOOST_CHECK_THROW(strings->get(object, 4), camp::OutOfRange);

    std::list<MyType>::const_iterator it = object.objects.begin();
    BOOST_CHECK_EQUAL(objects->get(object, 0), camp::Value(*std::next(it, 0)));
    BOOST_CHECK_EQUAL(objects->get(object, 1), camp::Value(*std::next(it, 1)));
    BOOST_CHECK_EQUAL(objects->get(object, 2), camp::Value(*std::next(it, 2)));
    BOOST_CHECK_EQUAL(objects->get(object, 3), camp::Value(*std::next(it, 3)));
    BOOST_CHECK_EQUAL(objects->get(object, 4), camp::Value(*std::next(it, 4)));
    BOOST_CHECK_THROW(objects->get(object, 5), camp::OutOfRange);
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

    BOOST_CHECK_THROW(bools->set(object, 10, true),        camp::OutOfRange);
    BOOST_CHECK_THROW(ints->set(object, 10, 1),            camp::OutOfRange);
    BOOST_CHECK_THROW(strings->set(object, 10, "hi"),      camp::OutOfRange);
    BOOST_CHECK_THROW(objects->set(object, 10, MyType(9)), camp::OutOfRange);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(insert)
{
    BOOST_CHECK_THROW(bools->insert(object, 0, true), camp::ForbiddenWrite);
    BOOST_CHECK_THROW(ints->insert(object, 0, true),  camp::ForbiddenWrite);

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
    BOOST_CHECK_THROW(bools->remove(object, 0), camp::ForbiddenWrite);
    BOOST_CHECK_THROW(ints->remove(object, 0),  camp::ForbiddenWrite);

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
