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

#include <ponder/classget.hpp>
#include <ponder/errors.hpp>
#include <ponder/arrayproperty.hpp>
#include <ponder/class.hpp>
#include <ponder/classbuilder.hpp>
#include "test.hpp"
#include <list>
#include <vector>

namespace ArrayPropertyTest
{
    struct MyType
    {
        MyType() : x(-1) {}
        
        MyType(int x_) : x(x_) {}
        
        bool operator == (const MyType& other) const
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
        std::vector<ponder::String> strings;
        std::list<MyType> objects;
        std::vector<std::shared_ptr<MyType>> smartptrs;
    };
    
    void declare()
    {
        ponder::Class::declare<MyType>("ArrayPropertyTest::MyType");
        
        ponder::Class::declare<MyClass>("ArrayPropertyTest::MyClass")
            .property("bools", &MyClass::bools)
            .property("ints", &MyClass::ints)
            .property("strings", &MyClass::strings)
            .property("objects", &MyClass::objects)
            //.property("smartptrs", &MyClass::smartptrs)
            ;
    }
}

PONDER_AUTO_TYPE(ArrayPropertyTest::MyType, &ArrayPropertyTest::declare)
PONDER_AUTO_TYPE(ArrayPropertyTest::MyClass, &ArrayPropertyTest::declare)

using namespace ArrayPropertyTest;


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

TEST_CASE_METHOD(ArrayPropertyFixture, "Array property can be inspected")
{
    REQUIRE(bools != nullptr);
    REQUIRE(ints != nullptr);
    REQUIRE(strings != nullptr);
    REQUIRE(objects != nullptr);

    SECTION("should be array type")
    {
        REQUIRE(bools->kind() == ponder::ValueKind::Array);
        REQUIRE(ints->kind() == ponder::ValueKind::Array);
        REQUIRE(strings->kind() == ponder::ValueKind::Array);
        REQUIRE(objects->kind() == ponder::ValueKind::Array);
    }
    
     SECTION("arrays have a type")
     {
         REQUIRE(bools->elementType() == ponder::ValueKind::Boolean);
         REQUIRE(ints->elementType() == ponder::ValueKind::Integer);
         REQUIRE(strings->elementType() == ponder::ValueKind::String);
         REQUIRE(objects->elementType() == ponder::ValueKind::User);
     }
     
     SECTION("can be dynamic")
     {
         REQUIRE(bools->dynamic() == false);
         REQUIRE(ints->dynamic() == false);
         REQUIRE(strings->dynamic() == true);
         REQUIRE(objects->dynamic() == true);
     }
     
    SECTION("have a size")
    {
        REQUIRE(bools->size(object) == std::extent<decltype(object.bools)>::value);
        REQUIRE(ints->size(object) == object.ints.size());
        REQUIRE(strings->size(object) == object.strings.size());
        REQUIRE(objects->size(object) == object.objects.size());
    }
}

