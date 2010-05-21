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


#define BOOST_TEST_NO_LIB
#include <boost/test/unit_test.hpp>

#include "value.hpp"
#include <camp/class.hpp>
#include <camp/enum.hpp>
#include <camp/value.hpp>
#include <camp/errors.hpp>

using namespace ValueTest;

//-----------------------------------------------------------------------------
struct ValueFixture
{
    ValueFixture() : object1(1), object2(1)
    {
        camp::Enum::declare<MyEnum>("MyEnum")
            .value("One", One)
            .value("Two", Two);
        camp::Class::declare<MyClass>("MyClass");

        noValue      = camp::Value::nothing;
        boolValue    = true;
        charValue    = static_cast<char>(1);
        shortValue   = static_cast<short>(1);
        intValue     = static_cast<int>(1);
        longValue    = static_cast<long>(1);
        ucharValue   = static_cast<unsigned char>(1);
        ushortValue  = static_cast<unsigned short>(1);
        uintValue    = static_cast<unsigned int>(1);
        ulongValue   = static_cast<unsigned long>(1);
        floatValue   = 1.f;
        doubleValue  = 1.;
        cstringValue = "1";
        stringValue  = std::string("1");
        enumValue    = One;
        objectValue  = object1;
    }

    ~ValueFixture()
    {
        camp::Class::undeclare<MyClass>();
        camp::Enum::undeclare<MyEnum>();
    }

    static bool equivalent(const camp::Value& left, const camp::Value& right)
    {
        return !(left < right) && !(right < left);
    }

    camp::Value noValue;
    camp::Value boolValue;
    camp::Value charValue;
    camp::Value shortValue;
    camp::Value intValue;
    camp::Value longValue;
    camp::Value ucharValue;
    camp::Value ushortValue;
    camp::Value uintValue;
    camp::Value ulongValue;
    camp::Value floatValue;
    camp::Value doubleValue;
    camp::Value cstringValue;
    camp::Value stringValue;
    camp::Value enumValue;
    camp::Value objectValue;
    MyClass object1;
    MyClass object2;
};

