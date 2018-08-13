/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2018 Nick Trout.
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

#include <ponder/value.hpp>
#include <ponder/class.hpp>
#include <ponder/enum.hpp>
#include <ponder/type.hpp>
#include <ponder/valuevisitor.hpp>
#include <ponder/enumobject.hpp>
#include <ponder/userobject.hpp>
#include <ponder/classbuilder.hpp>
#include "test.hpp"

namespace ValueTest
{
    struct MyClass
    {
        MyClass(int x_) : x(x_) {}
        int x;
        std::string str_ = "hello";
        double a[10];
        
        const std::string& str() const {return str_;}
    };
    
    bool operator == (const MyClass& left, const MyClass& right)
    {
        return left.x == right.x;
    }
    
    bool operator < (const MyClass& left, const MyClass& right)
    {
        return left.x < right.x;
    }
    
    std::ostream& operator<<(std::ostream& stream, const MyClass& object)
    {
        return stream << object.x;
    }
    
    enum MyEnum
    {
        One = 1,
        Two = 2
    };
    
    struct Visitor : public ponder::ValueVisitor<ponder::ValueKind>
    {
        ponder::ValueKind operator()(ponder::NoType)
        {
            return ponder::ValueKind::None;
        }
        
        ponder::ValueKind operator()(bool)
        {
            return ponder::ValueKind::Boolean;
        }
        
        ponder::ValueKind operator()(long)
        {
            return ponder::ValueKind::Integer;
        }
        
        ponder::ValueKind operator()(double)
        {
            return ponder::ValueKind::Real;
        }
        
        ponder::ValueKind operator()(const ponder::String&)
        {
            return ponder::ValueKind::String;
        }
        
        ponder::ValueKind operator()(const ponder::EnumObject&)
        {
            return ponder::ValueKind::Enum;
        }
        
        ponder::ValueKind operator()(const ponder::UserObject&)
        {
            return ponder::ValueKind::User;
        }
    };
    
    void declare()
    {
        ponder::Enum::declare<MyEnum>("ValueTest::MyEnum")
            .value("One", One)
            .value("Two", Two);
        ponder::Class::declare<MyClass>("ValueTest::MyClass")
            .function("str", &MyClass::str);
    }
}

PONDER_AUTO_TYPE(ValueTest::MyClass, &ValueTest::declare)
PONDER_AUTO_TYPE(ValueTest::MyEnum, &ValueTest::declare)

using namespace ValueTest;

//-----------------------------------------------------------------------------
//                         Tests for ponder::Value
//-----------------------------------------------------------------------------

