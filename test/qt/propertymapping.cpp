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

#include "propertymapping.hpp"
#include <ponder/class.hpp>
#include <ponder/classget.hpp>
#include "catch.hpp"

using namespace PropertyMappingTest;

struct PropertyMappingFixture
{
    PropertyMappingFixture()
    {
        metaclass = &ponder::classByType<MyClass>();

        object.setBool(true);
        object.setInt(-10);
        object.setULong(20);
        object.setDouble(0.55);
        object.setString("hello");
        object.setEnum(MyClass::two);
    }

    MyClass object;
    const ponder::Class* metaclass;
};

//-----------------------------------------------------------------------------
//                         Tests for camp_ext::QtMapper (properties)
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(PROPERTYMAPPING, PropertyMappingFixture)

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

BOOST_AUTO_TEST_CASE(type)
{
    BOOST_CHECK_EQUAL(metaclass->property("m_bool_read").kind(),   ponder::boolType);
    BOOST_CHECK_EQUAL(metaclass->property("m_int_read").kind(),    ponder::intType);
    BOOST_CHECK_EQUAL(metaclass->property("m_ulong_read").kind(),  ponder::intType);
    BOOST_CHECK_EQUAL(metaclass->property("m_double_read").kind(), ponder::realType);
    BOOST_CHECK_EQUAL(metaclass->property("m_string_read").kind(), ponder::stringType);
    BOOST_CHECK_EQUAL(metaclass->property("m_enum_read").kind(),   ponder::enumType);
}

BOOST_AUTO_TEST_CASE(get)
{
    BOOST_CHECK_EQUAL(metaclass->property("m_bool_read").get(object).to<bool>(),           true);
    BOOST_CHECK_EQUAL(metaclass->property("m_int_read").get(object).to<int>(),             -10);
    BOOST_CHECK_EQUAL(metaclass->property("m_ulong_read").get(object).to<unsigned long>(), 20);
    BOOST_CHECK_CLOSE(metaclass->property("m_double_read").get(object).to<double>(),       0.55, 1E-5);
    BOOST_CHECK_EQUAL(metaclass->property("m_string_read").get(object).to<QString>(),      "hello");
    BOOST_CHECK_EQUAL(metaclass->property("m_enum_read").get(object).to<MyClass::Enum>(),  MyClass::two);
}

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
