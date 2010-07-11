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
    BOOST_CHECK_EQUAL(bools->size(object),   boost::size(object.bools));
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
    BOOST_CHECK_EQUAL(objects->get(object, 0), camp::Value(*boost::next(it, 0)));
    BOOST_CHECK_EQUAL(objects->get(object, 1), camp::Value(*boost::next(it, 1)));
    BOOST_CHECK_EQUAL(objects->get(object, 2), camp::Value(*boost::next(it, 2)));
    BOOST_CHECK_EQUAL(objects->get(object, 3), camp::Value(*boost::next(it, 3)));
    BOOST_CHECK_EQUAL(objects->get(object, 4), camp::Value(*boost::next(it, 4)));
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
    BOOST_CHECK(*boost::next(object.objects.begin(), 1) == MyType(8));

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
    BOOST_CHECK(*boost::next(object.objects.begin(), 1) == MyType(10));
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(remove)
{
    BOOST_CHECK_THROW(bools->remove(object, 0), camp::ForbiddenWrite);
    BOOST_CHECK_THROW(ints->remove(object, 0),  camp::ForbiddenWrite);

    std::string string1 = object.strings[1];
    MyType      object1 = *boost::next(object.objects.begin(), 1);

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