TEST_CASE("Ponder has variant values")
{
    MyClass object1(1);
    MyClass object2(2);                          
    ponder::Value noValue      = ponder::Value::nothing;
    ponder::Value boolValue    = true;
    ponder::Value charValue    = static_cast<char>(1);
    ponder::Value shortValue   = static_cast<short>(1);
    ponder::Value intValue     = static_cast<int>(1);
    ponder::Value longValue    = static_cast<long>(1);
    ponder::Value ucharValue   = static_cast<unsigned char>(1);
    ponder::Value ushortValue  = static_cast<unsigned short>(1);
    ponder::Value uintValue    = static_cast<unsigned int>(1);
    ponder::Value ulongValue   = static_cast<unsigned long>(1);
    ponder::Value floatValue   = 1.f;
    ponder::Value doubleValue  = 1.;
    ponder::Value cstringValue = "1";
    ponder::Value stringValue  = ponder::String("1");
    ponder::Value enumValue    = One;
    ponder::Value objectValue  = ponder::UserObject::makeRef(&object1);
    
    SECTION("values have type")
    {
        IS_TRUE(noValue.kind() ==      ponder::ValueKind::None);
        IS_TRUE(boolValue.kind() ==    ponder::ValueKind::Boolean);
        IS_TRUE(charValue.kind() ==    ponder::ValueKind::Integer);
        IS_TRUE(shortValue.kind() ==   ponder::ValueKind::Integer);
        IS_TRUE(intValue.kind() ==     ponder::ValueKind::Integer);
        IS_TRUE(longValue.kind() ==    ponder::ValueKind::Integer);
        IS_TRUE(ucharValue.kind() ==   ponder::ValueKind::Integer);
        IS_TRUE(ushortValue.kind() ==  ponder::ValueKind::Integer);
        IS_TRUE(uintValue.kind() ==    ponder::ValueKind::Integer);
        IS_TRUE(ulongValue.kind() ==   ponder::ValueKind::Integer);
        IS_TRUE(floatValue.kind() ==   ponder::ValueKind::Real);
        IS_TRUE(doubleValue.kind() ==  ponder::ValueKind::Real);
        IS_TRUE(cstringValue.kind() == ponder::ValueKind::String);
        IS_TRUE(stringValue.kind() ==  ponder::ValueKind::String);
        IS_TRUE(enumValue.kind() ==    ponder::ValueKind::Enum);
        IS_TRUE(objectValue.kind() ==  ponder::ValueKind::User);
    }

    SECTION("boolean values can be converted to other types")
    {
        REQUIRE(boolValue.to<bool>() ==           true);
        REQUIRE(boolValue.to<char>() ==           1);
        REQUIRE(boolValue.to<short>() ==          1);
        REQUIRE(boolValue.to<int>() ==            1);
        REQUIRE(boolValue.to<long>() ==           1);
        REQUIRE(boolValue.to<unsigned char>() ==  1);
        REQUIRE(boolValue.to<unsigned short>() == 1);
        REQUIRE(boolValue.to<unsigned int>() ==   1);
        REQUIRE(boolValue.to<unsigned long>() ==  1);
        REQUIRE(boolValue.to<float>() == Approx(1.f).epsilon(1E-5f));
        REQUIRE(boolValue.to<double>() == Approx(1.).epsilon(1E-5));
        REQUIRE(boolValue.to<ponder::String>() ==    "1");
        REQUIRE(boolValue.to<MyEnum>() ==         One);
        REQUIRE_THROWS_AS(boolValue.to<MyClass>(), ponder::BadType);

        REQUIRE(boolValue.isCompatible<bool>() ==           true);
        REQUIRE(boolValue.isCompatible<char>() ==           true);
        REQUIRE(boolValue.isCompatible<short>() ==          true);
        REQUIRE(boolValue.isCompatible<int>() ==            true);
        REQUIRE(boolValue.isCompatible<long>() ==           true);
        REQUIRE(boolValue.isCompatible<unsigned char>() ==  true);
        REQUIRE(boolValue.isCompatible<unsigned short>() == true);
        REQUIRE(boolValue.isCompatible<unsigned int>() ==   true);
        REQUIRE(boolValue.isCompatible<unsigned long>() ==  true);
        REQUIRE(boolValue.isCompatible<float>() ==          true);
        REQUIRE(boolValue.isCompatible<double>() ==         true);
        REQUIRE(boolValue.isCompatible<ponder::String>() == true);
        REQUIRE(boolValue.isCompatible<MyEnum>() ==         true);
        REQUIRE(boolValue.isCompatible<MyClass>() ==        false);
    }

    SECTION("integer values can be converted to other types")
    {
        REQUIRE(intValue.to<bool>() ==           true);
        REQUIRE(intValue.to<char>() ==           1);
        REQUIRE(intValue.to<short>() ==          1);
        REQUIRE(intValue.to<int>() ==            1);
        REQUIRE(intValue.to<long>() ==           1);
        REQUIRE(intValue.to<unsigned char>() ==  1);
        REQUIRE(intValue.to<unsigned short>() == 1);
        REQUIRE(intValue.to<unsigned int>() ==   1);
        REQUIRE(intValue.to<unsigned long>() ==  1);
        REQUIRE(intValue.to<float>() == Approx(1.f).epsilon(1E-5f));
        REQUIRE(intValue.to<double>() == Approx(1.).epsilon(1E-5));
        REQUIRE(intValue.to<ponder::String>() ==    "1");
        REQUIRE(intValue.to<MyEnum>() ==         One);
        REQUIRE_THROWS_AS(intValue.to<MyClass>(),        ponder::BadType);

        REQUIRE(intValue.isCompatible<bool>() ==           true);
        REQUIRE(intValue.isCompatible<char>() ==           true);
        REQUIRE(intValue.isCompatible<short>() ==          true);
        REQUIRE(intValue.isCompatible<int>() ==            true);
        REQUIRE(intValue.isCompatible<long>() ==           true);
        REQUIRE(intValue.isCompatible<unsigned char>() ==  true);
        REQUIRE(intValue.isCompatible<unsigned short>() == true);
        REQUIRE(intValue.isCompatible<unsigned int>() ==   true);
        REQUIRE(intValue.isCompatible<unsigned long>() ==  true);
        REQUIRE(intValue.isCompatible<float>() ==          true);
        REQUIRE(intValue.isCompatible<double>() ==         true);
        REQUIRE(intValue.isCompatible<ponder::String>() ==    true);
        REQUIRE(intValue.isCompatible<MyEnum>() ==         true);
        REQUIRE(intValue.isCompatible<MyClass>() ==        false);
    }

    SECTION("double values can be converted to other types")
    {
        REQUIRE(doubleValue.to<bool>() ==           true);
        REQUIRE(doubleValue.to<char>() ==           1);
        REQUIRE(doubleValue.to<short>() ==          1);
        REQUIRE(doubleValue.to<int>() ==            1);
        REQUIRE(doubleValue.to<long>() ==           1);
        REQUIRE(doubleValue.to<unsigned char>() ==  1);
        REQUIRE(doubleValue.to<unsigned short>() == 1);
        REQUIRE(doubleValue.to<unsigned int>() ==   1);
        REQUIRE(doubleValue.to<unsigned long>() ==  1);
        REQUIRE(doubleValue.to<float>() == Approx(1.f).epsilon(1E-5f));
        REQUIRE(doubleValue.to<double>() == Approx(1.).epsilon(1E-5));
        REQUIRE(doubleValue.to<ponder::String>() == "1.000000");
        REQUIRE(doubleValue.to<MyEnum>() ==         One);
        REQUIRE_THROWS_AS(doubleValue.to<MyClass>(), ponder::BadType);

        REQUIRE(doubleValue.isCompatible<bool>() ==           true);
        REQUIRE(doubleValue.isCompatible<char>() ==           true);
        REQUIRE(doubleValue.isCompatible<short>() ==          true);
        REQUIRE(doubleValue.isCompatible<int>() ==            true);
        REQUIRE(doubleValue.isCompatible<long>() ==           true);
        REQUIRE(doubleValue.isCompatible<unsigned char>() ==  true);
        REQUIRE(doubleValue.isCompatible<unsigned short>() == true);
        REQUIRE(doubleValue.isCompatible<unsigned int>() ==   true);
        REQUIRE(doubleValue.isCompatible<unsigned long>() ==  true);
        REQUIRE(doubleValue.isCompatible<float>() ==          true);
        REQUIRE(doubleValue.isCompatible<double>() ==         true);
        REQUIRE(doubleValue.isCompatible<ponder::String>() ==    true);
        REQUIRE(doubleValue.isCompatible<MyEnum>() ==         true);
        REQUIRE(doubleValue.isCompatible<MyClass>() ==        false);
    }

    SECTION("string values can be converted to other types")
    {
        REQUIRE(stringValue.to<bool>() ==           true);
        REQUIRE(stringValue.to<char>() ==           '1');
        REQUIRE(stringValue.to<short>() ==          1);
        REQUIRE(stringValue.to<int>() ==            1);
        REQUIRE(stringValue.to<long>() ==           1);
        REQUIRE(stringValue.to<unsigned char>() ==  '1');
        REQUIRE(stringValue.to<unsigned short>() == 1);
        REQUIRE(stringValue.to<unsigned int>() ==   1);
        REQUIRE(stringValue.to<unsigned long>() ==  1);
        REQUIRE(stringValue.to<float>() == Approx(1.f).epsilon(1E-5f));
        REQUIRE(stringValue.to<double>() == Approx(1.).epsilon(1E-5));
        REQUIRE(stringValue.to<ponder::String>() ==    "1");
        REQUIRE(stringValue.to<MyEnum>() ==         One);
        REQUIRE_THROWS_AS(stringValue.to<MyClass>(), ponder::BadType);

        REQUIRE(stringValue.isCompatible<bool>() ==           true);
        REQUIRE(stringValue.isCompatible<char>() ==           true);
        REQUIRE(stringValue.isCompatible<short>() ==          true);
        REQUIRE(stringValue.isCompatible<int>() ==            true);
        REQUIRE(stringValue.isCompatible<long>() ==           true);
        REQUIRE(stringValue.isCompatible<unsigned char>() ==  true);
        REQUIRE(stringValue.isCompatible<unsigned short>() == true);
        REQUIRE(stringValue.isCompatible<unsigned int>() ==   true);
        REQUIRE(stringValue.isCompatible<unsigned long>() ==  true);
        REQUIRE(stringValue.isCompatible<float>() ==          true);
        REQUIRE(stringValue.isCompatible<double>() ==         true);
        REQUIRE(stringValue.isCompatible<ponder::String>() ==    true);
        REQUIRE(stringValue.isCompatible<MyEnum>() ==         true);
        REQUIRE(stringValue.isCompatible<MyClass>() ==        false);
    }

    SECTION("strings can be converted to enums")
    {
        REQUIRE(ponder::Value("One").to<MyEnum>() == One);
        REQUIRE(ponder::Value("Two").to<MyEnum>() == Two);
        REQUIRE(ponder::Value("1").to<MyEnum>() ==   One);
        REQUIRE(ponder::Value("2").to<MyEnum>() ==   Two);

        REQUIRE(ponder::Value("One").isCompatible<MyEnum>() == true);
        REQUIRE(ponder::Value("Two").isCompatible<MyEnum>() == true);
        REQUIRE(ponder::Value("1").isCompatible<MyEnum>() ==   true);
        REQUIRE(ponder::Value("2").isCompatible<MyEnum>() ==   true);
    }

    SECTION("not all string values can be converted to other values")
    {
        const ponder::Value badStringValue = "not a number";

        REQUIRE_THROWS_AS(badStringValue.to<bool>(),           ponder::BadType);
        REQUIRE_THROWS_AS(badStringValue.to<char>(),           ponder::BadType);
        REQUIRE_THROWS_AS(badStringValue.to<short>(),          ponder::BadType);
        REQUIRE_THROWS_AS(badStringValue.to<int>(),            ponder::BadType);
        REQUIRE_THROWS_AS(badStringValue.to<long>(),           ponder::BadType);
        REQUIRE_THROWS_AS(badStringValue.to<unsigned char>(),  ponder::BadType);
        REQUIRE_THROWS_AS(badStringValue.to<unsigned short>(), ponder::BadType);
        REQUIRE_THROWS_AS(badStringValue.to<unsigned int>(),   ponder::BadType);
        REQUIRE_THROWS_AS(badStringValue.to<unsigned long>(),  ponder::BadType);
        REQUIRE_THROWS_AS(badStringValue.to<float>(),          ponder::BadType);
        REQUIRE_THROWS_AS(badStringValue.to<double>(),         ponder::BadType);
        REQUIRE_THROWS_AS(badStringValue.to<MyEnum>(),         ponder::BadType);
        REQUIRE_THROWS_AS(badStringValue.to<MyClass>(),        ponder::BadType);

        REQUIRE(badStringValue.isCompatible<bool>() ==           false);
        REQUIRE(badStringValue.isCompatible<char>() ==           false);
        REQUIRE(badStringValue.isCompatible<short>() ==          false);
        REQUIRE(badStringValue.isCompatible<int>() ==            false);
        REQUIRE(badStringValue.isCompatible<long>() ==           false);
        REQUIRE(badStringValue.isCompatible<unsigned char>() ==  false);
        REQUIRE(badStringValue.isCompatible<unsigned short>() == false);
        REQUIRE(badStringValue.isCompatible<unsigned int>() ==   false);
        REQUIRE(badStringValue.isCompatible<unsigned long>() ==  false);
        REQUIRE(badStringValue.isCompatible<float>() ==          false);
        REQUIRE(badStringValue.isCompatible<double>() ==         false);
        REQUIRE(badStringValue.isCompatible<ponder::String>() ==    true);
        REQUIRE(badStringValue.isCompatible<MyEnum>() ==         false);
        REQUIRE(badStringValue.isCompatible<MyClass>() ==        false);
    }

    SECTION("enum values can be converted to other types")
    {
        REQUIRE(enumValue.to<bool>() ==           true);
        REQUIRE(enumValue.to<char>() ==           1);
        REQUIRE(enumValue.to<short>() ==          1);
        REQUIRE(enumValue.to<int>() ==            1);
        REQUIRE(enumValue.to<long>() ==           1);
        REQUIRE(enumValue.to<unsigned char>() ==  1);
        REQUIRE(enumValue.to<unsigned short>() == 1);
        REQUIRE(enumValue.to<unsigned int>() ==   1);
        REQUIRE(enumValue.to<unsigned long>() ==  1);
        REQUIRE(enumValue.to<float>() == Approx(1.f).epsilon(1E-5f));
        REQUIRE(enumValue.to<double>() == Approx(1.).epsilon(1E-5));
        REQUIRE(enumValue.to<ponder::String>() ==    "One");
        REQUIRE(enumValue.to<MyEnum>() ==         One);
        REQUIRE_THROWS_AS(enumValue.to<MyClass>(), ponder::BadType);

        REQUIRE(enumValue.isCompatible<bool>() ==           true);
        REQUIRE(enumValue.isCompatible<char>() ==           true);
        REQUIRE(enumValue.isCompatible<short>() ==          true);
        REQUIRE(enumValue.isCompatible<int>() ==            true);
        REQUIRE(enumValue.isCompatible<long>() ==           true);
        REQUIRE(enumValue.isCompatible<unsigned char>() ==  true);
        REQUIRE(enumValue.isCompatible<unsigned short>() == true);
        REQUIRE(enumValue.isCompatible<unsigned int>() ==   true);
        REQUIRE(enumValue.isCompatible<unsigned long>() ==  true);
        REQUIRE(enumValue.isCompatible<float>() ==          true);
        REQUIRE(enumValue.isCompatible<double>() ==         true);
        REQUIRE(enumValue.isCompatible<ponder::String>() ==    true);
        REQUIRE(enumValue.isCompatible<MyEnum>() ==         true);
        REQUIRE(enumValue.isCompatible<MyClass>() ==        false);
    }

    SECTION("user object values can be converted to other types")
    {
        REQUIRE(objectValue.to<bool>() == true);
        REQUIRE_THROWS_AS(objectValue.to<char>(),              ponder::BadType);
        REQUIRE_THROWS_AS(objectValue.to<short>(),             ponder::BadType);
        REQUIRE_THROWS_AS(objectValue.to<int>(),               ponder::BadType);
        REQUIRE_THROWS_AS(objectValue.to<long>(),              ponder::BadType);
        REQUIRE_THROWS_AS(objectValue.to<unsigned char>(),     ponder::BadType);
        REQUIRE_THROWS_AS(objectValue.to<unsigned short>(),    ponder::BadType);
        REQUIRE_THROWS_AS(objectValue.to<unsigned int>(),      ponder::BadType);
        REQUIRE_THROWS_AS(objectValue.to<unsigned long>(),     ponder::BadType);
        REQUIRE_THROWS_AS(objectValue.to<float>(),             ponder::BadType);
        REQUIRE_THROWS_AS(objectValue.to<double>(),            ponder::BadType);
        REQUIRE_THROWS_AS(objectValue.to<ponder::String>(),       ponder::BadType);
        REQUIRE_THROWS_AS(objectValue.to<MyEnum>(),            ponder::BadType);
        REQUIRE(objectValue.to<MyClass>() == object1);
        IS_TRUE(objectValue.to<ponder::UserObject>() == ponder::UserObject(&object1));

        REQUIRE(objectValue.isCompatible<bool>() ==           true);
        REQUIRE(objectValue.isCompatible<char>() ==           false);
        REQUIRE(objectValue.isCompatible<short>() ==          false);
        REQUIRE(objectValue.isCompatible<int>() ==            false);
        REQUIRE(objectValue.isCompatible<long>() ==           false);
        REQUIRE(objectValue.isCompatible<unsigned char>() ==  false);
        REQUIRE(objectValue.isCompatible<unsigned short>() == false);
        REQUIRE(objectValue.isCompatible<unsigned int>() ==   false);
        REQUIRE(objectValue.isCompatible<unsigned long>() ==  false);
        REQUIRE(objectValue.isCompatible<float>() ==          false);
        REQUIRE(objectValue.isCompatible<double>() ==         false);
        REQUIRE(objectValue.isCompatible<ponder::String>() == false);
        REQUIRE(objectValue.isCompatible<MyEnum>() ==         false);
        REQUIRE(objectValue.isCompatible<MyClass>() ==        true);
    }

    SECTION("values have a type visitor")
    {
        Visitor visitor;

        IS_TRUE(noValue.visit(visitor) ==      ponder::ValueKind::None);
        IS_TRUE(boolValue.visit(visitor) ==    ponder::ValueKind::Boolean);
        IS_TRUE(charValue.visit(visitor) ==    ponder::ValueKind::Integer);
        IS_TRUE(shortValue.visit(visitor) ==   ponder::ValueKind::Integer);
        IS_TRUE(intValue.visit(visitor) ==     ponder::ValueKind::Integer);
        IS_TRUE(longValue.visit(visitor) ==    ponder::ValueKind::Integer);
        IS_TRUE(ucharValue.visit(visitor) ==   ponder::ValueKind::Integer);
        IS_TRUE(ushortValue.visit(visitor) ==  ponder::ValueKind::Integer);
        IS_TRUE(uintValue.visit(visitor) ==    ponder::ValueKind::Integer);
        IS_TRUE(ulongValue.visit(visitor) ==   ponder::ValueKind::Integer);
        IS_TRUE(floatValue.visit(visitor) ==   ponder::ValueKind::Real);
        IS_TRUE(doubleValue.visit(visitor) ==  ponder::ValueKind::Real);
        IS_TRUE(cstringValue.visit(visitor) == ponder::ValueKind::String);
        IS_TRUE(stringValue.visit(visitor) ==  ponder::ValueKind::String);
        IS_TRUE(enumValue.visit(visitor) ==    ponder::ValueKind::Enum);
        IS_TRUE(objectValue.visit(visitor) ==  ponder::ValueKind::User);
    }

    SECTION("values can be compared for equality")
    {
        REQUIRE((noValue == noValue) ==     true);
        REQUIRE((noValue == boolValue) ==   false);
        REQUIRE((noValue == intValue) ==    false);
        REQUIRE((noValue == doubleValue) == false);
        REQUIRE((noValue == stringValue) == false);
        REQUIRE((noValue == enumValue) ==   false);
        REQUIRE((noValue == objectValue) == false);

        REQUIRE((boolValue == noValue) ==     false);
        REQUIRE((boolValue == boolValue) ==   true);
        REQUIRE((boolValue == intValue) ==    false);
        REQUIRE((boolValue == doubleValue) == false);
        REQUIRE((boolValue == stringValue) == false);
        REQUIRE((boolValue == enumValue) ==   false);
        REQUIRE((boolValue == objectValue) == false);

        REQUIRE((intValue == noValue) ==     false);
        REQUIRE((intValue == boolValue) ==   false);
        REQUIRE((intValue == intValue) ==    true);
        REQUIRE((intValue == doubleValue) == false);
        REQUIRE((intValue == stringValue) == false);
        REQUIRE((intValue == enumValue) ==   false);
        REQUIRE((intValue == objectValue) == false);

        REQUIRE((doubleValue == noValue) ==     false);
        REQUIRE((doubleValue == boolValue) ==   false);
        REQUIRE((doubleValue == intValue) ==    false);
        REQUIRE((doubleValue == doubleValue) == true);
        REQUIRE((doubleValue == stringValue) == false);
        REQUIRE((doubleValue == enumValue) ==   false);
        REQUIRE((doubleValue == objectValue) == false);

        REQUIRE((stringValue == noValue) ==     false);
        REQUIRE((stringValue == boolValue) ==   false);
        REQUIRE((stringValue == intValue) ==    false);
        REQUIRE((stringValue == doubleValue) == false);
        REQUIRE((stringValue == stringValue) == true);
        REQUIRE((stringValue == enumValue) ==   false);
        REQUIRE((stringValue == objectValue) == false);

        REQUIRE((enumValue == noValue) ==     false);
        REQUIRE((enumValue == boolValue) ==   false);
        REQUIRE((enumValue == intValue) ==    false);
        REQUIRE((enumValue == doubleValue) == false);
        REQUIRE((enumValue == stringValue) == false);
        REQUIRE((enumValue == enumValue) ==   true);
        REQUIRE((enumValue == objectValue) == false);

        REQUIRE((objectValue == noValue) ==     false);
        REQUIRE((objectValue == boolValue) ==   false);
        REQUIRE((objectValue == intValue) ==    false);
        REQUIRE((objectValue == doubleValue) == false);
        REQUIRE((objectValue == stringValue) == false);
        REQUIRE((objectValue == enumValue) ==   false);
        REQUIRE((objectValue == objectValue) == true);

        REQUIRE(ponder::Value(true)    == ponder::Value(true));
        REQUIRE(ponder::Value(1)       == ponder::Value(1));
        REQUIRE(ponder::Value(1.)      == ponder::Value(1.));
        REQUIRE(ponder::Value("1")     == ponder::Value("1"));
        REQUIRE(ponder::Value(One)     == ponder::Value(One));
        IS_TRUE(ponder::Value(&object1) == ponder::Value(&object1));    // refs are ==
        IS_TRUE(ponder::Value(object1) != ponder::Value(&object1));     // copies are !=

        REQUIRE(ponder::Value(true)    != ponder::Value(false));
        REQUIRE(ponder::Value(1)       != ponder::Value(2));
        REQUIRE(ponder::Value(1.)      != ponder::Value(2.));
        REQUIRE(ponder::Value("1")     != ponder::Value("2"));
        REQUIRE(ponder::Value(One)     != ponder::Value(Two));
        IS_TRUE(ponder::Value(&object1) != ponder::Value(&object2));    // ref
        IS_TRUE(ponder::Value(object1) != ponder::Value(object2));      // copy
    }

    SECTION("values can be compared using less than")
    {
#define equivalent(left, right) (!(left < right) && !(right < left))

        REQUIRE(equivalent(noValue, noValue) ==     true);
        REQUIRE(equivalent(noValue, boolValue) ==   false);
        REQUIRE(equivalent(noValue, intValue) ==    false);
        REQUIRE(equivalent(noValue, doubleValue) == false);
        REQUIRE(equivalent(noValue, stringValue) == false);
        REQUIRE(equivalent(noValue, enumValue) ==   false);
        REQUIRE(equivalent(noValue, objectValue) == false);

        REQUIRE(equivalent(boolValue, noValue) ==     false);
        REQUIRE(equivalent(boolValue, boolValue) ==   true);
        REQUIRE(equivalent(boolValue, intValue) ==    false);
        REQUIRE(equivalent(boolValue, doubleValue) == false);
        REQUIRE(equivalent(boolValue, stringValue) == false);
        REQUIRE(equivalent(boolValue, enumValue) ==   false);
        REQUIRE(equivalent(boolValue, objectValue) == false);

        REQUIRE(equivalent(intValue, noValue) ==     false);
        REQUIRE(equivalent(intValue, boolValue) ==   false);
        REQUIRE(equivalent(intValue, intValue) ==    true);
        REQUIRE(equivalent(intValue, doubleValue) == false);
        REQUIRE(equivalent(intValue, stringValue) == false);
        REQUIRE(equivalent(intValue, enumValue) ==   false);
        REQUIRE(equivalent(intValue, objectValue) == false);

        REQUIRE(equivalent(doubleValue, noValue) ==     false);
        REQUIRE(equivalent(doubleValue, boolValue) ==   false);
        REQUIRE(equivalent(doubleValue, intValue) ==    false);
        REQUIRE(equivalent(doubleValue, doubleValue) == true);
        REQUIRE(equivalent(doubleValue, stringValue) == false);
        REQUIRE(equivalent(doubleValue, enumValue) ==   false);
        REQUIRE(equivalent(doubleValue, objectValue) == false);

        REQUIRE(equivalent(stringValue, noValue) ==     false);
        REQUIRE(equivalent(stringValue, boolValue) ==   false);
        REQUIRE(equivalent(stringValue, intValue) ==    false);
        REQUIRE(equivalent(stringValue, doubleValue) == false);
        REQUIRE(equivalent(stringValue, stringValue) == true);
        REQUIRE(equivalent(stringValue, enumValue) ==   false);
        REQUIRE(equivalent(stringValue, objectValue) == false);

        REQUIRE(equivalent(enumValue, noValue) ==     false);
        REQUIRE(equivalent(enumValue, boolValue) ==   false);
        REQUIRE(equivalent(enumValue, intValue) ==    false);
        REQUIRE(equivalent(enumValue, doubleValue) == false);
        REQUIRE(equivalent(enumValue, stringValue) == false);
        REQUIRE(equivalent(enumValue, enumValue) ==   true);
        REQUIRE(equivalent(enumValue, objectValue) == false);

        REQUIRE(equivalent(objectValue, noValue) ==     false);
        REQUIRE(equivalent(objectValue, boolValue) ==   false);
        REQUIRE(equivalent(objectValue, intValue) ==    false);
        REQUIRE(equivalent(objectValue, doubleValue) == false);
        REQUIRE(equivalent(objectValue, stringValue) == false);
        REQUIRE(equivalent(objectValue, enumValue) ==   false);
        REQUIRE(equivalent(objectValue, objectValue) == true);

        REQUIRE(equivalent(ponder::Value(),        ponder::Value()) ==        true);
        REQUIRE(equivalent(ponder::Value(true),    ponder::Value(true)) ==    true);
        REQUIRE(equivalent(ponder::Value(1),       ponder::Value(1)) ==       true);
        REQUIRE(equivalent(ponder::Value(1.),      ponder::Value(1.)) ==      true);
        REQUIRE(equivalent(ponder::Value("1"),     ponder::Value("1")) ==     true);
        REQUIRE(equivalent(ponder::Value(One),     ponder::Value(One)) ==     true);
        REQUIRE(equivalent(ponder::Value(&object1), ponder::Value(&object1)) == true);
        REQUIRE(equivalent(ponder::Value(&object1), ponder::Value(&object2)) == false);

        REQUIRE((ponder::Value(false) < ponder::Value(true)) == true);
        REQUIRE((ponder::Value(1)     < ponder::Value(2)) ==    true);
        REQUIRE((ponder::Value(1.)    < ponder::Value(2.)) ==   true);
        REQUIRE((ponder::Value("1")   < ponder::Value("2")) ==  true);
        REQUIRE((ponder::Value(One)   < ponder::Value(Two)) ==  true);
        
#undef equivalent
    }
}

