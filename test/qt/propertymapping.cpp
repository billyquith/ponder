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


#include "propertymapping.hpp"
#include <camp/class.hpp>
#include <camp/classget.hpp>
#include <boost/test/unit_test.hpp>

using namespace PropertyMappingTest;

//-----------------------------------------------------------------------------
struct PropertyMappingFixture
{
    PropertyMappingFixture()
    {
        camp::Enum::declare<MyClass::Enum>("MyClass::Enum")
            .value("one", MyClass::one)
            .value("two", MyClass::two)
            .value("three", MyClass::three);

        camp::Class::declare<MyClass>("MyClass")
            .external<camp_ext::QtMapper>();

        metaclass = &camp::classByType<MyClass>();

        object.setBool(true);
        object.setInt(-10);
        object.setULong(20);
        object.setDouble(0.55);
        object.setString("hello");
        object.setEnum(MyClass::two);
    }

    ~PropertyMappingFixture()
    {
        camp::Class::undeclare<MyClass>();
        camp::Enum::undeclare<MyClass::Enum>();
    }

    MyClass object;
    const camp::Class* metaclass;
};


//-----------------------------------------------------------------------------
//                         Tests for camp_ext::QtMapper (properties)
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(PROPERTYMAPPING, PropertyMappingFixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(name)
{
    BOOST_CHECK(metaclass->hasProperty("m_bool_read"));
    BOOST_CHECK(metaclass->hasProperty("m_int_read"));
    BOOST_CHECK(metaclass->hasProperty("m_ulong_read"));
    BOOST_CHECK(metaclass->hasProperty("m_double_read"));
    BOOST_CHECK(metaclass->hasProperty("m_string_read"));
    BOOST_CHECK(metaclass->hasProperty("m_enum_read"));
    BOOST_CHECK(metaclass->hasProperty("m_bool"));
    BOOST_CHECK(metaclass->hasProperty("m_int"));
    BOOST_CHECK(metaclass->hasProperty("m_ulong"));
    BOOST_CHECK(metaclass->hasProperty("m_double"));
    BOOST_CHECK(metaclass->hasProperty("m_string"));
    BOOST_CHECK(metaclass->hasProperty("m_enum"));
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(type)
{
    BOOST_CHECK_EQUAL(metaclass->property("m_bool_read").type(),   camp::boolType);
    BOOST_CHECK_EQUAL(metaclass->property("m_int_read").type(),    camp::intType);
    BOOST_CHECK_EQUAL(metaclass->property("m_ulong_read").type(),  camp::intType);
    BOOST_CHECK_EQUAL(metaclass->property("m_double_read").type(), camp::realType);
    BOOST_CHECK_EQUAL(metaclass->property("m_string_read").type(), camp::stringType);
    BOOST_CHECK_EQUAL(metaclass->property("m_enum_read").type(),   camp::enumType);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(get)
{
    BOOST_CHECK_EQUAL(metaclass->property("m_bool_read").get(object).to<bool>(),           true);
    BOOST_CHECK_EQUAL(metaclass->property("m_int_read").get(object).to<int>(),             -10);
    BOOST_CHECK_EQUAL(metaclass->property("m_ulong_read").get(object).to<unsigned long>(), 20);
    BOOST_CHECK_CLOSE(metaclass->property("m_double_read").get(object).to<double>(),       0.55, 1E-5);
    BOOST_CHECK_EQUAL(metaclass->property("m_string_read").get(object).to<QString>(),      "hello");
    BOOST_CHECK_EQUAL(metaclass->property("m_enum_read").get(object).to<MyClass::Enum>(),  MyClass::two);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(set)
{
    metaclass->property("m_bool").set(object, false);
    metaclass->property("m_int").set(object, -2);
    metaclass->property("m_ulong").set(object, 50);
    metaclass->property("m_double").set(object, -8.8);
    metaclass->property("m_string").set(object, "bonjour");
    metaclass->property("m_enum").set(object, MyClass::three);

    BOOST_CHECK_EQUAL(metaclass->property("m_bool").get(object).to<bool>(),           false);
    BOOST_CHECK_EQUAL(metaclass->property("m_int").get(object).to<int>(),             -2);
    BOOST_CHECK_EQUAL(metaclass->property("m_ulong").get(object).to<unsigned long>(), 50);
    BOOST_CHECK_CLOSE(metaclass->property("m_double").get(object).to<double>(),       -8.8, 1E-5);
    BOOST_CHECK_EQUAL(metaclass->property("m_string").get(object).to<QString>(),      "bonjour");
    BOOST_CHECK_EQUAL(metaclass->property("m_enum").get(object).to<MyClass::Enum>(),  MyClass::three);
}

BOOST_AUTO_TEST_SUITE_END()