//-----------------------------------------------------------------------------
//                         Tests for camp::Value
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(VALUE, ValueFixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(type)
{
    BOOST_CHECK_EQUAL(noValue.type(),      camp::noType);
    BOOST_CHECK_EQUAL(boolValue.type(),    camp::boolType);
    BOOST_CHECK_EQUAL(charValue.type(),    camp::intType);
    BOOST_CHECK_EQUAL(shortValue.type(),   camp::intType);
    BOOST_CHECK_EQUAL(intValue.type(),     camp::intType);
    BOOST_CHECK_EQUAL(longValue.type(),    camp::intType);
    BOOST_CHECK_EQUAL(ucharValue.type(),   camp::intType);
    BOOST_CHECK_EQUAL(ushortValue.type(),  camp::intType);
    BOOST_CHECK_EQUAL(uintValue.type(),    camp::intType);
    BOOST_CHECK_EQUAL(ulongValue.type(),   camp::intType);
    BOOST_CHECK_EQUAL(floatValue.type(),   camp::realType);
    BOOST_CHECK_EQUAL(doubleValue.type(),  camp::realType);
    BOOST_CHECK_EQUAL(cstringValue.type(), camp::stringType);
    BOOST_CHECK_EQUAL(stringValue.type(),  camp::stringType);
    BOOST_CHECK_EQUAL(enumValue.type(),    camp::enumType);
    BOOST_CHECK_EQUAL(objectValue.type(),  camp::userType);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(boolConversions)
{
    BOOST_CHECK_EQUAL(boolValue.to<bool>(),           true);
    BOOST_CHECK_EQUAL(boolValue.to<char>(),           1);
    BOOST_CHECK_EQUAL(boolValue.to<short>(),          1);
    BOOST_CHECK_EQUAL(boolValue.to<int>(),            1);
    BOOST_CHECK_EQUAL(boolValue.to<long>(),           1);
    BOOST_CHECK_EQUAL(boolValue.to<unsigned char>(),  1);
    BOOST_CHECK_EQUAL(boolValue.to<unsigned short>(), 1);
    BOOST_CHECK_EQUAL(boolValue.to<unsigned int>(),   1);
    BOOST_CHECK_EQUAL(boolValue.to<unsigned long>(),  1);
    BOOST_CHECK_CLOSE(boolValue.to<float>(),          1.f, 1E-5f);
    BOOST_CHECK_CLOSE(boolValue.to<double>(),         1., 1E-5);
    BOOST_CHECK_EQUAL(boolValue.to<std::string>(),    "1");
    BOOST_CHECK_EQUAL(boolValue.to<MyEnum>(),         One);
    BOOST_CHECK_THROW(boolValue.to<MyClass>(),        camp::BadType);

    BOOST_CHECK_EQUAL(boolValue.isCompatible<bool>(),           true);
    BOOST_CHECK_EQUAL(boolValue.isCompatible<char>(),           true);
    BOOST_CHECK_EQUAL(boolValue.isCompatible<short>(),          true);
    BOOST_CHECK_EQUAL(boolValue.isCompatible<int>(),            true);
    BOOST_CHECK_EQUAL(boolValue.isCompatible<long>(),           true);
    BOOST_CHECK_EQUAL(boolValue.isCompatible<unsigned char>(),  true);
    BOOST_CHECK_EQUAL(boolValue.isCompatible<unsigned short>(), true);
    BOOST_CHECK_EQUAL(boolValue.isCompatible<unsigned int>(),   true);
    BOOST_CHECK_EQUAL(boolValue.isCompatible<unsigned long>(),  true);
    BOOST_CHECK_EQUAL(boolValue.isCompatible<float>(),          true);
    BOOST_CHECK_EQUAL(boolValue.isCompatible<double>(),         true);
    BOOST_CHECK_EQUAL(boolValue.isCompatible<std::string>(),    true);
    BOOST_CHECK_EQUAL(boolValue.isCompatible<MyEnum>(),         true);
    BOOST_CHECK_EQUAL(boolValue.isCompatible<MyClass>(),        false);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(intConversions)
{
    BOOST_CHECK_EQUAL(intValue.to<bool>(),           true);
    BOOST_CHECK_EQUAL(intValue.to<char>(),           1);
    BOOST_CHECK_EQUAL(intValue.to<short>(),          1);
    BOOST_CHECK_EQUAL(intValue.to<int>(),            1);
    BOOST_CHECK_EQUAL(intValue.to<long>(),           1);
    BOOST_CHECK_EQUAL(intValue.to<unsigned char>(),  1);
    BOOST_CHECK_EQUAL(intValue.to<unsigned short>(), 1);
    BOOST_CHECK_EQUAL(intValue.to<unsigned int>(),   1);
    BOOST_CHECK_EQUAL(intValue.to<unsigned long>(),  1);
    BOOST_CHECK_CLOSE(intValue.to<float>(),          1.f, 1E-5f);
    BOOST_CHECK_CLOSE(intValue.to<double>(),         1., 1E-5);
    BOOST_CHECK_EQUAL(intValue.to<std::string>(),    "1");
    BOOST_CHECK_EQUAL(intValue.to<MyEnum>(),         One);
    BOOST_CHECK_THROW(intValue.to<MyClass>(),        camp::BadType);

    BOOST_CHECK_EQUAL(intValue.isCompatible<bool>(),           true);
    BOOST_CHECK_EQUAL(intValue.isCompatible<char>(),           true);
    BOOST_CHECK_EQUAL(intValue.isCompatible<short>(),          true);
    BOOST_CHECK_EQUAL(intValue.isCompatible<int>(),            true);
    BOOST_CHECK_EQUAL(intValue.isCompatible<long>(),           true);
    BOOST_CHECK_EQUAL(intValue.isCompatible<unsigned char>(),  true);
    BOOST_CHECK_EQUAL(intValue.isCompatible<unsigned short>(), true);
    BOOST_CHECK_EQUAL(intValue.isCompatible<unsigned int>(),   true);
    BOOST_CHECK_EQUAL(intValue.isCompatible<unsigned long>(),  true);
    BOOST_CHECK_EQUAL(intValue.isCompatible<float>(),          true);
    BOOST_CHECK_EQUAL(intValue.isCompatible<double>(),         true);
    BOOST_CHECK_EQUAL(intValue.isCompatible<std::string>(),    true);
    BOOST_CHECK_EQUAL(intValue.isCompatible<MyEnum>(),         true);
    BOOST_CHECK_EQUAL(intValue.isCompatible<MyClass>(),        false);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(doubleConversions)
{
    BOOST_CHECK_EQUAL(doubleValue.to<bool>(),           true);
    BOOST_CHECK_EQUAL(doubleValue.to<char>(),           1);
    BOOST_CHECK_EQUAL(doubleValue.to<short>(),          1);
    BOOST_CHECK_EQUAL(doubleValue.to<int>(),            1);
    BOOST_CHECK_EQUAL(doubleValue.to<long>(),           1);
    BOOST_CHECK_EQUAL(doubleValue.to<unsigned char>(),  1);
    BOOST_CHECK_EQUAL(doubleValue.to<unsigned short>(), 1);
    BOOST_CHECK_EQUAL(doubleValue.to<unsigned int>(),   1);
    BOOST_CHECK_EQUAL(doubleValue.to<unsigned long>(),  1);
    BOOST_CHECK_CLOSE(doubleValue.to<float>(),          1.f, 1E-5f);
    BOOST_CHECK_CLOSE(doubleValue.to<double>(),         1., 1E-5);
    BOOST_CHECK_EQUAL(doubleValue.to<std::string>(),    "1");
    BOOST_CHECK_EQUAL(doubleValue.to<MyEnum>(),         One);
    BOOST_CHECK_THROW(doubleValue.to<MyClass>(),        camp::BadType);

    BOOST_CHECK_EQUAL(doubleValue.isCompatible<bool>(),           true);
    BOOST_CHECK_EQUAL(doubleValue.isCompatible<char>(),           true);
    BOOST_CHECK_EQUAL(doubleValue.isCompatible<short>(),          true);
    BOOST_CHECK_EQUAL(doubleValue.isCompatible<int>(),            true);
    BOOST_CHECK_EQUAL(doubleValue.isCompatible<long>(),           true);
    BOOST_CHECK_EQUAL(doubleValue.isCompatible<unsigned char>(),  true);
    BOOST_CHECK_EQUAL(doubleValue.isCompatible<unsigned short>(), true);
    BOOST_CHECK_EQUAL(doubleValue.isCompatible<unsigned int>(),   true);
    BOOST_CHECK_EQUAL(doubleValue.isCompatible<unsigned long>(),  true);
    BOOST_CHECK_EQUAL(doubleValue.isCompatible<float>(),          true);
    BOOST_CHECK_EQUAL(doubleValue.isCompatible<double>(),         true);
    BOOST_CHECK_EQUAL(doubleValue.isCompatible<std::string>(),    true);
    BOOST_CHECK_EQUAL(doubleValue.isCompatible<MyEnum>(),         true);
    BOOST_CHECK_EQUAL(doubleValue.isCompatible<MyClass>(),        false);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(stringConversionsOk)
{
    BOOST_CHECK_EQUAL(stringValue.to<bool>(),           true);
    BOOST_CHECK_EQUAL(stringValue.to<char>(),           '1');
    BOOST_CHECK_EQUAL(stringValue.to<short>(),          1);
    BOOST_CHECK_EQUAL(stringValue.to<int>(),            1);
    BOOST_CHECK_EQUAL(stringValue.to<long>(),           1);
    BOOST_CHECK_EQUAL(stringValue.to<unsigned char>(),  '1');
    BOOST_CHECK_EQUAL(stringValue.to<unsigned short>(), 1);
    BOOST_CHECK_EQUAL(stringValue.to<unsigned int>(),   1);
    BOOST_CHECK_EQUAL(stringValue.to<unsigned long>(),  1);
    BOOST_CHECK_CLOSE(stringValue.to<float>(),          1.f, 1E-5f);
    BOOST_CHECK_CLOSE(stringValue.to<double>(),         1., 1E-5);
    BOOST_CHECK_EQUAL(stringValue.to<std::string>(),    "1");
    BOOST_CHECK_EQUAL(stringValue.to<MyEnum>(),         One);
    BOOST_CHECK_THROW(stringValue.to<MyClass>(),        camp::BadType);

    BOOST_CHECK_EQUAL(stringValue.isCompatible<bool>(),           true);
    BOOST_CHECK_EQUAL(stringValue.isCompatible<char>(),           true);
    BOOST_CHECK_EQUAL(stringValue.isCompatible<short>(),          true);
    BOOST_CHECK_EQUAL(stringValue.isCompatible<int>(),            true);
    BOOST_CHECK_EQUAL(stringValue.isCompatible<long>(),           true);
    BOOST_CHECK_EQUAL(stringValue.isCompatible<unsigned char>(),  true);
    BOOST_CHECK_EQUAL(stringValue.isCompatible<unsigned short>(), true);
    BOOST_CHECK_EQUAL(stringValue.isCompatible<unsigned int>(),   true);
    BOOST_CHECK_EQUAL(stringValue.isCompatible<unsigned long>(),  true);
    BOOST_CHECK_EQUAL(stringValue.isCompatible<float>(),          true);
    BOOST_CHECK_EQUAL(stringValue.isCompatible<double>(),         true);
    BOOST_CHECK_EQUAL(stringValue.isCompatible<std::string>(),    true);
    BOOST_CHECK_EQUAL(stringValue.isCompatible<MyEnum>(),         true);
    BOOST_CHECK_EQUAL(stringValue.isCompatible<MyClass>(),        false);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(stringConversionsEnumOk)
{
    BOOST_CHECK_EQUAL(camp::Value("One").to<MyEnum>(), One);
    BOOST_CHECK_EQUAL(camp::Value("Two").to<MyEnum>(), Two);
    BOOST_CHECK_EQUAL(camp::Value("1").to<MyEnum>(),   One);
    BOOST_CHECK_EQUAL(camp::Value("2").to<MyEnum>(),   Two);

    BOOST_CHECK_EQUAL(camp::Value("One").isCompatible<MyEnum>(), true);
    BOOST_CHECK_EQUAL(camp::Value("Two").isCompatible<MyEnum>(), true);
    BOOST_CHECK_EQUAL(camp::Value("1").isCompatible<MyEnum>(),   true);
    BOOST_CHECK_EQUAL(camp::Value("2").isCompatible<MyEnum>(),   true);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(stringConversionsNotOk)
{
    camp::Value badStringValue = "not a number";

    BOOST_CHECK_THROW(badStringValue.to<bool>(),           camp::BadType);
    BOOST_CHECK_THROW(badStringValue.to<char>(),           camp::BadType);
    BOOST_CHECK_THROW(badStringValue.to<short>(),          camp::BadType);
    BOOST_CHECK_THROW(badStringValue.to<int>(),            camp::BadType);
    BOOST_CHECK_THROW(badStringValue.to<long>(),           camp::BadType);
    BOOST_CHECK_THROW(badStringValue.to<unsigned char>(),  camp::BadType);
    BOOST_CHECK_THROW(badStringValue.to<unsigned short>(), camp::BadType);
    BOOST_CHECK_THROW(badStringValue.to<unsigned int>(),   camp::BadType);
    BOOST_CHECK_THROW(badStringValue.to<unsigned long>(),  camp::BadType);
    BOOST_CHECK_THROW(badStringValue.to<float>(),          camp::BadType);
    BOOST_CHECK_THROW(badStringValue.to<double>(),         camp::BadType);
    BOOST_CHECK_THROW(badStringValue.to<MyEnum>(),         camp::BadType);
    BOOST_CHECK_THROW(badStringValue.to<MyClass>(),        camp::BadType);

    BOOST_CHECK_EQUAL(badStringValue.isCompatible<bool>(),           false);
    BOOST_CHECK_EQUAL(badStringValue.isCompatible<char>(),           false);
    BOOST_CHECK_EQUAL(badStringValue.isCompatible<short>(),          false);
    BOOST_CHECK_EQUAL(badStringValue.isCompatible<int>(),            false);
    BOOST_CHECK_EQUAL(badStringValue.isCompatible<long>(),           false);
    BOOST_CHECK_EQUAL(badStringValue.isCompatible<unsigned char>(),  false);
    BOOST_CHECK_EQUAL(badStringValue.isCompatible<unsigned short>(), false);
    BOOST_CHECK_EQUAL(badStringValue.isCompatible<unsigned int>(),   false);
    BOOST_CHECK_EQUAL(badStringValue.isCompatible<unsigned long>(),  false);
    BOOST_CHECK_EQUAL(badStringValue.isCompatible<float>(),          false);
    BOOST_CHECK_EQUAL(badStringValue.isCompatible<double>(),         false);
    BOOST_CHECK_EQUAL(badStringValue.isCompatible<std::string>(),    true);
    BOOST_CHECK_EQUAL(badStringValue.isCompatible<MyEnum>(),         false);
    BOOST_CHECK_EQUAL(badStringValue.isCompatible<MyClass>(),        false);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(enumConversions)
{
    BOOST_CHECK_EQUAL(enumValue.to<bool>(),           true);
    BOOST_CHECK_EQUAL(enumValue.to<char>(),           1);
    BOOST_CHECK_EQUAL(enumValue.to<short>(),          1);
    BOOST_CHECK_EQUAL(enumValue.to<int>(),            1);
    BOOST_CHECK_EQUAL(enumValue.to<long>(),           1);
    BOOST_CHECK_EQUAL(enumValue.to<unsigned char>(),  1);
    BOOST_CHECK_EQUAL(enumValue.to<unsigned short>(), 1);
    BOOST_CHECK_EQUAL(enumValue.to<unsigned int>(),   1);
    BOOST_CHECK_EQUAL(enumValue.to<unsigned long>(),  1);
    BOOST_CHECK_CLOSE(enumValue.to<float>(),          1.f, 1E-5f);
    BOOST_CHECK_CLOSE(enumValue.to<double>(),         1., 1E-5);
    BOOST_CHECK_EQUAL(enumValue.to<std::string>(),    "One");
    BOOST_CHECK_EQUAL(enumValue.to<MyEnum>(),         One);
    BOOST_CHECK_THROW(enumValue.to<MyClass>(),        camp::BadType);

    BOOST_CHECK_EQUAL(enumValue.isCompatible<bool>(),           true);
    BOOST_CHECK_EQUAL(enumValue.isCompatible<char>(),           true);
    BOOST_CHECK_EQUAL(enumValue.isCompatible<short>(),          true);
    BOOST_CHECK_EQUAL(enumValue.isCompatible<int>(),            true);
    BOOST_CHECK_EQUAL(enumValue.isCompatible<long>(),           true);
    BOOST_CHECK_EQUAL(enumValue.isCompatible<unsigned char>(),  true);
    BOOST_CHECK_EQUAL(enumValue.isCompatible<unsigned short>(), true);
    BOOST_CHECK_EQUAL(enumValue.isCompatible<unsigned int>(),   true);
    BOOST_CHECK_EQUAL(enumValue.isCompatible<unsigned long>(),  true);
    BOOST_CHECK_EQUAL(enumValue.isCompatible<float>(),          true);
    BOOST_CHECK_EQUAL(enumValue.isCompatible<double>(),         true);
    BOOST_CHECK_EQUAL(enumValue.isCompatible<std::string>(),    true);
    BOOST_CHECK_EQUAL(enumValue.isCompatible<MyEnum>(),         true);
    BOOST_CHECK_EQUAL(enumValue.isCompatible<MyClass>(),        false);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(objectConversions)
{
    BOOST_CHECK_EQUAL(objectValue.to<bool>(),             true);
    BOOST_CHECK_THROW(objectValue.to<char>(),             camp::BadType);
    BOOST_CHECK_THROW(objectValue.to<short>(),            camp::BadType);
    BOOST_CHECK_THROW(objectValue.to<int>(),              camp::BadType);
    BOOST_CHECK_THROW(objectValue.to<long>(),             camp::BadType);
    BOOST_CHECK_THROW(objectValue.to<unsigned char>(),    camp::BadType);
    BOOST_CHECK_THROW(objectValue.to<unsigned short>(),   camp::BadType);
    BOOST_CHECK_THROW(objectValue.to<unsigned int>(),     camp::BadType);
    BOOST_CHECK_THROW(objectValue.to<unsigned long>(),    camp::BadType);
    BOOST_CHECK_THROW(objectValue.to<float>(),            camp::BadType);
    BOOST_CHECK_THROW(objectValue.to<double>(),           camp::BadType);
    BOOST_CHECK_THROW(objectValue.to<std::string>(),      camp::BadType);
    BOOST_CHECK_THROW(objectValue.to<MyEnum>(),           camp::BadType);
    BOOST_CHECK_EQUAL(objectValue.to<MyClass>(),          object1);
    BOOST_CHECK_EQUAL(objectValue.to<camp::UserObject>(), camp::UserObject(object1));

    BOOST_CHECK_EQUAL(objectValue.isCompatible<bool>(),           true);
    BOOST_CHECK_EQUAL(objectValue.isCompatible<char>(),           false);
    BOOST_CHECK_EQUAL(objectValue.isCompatible<short>(),          false);
    BOOST_CHECK_EQUAL(objectValue.isCompatible<int>(),            false);
    BOOST_CHECK_EQUAL(objectValue.isCompatible<long>(),           false);
    BOOST_CHECK_EQUAL(objectValue.isCompatible<unsigned char>(),  false);
    BOOST_CHECK_EQUAL(objectValue.isCompatible<unsigned short>(), false);
    BOOST_CHECK_EQUAL(objectValue.isCompatible<unsigned int>(),   false);
    BOOST_CHECK_EQUAL(objectValue.isCompatible<unsigned long>(),  false);
    BOOST_CHECK_EQUAL(objectValue.isCompatible<float>(),          false);
    BOOST_CHECK_EQUAL(objectValue.isCompatible<double>(),         false);
    BOOST_CHECK_EQUAL(objectValue.isCompatible<std::string>(),    false);
    BOOST_CHECK_EQUAL(objectValue.isCompatible<MyEnum>(),         false);
    BOOST_CHECK_EQUAL(objectValue.isCompatible<MyClass>(),        true);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(visitor)
{
    Visitor visitor;

    BOOST_CHECK_EQUAL(noValue.visit(visitor),      camp::noType);
    BOOST_CHECK_EQUAL(boolValue.visit(visitor),    camp::boolType);
    BOOST_CHECK_EQUAL(charValue.visit(visitor),    camp::intType);
    BOOST_CHECK_EQUAL(shortValue.visit(visitor),   camp::intType);
    BOOST_CHECK_EQUAL(intValue.visit(visitor),     camp::intType);
    BOOST_CHECK_EQUAL(longValue.visit(visitor),    camp::intType);
    BOOST_CHECK_EQUAL(ucharValue.visit(visitor),   camp::intType);
    BOOST_CHECK_EQUAL(ushortValue.visit(visitor),  camp::intType);
    BOOST_CHECK_EQUAL(uintValue.visit(visitor),    camp::intType);
    BOOST_CHECK_EQUAL(ulongValue.visit(visitor),   camp::intType);
    BOOST_CHECK_EQUAL(floatValue.visit(visitor),   camp::realType);
    BOOST_CHECK_EQUAL(doubleValue.visit(visitor),  camp::realType);
    BOOST_CHECK_EQUAL(cstringValue.visit(visitor), camp::stringType);
    BOOST_CHECK_EQUAL(stringValue.visit(visitor),  camp::stringType);
    BOOST_CHECK_EQUAL(enumValue.visit(visitor),    camp::enumType);
    BOOST_CHECK_EQUAL(objectValue.visit(visitor),  camp::userType);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(equal)
{
    BOOST_CHECK_EQUAL(noValue == noValue,     true);
    BOOST_CHECK_EQUAL(noValue == boolValue,   false);
    BOOST_CHECK_EQUAL(noValue == intValue,    false);
    BOOST_CHECK_EQUAL(noValue == doubleValue, false);
    BOOST_CHECK_EQUAL(noValue == stringValue, false);
    BOOST_CHECK_EQUAL(noValue == enumValue,   false);
    BOOST_CHECK_EQUAL(noValue == objectValue, false);

    BOOST_CHECK_EQUAL(boolValue == noValue,     false);
    BOOST_CHECK_EQUAL(boolValue == boolValue,   true);
    BOOST_CHECK_EQUAL(boolValue == intValue,    false);
    BOOST_CHECK_EQUAL(boolValue == doubleValue, false);
    BOOST_CHECK_EQUAL(boolValue == stringValue, false);
    BOOST_CHECK_EQUAL(boolValue == enumValue,   false);
    BOOST_CHECK_EQUAL(boolValue == objectValue, false);

    BOOST_CHECK_EQUAL(intValue == noValue,     false);
    BOOST_CHECK_EQUAL(intValue == boolValue,   false);
    BOOST_CHECK_EQUAL(intValue == intValue,    true);
    BOOST_CHECK_EQUAL(intValue == doubleValue, false);
    BOOST_CHECK_EQUAL(intValue == stringValue, false);
    BOOST_CHECK_EQUAL(intValue == enumValue,   false);
    BOOST_CHECK_EQUAL(intValue == objectValue, false);

    BOOST_CHECK_EQUAL(doubleValue == noValue,     false);
    BOOST_CHECK_EQUAL(doubleValue == boolValue,   false);
    BOOST_CHECK_EQUAL(doubleValue == intValue,    false);
    BOOST_CHECK_EQUAL(doubleValue == doubleValue, true);
    BOOST_CHECK_EQUAL(doubleValue == stringValue, false);
    BOOST_CHECK_EQUAL(doubleValue == enumValue,   false);
    BOOST_CHECK_EQUAL(doubleValue == objectValue, false);

    BOOST_CHECK_EQUAL(stringValue == noValue,     false);
    BOOST_CHECK_EQUAL(stringValue == boolValue,   false);
    BOOST_CHECK_EQUAL(stringValue == intValue,    false);
    BOOST_CHECK_EQUAL(stringValue == doubleValue, false);
    BOOST_CHECK_EQUAL(stringValue == stringValue, true);
    BOOST_CHECK_EQUAL(stringValue == enumValue,   false);
    BOOST_CHECK_EQUAL(stringValue == objectValue, false);

    BOOST_CHECK_EQUAL(enumValue == noValue,     false);
    BOOST_CHECK_EQUAL(enumValue == boolValue,   false);
    BOOST_CHECK_EQUAL(enumValue == intValue,    false);
    BOOST_CHECK_EQUAL(enumValue == doubleValue, false);
    BOOST_CHECK_EQUAL(enumValue == stringValue, false);
    BOOST_CHECK_EQUAL(enumValue == enumValue,   true);
    BOOST_CHECK_EQUAL(enumValue == objectValue, false);

    BOOST_CHECK_EQUAL(objectValue == noValue,     false);
    BOOST_CHECK_EQUAL(objectValue == boolValue,   false);
    BOOST_CHECK_EQUAL(objectValue == intValue,    false);
    BOOST_CHECK_EQUAL(objectValue == doubleValue, false);
    BOOST_CHECK_EQUAL(objectValue == stringValue, false);
    BOOST_CHECK_EQUAL(objectValue == enumValue,   false);
    BOOST_CHECK_EQUAL(objectValue == objectValue, true);

    BOOST_CHECK_EQUAL(camp::Value(true)    == camp::Value(true),    true);
    BOOST_CHECK_EQUAL(camp::Value(1)       == camp::Value(1),       true);
    BOOST_CHECK_EQUAL(camp::Value(1.)      == camp::Value(1.),      true);
    BOOST_CHECK_EQUAL(camp::Value("1")     == camp::Value("1"),     true);
    BOOST_CHECK_EQUAL(camp::Value(One)     == camp::Value(One),     true);
    BOOST_CHECK_EQUAL(camp::Value(object1) == camp::Value(object1), true);

    BOOST_CHECK_EQUAL(camp::Value(true)    == camp::Value(false),   false);
    BOOST_CHECK_EQUAL(camp::Value(1)       == camp::Value(2),       false);
    BOOST_CHECK_EQUAL(camp::Value(1.)      == camp::Value(2.),      false);
    BOOST_CHECK_EQUAL(camp::Value("1")     == camp::Value("2"),     false);
    BOOST_CHECK_EQUAL(camp::Value(One)     == camp::Value(Two),     false);
    BOOST_CHECK_EQUAL(camp::Value(object1) == camp::Value(object2), false);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(lessThan)
{
    BOOST_CHECK_EQUAL(equivalent(noValue, noValue),     true);
    BOOST_CHECK_EQUAL(equivalent(noValue, boolValue),   false);
    BOOST_CHECK_EQUAL(equivalent(noValue, intValue),    false);
    BOOST_CHECK_EQUAL(equivalent(noValue, doubleValue), false);
    BOOST_CHECK_EQUAL(equivalent(noValue, stringValue), false);
    BOOST_CHECK_EQUAL(equivalent(noValue, enumValue),   false);
    BOOST_CHECK_EQUAL(equivalent(noValue, objectValue), false);

    BOOST_CHECK_EQUAL(equivalent(boolValue, noValue),     false);
    BOOST_CHECK_EQUAL(equivalent(boolValue, boolValue),   true);
    BOOST_CHECK_EQUAL(equivalent(boolValue, intValue),    false);
    BOOST_CHECK_EQUAL(equivalent(boolValue, doubleValue), false);
    BOOST_CHECK_EQUAL(equivalent(boolValue, stringValue), false);
    BOOST_CHECK_EQUAL(equivalent(boolValue, enumValue),   false);
    BOOST_CHECK_EQUAL(equivalent(boolValue, objectValue), false);

    BOOST_CHECK_EQUAL(equivalent(intValue, noValue),     false);
    BOOST_CHECK_EQUAL(equivalent(intValue, boolValue),   false);
    BOOST_CHECK_EQUAL(equivalent(intValue, intValue),    true);
    BOOST_CHECK_EQUAL(equivalent(intValue, doubleValue), false);
    BOOST_CHECK_EQUAL(equivalent(intValue, stringValue), false);
    BOOST_CHECK_EQUAL(equivalent(intValue, enumValue),   false);
    BOOST_CHECK_EQUAL(equivalent(intValue, objectValue), false);

    BOOST_CHECK_EQUAL(equivalent(doubleValue, noValue),     false);
    BOOST_CHECK_EQUAL(equivalent(doubleValue, boolValue),   false);
    BOOST_CHECK_EQUAL(equivalent(doubleValue, intValue),    false);
    BOOST_CHECK_EQUAL(equivalent(doubleValue, doubleValue), true);
    BOOST_CHECK_EQUAL(equivalent(doubleValue, stringValue), false);
    BOOST_CHECK_EQUAL(equivalent(doubleValue, enumValue),   false);
    BOOST_CHECK_EQUAL(equivalent(doubleValue, objectValue), false);

    BOOST_CHECK_EQUAL(equivalent(stringValue, noValue),     false);
    BOOST_CHECK_EQUAL(equivalent(stringValue, boolValue),   false);
    BOOST_CHECK_EQUAL(equivalent(stringValue, intValue),    false);
    BOOST_CHECK_EQUAL(equivalent(stringValue, doubleValue), false);
    BOOST_CHECK_EQUAL(equivalent(stringValue, stringValue), true);
    BOOST_CHECK_EQUAL(equivalent(stringValue, enumValue),   false);
    BOOST_CHECK_EQUAL(equivalent(stringValue, objectValue), false);

    BOOST_CHECK_EQUAL(equivalent(enumValue, noValue),     false);
    BOOST_CHECK_EQUAL(equivalent(enumValue, boolValue),   false);
    BOOST_CHECK_EQUAL(equivalent(enumValue, intValue),    false);
    BOOST_CHECK_EQUAL(equivalent(enumValue, doubleValue), false);
    BOOST_CHECK_EQUAL(equivalent(enumValue, stringValue), false);
    BOOST_CHECK_EQUAL(equivalent(enumValue, enumValue),   true);
    BOOST_CHECK_EQUAL(equivalent(enumValue, objectValue), false);

    BOOST_CHECK_EQUAL(equivalent(objectValue, noValue),     false);
    BOOST_CHECK_EQUAL(equivalent(objectValue, boolValue),   false);
    BOOST_CHECK_EQUAL(equivalent(objectValue, intValue),    false);
    BOOST_CHECK_EQUAL(equivalent(objectValue, doubleValue), false);
    BOOST_CHECK_EQUAL(equivalent(objectValue, stringValue), false);
    BOOST_CHECK_EQUAL(equivalent(objectValue, enumValue),   false);
    BOOST_CHECK_EQUAL(equivalent(objectValue, objectValue), true);

    BOOST_CHECK_EQUAL(equivalent(camp::Value(),        camp::Value()),        true);
    BOOST_CHECK_EQUAL(equivalent(camp::Value(true),    camp::Value(true)),    true);
    BOOST_CHECK_EQUAL(equivalent(camp::Value(1),       camp::Value(1)),       true);
    BOOST_CHECK_EQUAL(equivalent(camp::Value(1.),      camp::Value(1.)),      true);
    BOOST_CHECK_EQUAL(equivalent(camp::Value("1"),     camp::Value("1")),     true);
    BOOST_CHECK_EQUAL(equivalent(camp::Value(One),     camp::Value(One)),     true);
    BOOST_CHECK_EQUAL(equivalent(camp::Value(object1), camp::Value(object1)), true);
    BOOST_CHECK_EQUAL(equivalent(camp::Value(object1), camp::Value(object2)), false);

    BOOST_CHECK_EQUAL(camp::Value(false) < camp::Value(true), true);
    BOOST_CHECK_EQUAL(camp::Value(1)     < camp::Value(2),    true);
    BOOST_CHECK_EQUAL(camp::Value(1.)    < camp::Value(2.),   true);
    BOOST_CHECK_EQUAL(camp::Value("1")   < camp::Value("2"),  true);
    BOOST_CHECK_EQUAL(camp::Value(One)   < camp::Value(Two),  true);
}

BOOST_AUTO_TEST_SUITE_END()