TEST_CASE("We can convert values from strings")
{
    using ponder::detail::conv;

    SECTION("bool")
    {
        bool r;

        REQUIRE(conv("1", r) == true);
        REQUIRE(r == true);

        REQUIRE(conv("true", r) == true);
        REQUIRE(r == true);

        REQUIRE(conv("0", r) == true);
        REQUIRE(r == false);

        REQUIRE(conv("false", r) == true);
        REQUIRE(r == false);
    }

    SECTION("bool fail")
    {
        bool r;

        REQUIRE(conv("X", r) == false);
        REQUIRE(conv("", r) == false);
    }
    
    SECTION("int")
    {
        int r;

        REQUIRE(conv("1", r) == true);
        REQUIRE(r == 1);

        REQUIRE(conv("-1", r) == true);
        REQUIRE(r == -1);

        REQUIRE(conv("87654321", r) == true);
        REQUIRE(r == 87654321);

        REQUIRE(conv("-87654321", r) == true);
        REQUIRE(r == -87654321);
        
        REQUIRE(conv("-87654321", r) == true);
        REQUIRE(r == -87654321);
        
        REQUIRE(conv("0xabc", r) == true);  // hex
        REQUIRE(r == 0xABC);
        
        REQUIRE(conv("0654", r) == true);   // octal
        REQUIRE(r == 0654);
    }

    SECTION("int fail")
    {
        int r;

        REQUIRE(conv("", r) == false);
        REQUIRE(conv("whoops", r) == false);
        REQUIRE(conv(".78", r) == false);
    }
    
    SECTION("long long")
    {
        long long r;

        REQUIRE(conv("1", r) == true);
        REQUIRE(r == 1);

        REQUIRE(conv("-1", r) == true);
        REQUIRE(r == -1);

        REQUIRE(conv("87654321", r) == true);
        REQUIRE(r == 87654321);

        REQUIRE(conv("-87654321", r) == true);
        REQUIRE(r == -87654321);
        
        REQUIRE(conv("-87654321", r) == true);
        REQUIRE(r == -87654321);
        
        REQUIRE(conv("0xabc", r) == true);  // hex
        REQUIRE(r == 0xABC);

        REQUIRE(conv("0x1234567812345678", r) == true);  // hex
        REQUIRE(r == 0x1234567812345678);
        
        REQUIRE(conv("0654", r) == true);   // octal
        REQUIRE(r == 0654);
    }

    SECTION("long long fail")
    {
        long long r;

        REQUIRE(conv("", r) == false);
        REQUIRE(conv("whoops", r) == false);
        REQUIRE(conv(".78", r) == false);
    }
    
    SECTION("float")
    {
        float r;

        REQUIRE(conv("1", r) == true);
        REQUIRE(r == 1.f);

        REQUIRE(conv("-1", r) == true);
        REQUIRE(r == -1.f);

        REQUIRE(conv("87654321", r) == true);
        REQUIRE(r == 87654321);

        REQUIRE(conv("-87654321", r) == true);
        REQUIRE(r == -87654321);
        
        REQUIRE(conv("-87654321", r) == true);
        REQUIRE(r == -87654321);
        
        REQUIRE(conv("0xabc", r) == true);  // hex
        REQUIRE(r == 0xABC);        
    }

    SECTION("float fail")
    {
        float r;

        REQUIRE(conv("", r) == false);
        REQUIRE(conv("whoops", r) == false);
    }

}

