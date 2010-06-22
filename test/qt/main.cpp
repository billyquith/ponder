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


#include "test.hpp"

#define BOOST_TEST_MODULE CAMP testqt
#include <boost/test/included/unit_test.hpp>


//////////////////////////////////////////////////////////////
/////               QT MAPPER - PROPERTIES               /////
//////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE(QtMapperProperties)
{
    // Declare the test metaenum
    camp::Enum::declare<PropertiesTest::Enum>("PropertiesTest::Enum")
        .value("one", PropertiesTest::one)
        .value("two", PropertiesTest::two)
        .value("three", PropertiesTest::three)
        ;

    // Declare the metaclass
    camp::Class::declare<PropertiesTest>("PropertiesTest")
        .external<camp_ext::QtMapper>()
        ;

    // Setup the test object
    PropertiesTest test;
    test.setBool(true);
    test.setInt(-10);
    test.setULong(20);
    test.setDouble(0.55);
    test.setString("qt");
    test.setEnum(PropertiesTest::two);

    const camp::Class& metaclass = camp::classByType<PropertiesTest>();
    camp::UserObject object = test;

    // ***** names *****
    BOOST_CHECK_EQUAL(metaclass.hasProperty("m_bool_read"), true);
    BOOST_CHECK_EQUAL(metaclass.hasProperty("m_int_read"), true);
    BOOST_CHECK_EQUAL(metaclass.hasProperty("m_ulong_read"), true);
    BOOST_CHECK_EQUAL(metaclass.hasProperty("m_double_read"), true);
    BOOST_CHECK_EQUAL(metaclass.hasProperty("m_string_read"), true);
    BOOST_CHECK_EQUAL(metaclass.hasProperty("m_enum_read"), true);
    BOOST_CHECK_EQUAL(metaclass.hasProperty("m_bool"), true);
    BOOST_CHECK_EQUAL(metaclass.hasProperty("m_int"), true);
    BOOST_CHECK_EQUAL(metaclass.hasProperty("m_ulong"), true);
    BOOST_CHECK_EQUAL(metaclass.hasProperty("m_double"), true);
    BOOST_CHECK_EQUAL(metaclass.hasProperty("m_string"), true);
    BOOST_CHECK_EQUAL(metaclass.hasProperty("m_enum"), true);

    // ***** types *****
    BOOST_CHECK_EQUAL(metaclass.property("m_bool_read").type(), camp::boolType);
    BOOST_CHECK_EQUAL(metaclass.property("m_int_read").type(), camp::intType);
    BOOST_CHECK_EQUAL(metaclass.property("m_ulong_read").type(), camp::intType);
    BOOST_CHECK_EQUAL(metaclass.property("m_double_read").type(), camp::realType);
    BOOST_CHECK_EQUAL(metaclass.property("m_string_read").type(), camp::stringType);
    BOOST_CHECK_EQUAL(metaclass.property("m_enum_read").type(), camp::enumType);

    // ***** get *****
    BOOST_CHECK_EQUAL(metaclass.property("m_bool_read").get(object).to<bool>(), true);
    BOOST_CHECK_EQUAL(metaclass.property("m_int_read").get(object).to<int>(), -10);
    BOOST_CHECK_EQUAL(metaclass.property("m_ulong_read").get(object).to<unsigned long>(), 20);
    BOOST_CHECK_CLOSE(metaclass.property("m_double_read").get(object).to<double>(), 0.55, 1E-5);
    BOOST_CHECK_EQUAL(metaclass.property("m_string_read").get(object).to<std::string>(), "qt");
    BOOST_CHECK_EQUAL(metaclass.property("m_enum_read").get(object).to<PropertiesTest::Enum>(), PropertiesTest::two);

    // ***** set *****
    metaclass.property("m_bool").set(object, false);
    metaclass.property("m_int").set(object, -2);
    metaclass.property("m_ulong").set(object, 50);
    metaclass.property("m_double").set(object, -8.88);
    metaclass.property("m_string").set(object, "camp");
    metaclass.property("m_enum").set(object, PropertiesTest::three);

    // ***** get *****
    BOOST_CHECK_EQUAL(metaclass.property("m_bool").get(object).to<bool>(), false);
    BOOST_CHECK_EQUAL(metaclass.property("m_int").get(object).to<int>(), -2);
    BOOST_CHECK_EQUAL(metaclass.property("m_ulong").get(object).to<unsigned long>(), 50);
    BOOST_CHECK_CLOSE(metaclass.property("m_double").get(object).to<double>(), -8.88, 1E-5);
    BOOST_CHECK_EQUAL(metaclass.property("m_string").get(object).to<std::string>(), "camp");
    BOOST_CHECK_EQUAL(metaclass.property("m_enum").get(object).to<PropertiesTest::Enum>(), PropertiesTest::three);
}


