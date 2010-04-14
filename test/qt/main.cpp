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

#define BOOST_TEST_NO_LIB
#define BOOST_TEST_MODULE CAMP testqt
#include <boost/test/unit_test.hpp>


//////////////////////////////////////////////////////////////
/////                      QT MAPPER                     /////
//////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE(campQtMapper)
{
    // Declare the metaclass
    camp::Class::declare<MapperTest>("MapperTest")
        .external<camp_ext::QtMapper>()
        ;

    // Setup the test object
    MapperTest test;
    test.setBool(true);
    test.setInt(-10);
    test.setUInt(20);
    test.setDouble(0.55);
    test.setString("qt");
    test.setStringList(QStringList() << "aaa" << "bbb" << "ccc");

    const camp::Class& metaclass = camp::classByType<MapperTest>();
    //const camp::ArrayProperty& arrayProperty = static_cast<const camp::ArrayProperty&>(metaclass.property("m_stringlist"));
    camp::UserObject object = test;

    // ***** names *****
    BOOST_CHECK_EQUAL(metaclass.hasProperty("m_bool_read"), true);
    BOOST_CHECK_EQUAL(metaclass.hasProperty("m_int_read"), true);
    BOOST_CHECK_EQUAL(metaclass.hasProperty("m_uint_read"), true);
    BOOST_CHECK_EQUAL(metaclass.hasProperty("m_double_read"), true);
    BOOST_CHECK_EQUAL(metaclass.hasProperty("m_string_read"), true);
    //BOOST_CHECK_EQUAL(metaclass.hasProperty("m_stringlist_read"), true);
    BOOST_CHECK_EQUAL(metaclass.hasProperty("m_bool"), true);
    BOOST_CHECK_EQUAL(metaclass.hasProperty("m_int"), true);
    BOOST_CHECK_EQUAL(metaclass.hasProperty("m_uint"), true);
    BOOST_CHECK_EQUAL(metaclass.hasProperty("m_double"), true);
    BOOST_CHECK_EQUAL(metaclass.hasProperty("m_string"), true);
    //BOOST_CHECK_EQUAL(metaclass.hasProperty("m_stringlist"), true);

    // ***** types *****
    BOOST_CHECK_EQUAL(metaclass.property("m_bool_read").type(), camp::boolType);
    BOOST_CHECK_EQUAL(metaclass.property("m_int_read").type(), camp::intType);
    BOOST_CHECK_EQUAL(metaclass.property("m_uint_read").type(), camp::intType);
    BOOST_CHECK_EQUAL(metaclass.property("m_double_read").type(), camp::realType);
    BOOST_CHECK_EQUAL(metaclass.property("m_string_read").type(), camp::stringType);
    //BOOST_CHECK_EQUAL(metaclass.property("m_stringlist_read").type(), camp::arrayType);

    // ***** get *****
    BOOST_CHECK_EQUAL(metaclass.property("m_bool_read").get(object).to<bool>(), true);
    BOOST_CHECK_EQUAL(metaclass.property("m_int_read").get(object).to<int>(), -10);
    BOOST_CHECK_EQUAL(metaclass.property("m_uint_read").get(object).to<unsigned int>(), 20);
    BOOST_CHECK_CLOSE(metaclass.property("m_double_read").get(object).to<double>(), 0.55, 1E-5);
    BOOST_CHECK_EQUAL(metaclass.property("m_string_read").get(object).to<std::string>(), "qt");
    //BOOST_CHECK_EQUAL(arrayProperty.get(object, 1).to<std::string>(), "bbb");

    // ***** set *****
    metaclass.property("m_bool").set(object, false);
    metaclass.property("m_int").set(object, -2);
    metaclass.property("m_uint").set(object, 50);
    metaclass.property("m_double").set(object, -8.88);
    metaclass.property("m_string").set(object, "camp");
    //BOOST_CHECK_EQUAL(arrayProperty.set(object, 2, "zzz");

    // ***** get *****
    BOOST_CHECK_EQUAL(metaclass.property("m_bool").get(object).to<bool>(), false);
    BOOST_CHECK_EQUAL(metaclass.property("m_int").get(object).to<int>(), -2);
    BOOST_CHECK_EQUAL(metaclass.property("m_uint").get(object).to<unsigned int>(), 50);
    BOOST_CHECK_CLOSE(metaclass.property("m_double").get(object).to<double>(), -8.88, 1E-5);
    BOOST_CHECK_EQUAL(metaclass.property("m_string").get(object).to<std::string>(), "camp");
    //BOOST_CHECK_EQUAL(arrayProperty.get(object, 2).to<std::string>(), "zzz");
}


//////////////////////////////////////////////////////////////
/////                       QSTRING                      /////
//////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE(campQString)
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