TEST_CASE("Values have their type determined")
{
    static_assert(ponder_ext::ValueMapper<bool>::kind == ponder::ValueKind::Boolean, "");
    static_assert(ponder_ext::ValueMapper<char>::kind == ponder::ValueKind::Integer, "");
    static_assert(ponder_ext::ValueMapper<int>::kind == ponder::ValueKind::Integer, "");
    static_assert(ponder_ext::ValueMapper<long>::kind == ponder::ValueKind::Integer, "");
    static_assert(ponder_ext::ValueMapper<float>::kind == ponder::ValueKind::Real, "");
    static_assert(ponder_ext::ValueMapper<double>::kind == ponder::ValueKind::Real, "");
    static_assert(ponder_ext::ValueMapper<MyEnum>::kind == ponder::ValueKind::Enum, "");
    static_assert(ponder_ext::ValueMapper<int[10]>::kind == ponder::ValueKind::Array, "");
    static_assert(ponder_ext::ValueMapper<MyClass>::kind == ponder::ValueKind::User, "");
    
    static_assert(ponder_ext::ValueMapper<std::string>::kind == ponder::ValueKind::String, "");
    static_assert(ponder_ext::ValueMapper<const std::string>::kind == ponder::ValueKind::String, "");
    static_assert(ponder_ext::ValueMapper<const char*>::kind == ponder::ValueKind::String, "");
}

TEST_CASE("Values can be held in containers")
{
    SECTION("a std::map")
    {
        std::map<ponder::String, ponder::Value> testmap;
        ponder::Value value = "Hello";
        testmap["1"] = value;
        REQUIRE(testmap["1"] == ponder::String("Hello"));
    }
}

TEST_CASE("Default values can be created")
{
    SECTION("builtin")
    {
        ponder::detail::ValueProvider<int> pi;
        int i(pi());
    }

    SECTION("string")
    {
        ponder::detail::ValueProvider<std::string> vp;
        std::string v(vp());
    }

//    SECTION("class")
//    {
//        ponder::detail::ValueProvider<MyClass> vp;
//        MyClass v(vp());
//    }

//    SECTION("smart ptr") // TODO - fix creation of smart ptr default values
//    {
//        ponder::detail::ValueProvider<std::shared_ptr<MyClass>> vp;
//        std::shared_ptr<MyClass> v(vp());
//    }
}

TEST_CASE("Value mapper maps between C++ and Ponder runtime types")
{
    SECTION("return types")
    {
        REQUIRE(ponder::mapType<std::string>() == ponder::ValueKind::String);
        REQUIRE(ponder::mapType<const std::string&>() == ponder::ValueKind::String);
    }
}