//////////////////////////////////////////////////////////////
/////               QT MAPPER - FUNCTIONS                /////
//////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE(QtMapperFunctions)
{
    // Declare the test metaclass
    camp::Class::declare<Test>("Test")
        .property("x", &Test::x)
        ;

    // Declare the metaclass
    camp::Class::declare<FunctionsTest>("FunctionsTest")
        .external<camp_ext::QtMapper>()
        ;

    FunctionsTest test;
    const camp::Class& metaclass = camp::classByType<FunctionsTest>();
    camp::UserObject object = test;

    // ***** names *****
    BOOST_CHECK_EQUAL(metaclass.hasFunction("f1"), true);
    BOOST_CHECK_EQUAL(metaclass.hasFunction("f2"), true);
    BOOST_CHECK_EQUAL(metaclass.hasFunction("f3"), true);
    BOOST_CHECK_EQUAL(metaclass.hasFunction("f4"), true);
    BOOST_CHECK_EQUAL(metaclass.hasFunction("f5"), true);
    BOOST_CHECK_EQUAL(metaclass.hasFunction("f6"), true);

    // ***** return types *****
    BOOST_CHECK_EQUAL(metaclass.function("f1").returnType(), camp::noType);
    BOOST_CHECK_EQUAL(metaclass.function("f2").returnType(), camp::boolType);
    BOOST_CHECK_EQUAL(metaclass.function("f3").returnType(), camp::intType);
    BOOST_CHECK_EQUAL(metaclass.function("f4").returnType(), camp::intType);
    BOOST_CHECK_EQUAL(metaclass.function("f5").returnType(), camp::realType);
    BOOST_CHECK_EQUAL(metaclass.function("f6").returnType(), camp::stringType);

    // ***** parameters count *****
    BOOST_CHECK_EQUAL(metaclass.function("f1").argCount(), 0);
    BOOST_CHECK_EQUAL(metaclass.function("f2").argCount(), 1);
    BOOST_CHECK_EQUAL(metaclass.function("f3").argCount(), 2);
    BOOST_CHECK_EQUAL(metaclass.function("f4").argCount(), 3);
    BOOST_CHECK_EQUAL(metaclass.function("f5").argCount(), 4);
    BOOST_CHECK_EQUAL(metaclass.function("f6").argCount(), 5);

    // ***** return values and parameters *****
    BOOST_CHECK_EQUAL(metaclass.function("f1").call(object), camp::Value::nothing);
    BOOST_CHECK_EQUAL(metaclass.function("f2").call(object, camp::Args(true)).to<bool>(), true);
    BOOST_CHECK_EQUAL(metaclass.function("f3").call(object, camp::Args(-1, -9)).to<int>(), -10);
    BOOST_CHECK_EQUAL(metaclass.function("f4").call(object, camp::Args(1, 4, 15)).to<unsigned long>(), 20);
    BOOST_CHECK_CLOSE(metaclass.function("f5").call(object, camp::Args(1., 2., 3., .5)).to<double>(), 6.5, 1E-5);
    BOOST_CHECK_EQUAL(metaclass.function("f6").call(object, camp::Args("h", "e", "l", "l", "o")).to<QString>(), "hello");
}


//////////////////////////////////////////////////////////////
/////                       QSTRING                      /////
//////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE(QStringValueMapper)
{
    // ***** QString to std::string *****
    camp::Value str1 = QString("hello");
    BOOST_CHECK_EQUAL(str1.type(), camp::stringType);
    BOOST_CHECK_EQUAL(str1.to<std::string>(), "hello");
    BOOST_CHECK_EQUAL(str1.to<QString>(), "hello");

    // ***** std::string to QString *****
    camp::Value str2 = std::string("hello");
    BOOST_CHECK_EQUAL(str2.type(), camp::stringType);
    BOOST_CHECK_EQUAL(str2.to<std::string>(), "hello");
    BOOST_CHECK_EQUAL(str2.to<QString>(), "hello");
}