TEST_CASE_METHOD(ArrayPropertyFixture, "Property arrays can be read")
{
    REQUIRE(bools->get(object, 0) == ponder::Value(object.bools[0]));
    REQUIRE(bools->get(object, 1) == ponder::Value(object.bools[1]));
    REQUIRE_THROWS_AS(bools->get(object, 2), ponder::OutOfRange);
    
    REQUIRE(ints->get(object, 0) == ponder::Value(object.ints[0]));
    REQUIRE(ints->get(object, 1) == ponder::Value(object.ints[1]));
    REQUIRE(ints->get(object, 2) == ponder::Value(object.ints[2]));
    REQUIRE_THROWS_AS(ints->get(object, 3), ponder::OutOfRange);
    
    REQUIRE(strings->get(object, 0) == ponder::Value(object.strings[0]));
    REQUIRE(strings->get(object, 1) == ponder::Value(object.strings[1]));
    REQUIRE(strings->get(object, 2) == ponder::Value(object.strings[2]));
    REQUIRE(strings->get(object, 3) == ponder::Value(object.strings[3]));
    REQUIRE_THROWS_AS(strings->get(object, 4), ponder::OutOfRange);
    
    // Test objects in the list. Note, these test check different things.
    std::list<MyType>::const_iterator it = object.objects.begin();
    // test values equivalent explicitly
    REQUIRE(objects->get(object, 0).to<MyType>().x == 0);
    REQUIRE(ponder::Value(*std::next(it, 0)).to<MyType>().x == 0);
    // Note, these are *not* equivalent as both create copies of Values and use Value ==
    IS_TRUE(objects->get(object, 1) != ponder::Value(*std::next(it, 1)));
    // convert and test: uses MyType operator ==
    IS_TRUE(objects->get(object, 2).to<MyType>() == ponder::Value(*std::next(it, 2)).to<MyType>());
    IS_TRUE(objects->get(object, 3).to<MyType>() == ponder::Value(*std::next(it, 3)).to<MyType>());
    IS_TRUE(objects->get(object, 4).to<MyType>() == ponder::Value(*std::next(it, 4)).to<MyType>());
    REQUIRE_THROWS_AS(objects->get(object, 5), ponder::OutOfRange);
}

TEST_CASE_METHOD(ArrayPropertyFixture, "Property arrays can be written to")
{
    bools->set(&object, 1, true);
    ints->set(&object, 1, 20);
    strings->set(&object, 1, "hello");
    objects->set(&object, 1, MyType(8));
    
    REQUIRE(object.bools[1] == true);
    REQUIRE(object.ints[1] == 20);
    REQUIRE(object.strings[1] == "hello");
    REQUIRE(*std::next(object.objects.begin(), 1) == MyType(8));
    
    REQUIRE_THROWS_AS(bools->set(&object, 10, true),        ponder::OutOfRange);
    REQUIRE_THROWS_AS(ints->set(&object, 10, 1),            ponder::OutOfRange);
    REQUIRE_THROWS_AS(strings->set(&object, 10, "hi"),      ponder::OutOfRange);
    REQUIRE_THROWS_AS(objects->set(&object, 10, MyType(9)), ponder::OutOfRange);
}

TEST_CASE_METHOD(ArrayPropertyFixture, "Property arrays can be inserted into")
{
    REQUIRE_THROWS_AS(bools->insert(&object, 0, true), ponder::ForbiddenWrite);
    REQUIRE_THROWS_AS(ints->insert(&object, 0, true),  ponder::ForbiddenWrite);
    
    const std::size_t stringsSize = object.strings.size();
    const std::size_t objectsSize = object.objects.size();
    
    strings->insert(&object, 1, "bonjour");
    objects->insert(&object, 1, MyType(10));
    
    REQUIRE(object.strings.size() == stringsSize + 1);
    REQUIRE(object.objects.size() == objectsSize + 1);
    
    REQUIRE(object.strings[1] == "bonjour");
    REQUIRE(*std::next(object.objects.begin(), 1) == MyType(10));
}


TEST_CASE_METHOD(ArrayPropertyFixture, "Property arrays can be removed from")
{
    REQUIRE_THROWS_AS(bools->remove(&object, 0), ponder::ForbiddenWrite);
    REQUIRE_THROWS_AS(ints->remove(&object, 0),  ponder::ForbiddenWrite);
    
    const ponder::String string1 = object.strings[1];
    const MyType      object1 = *std::next(object.objects.begin(), 1);
    
    const std::size_t stringsSize = object.strings.size();
    const std::size_t objectsSize = object.objects.size();
    
    strings->remove(&object, 0);
    objects->remove(&object, 0);
    
    REQUIRE(object.strings.size() == stringsSize - 1);
    REQUIRE(object.objects.size() == objectsSize - 1);
    
    REQUIRE(object.strings.front() == string1);
    REQUIRE(object.objects.front() == object1);
}


