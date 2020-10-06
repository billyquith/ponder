/****************************************************************************
**
** This file is part of the CAMP library.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2020 Nick Trout.
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

// Tests for UserProperty, an exposed UserObject.

#include "test.hpp"
#include <ponder/classbuilder.hpp>
#include <ponder/uses/runtime.hpp>


namespace UserPropertyTest
{
    struct Inner
    {
        Inner(int x_) : x(x_) {}
        int x;
    };
    
    struct MyObject
    {
        MyObject(int x) : prop(x) {}
        
        Inner prop;
        
        // by ref
        const Inner& getr() const {return prop;}
        void setr(const Inner& p) {prop = p;}
        
        // by value
        Inner getv() const {return prop;}
        void setv(Inner p) {prop = p;}
    };
    
    static void declare()
    {
        ponder::Class::declare<UserPropertyTest::Inner>();
        
        ponder::Class::declare<UserPropertyTest::MyObject>()
            .constructor<int>()
            .property("prop", &MyObject::prop)
            .property("prop2", &MyObject::getr, &MyObject::setr)
            .property("prop3", &MyObject::getv, &MyObject::setv)
            ;
    }
}

PONDER_AUTO_TYPE(UserPropertyTest::Inner, &UserPropertyTest::declare)
PONDER_AUTO_TYPE(UserPropertyTest::MyObject, &UserPropertyTest::declare)

//-----------------------------------------------------------------------------
//                         Tests for ponder::UserProperty
//-----------------------------------------------------------------------------

using namespace UserPropertyTest;

TEST_CASE("Ponder has user properties (member variable)")
{
    const ponder::Class& metaclass = ponder::classByType<MyObject>();
    
    const ponder::Property* property = &metaclass.property("prop");
    
    IS_TRUE(property->kind() == ponder::ValueKind::User);
    
    IS_TRUE(static_cast<const ponder::UserProperty*>(property)->getClass()
            == ponder::classByType<Inner>());
    
    SECTION("propery values can be got")
    {
        REQUIRE(property->get(MyObject(-1)).to<Inner>().x == -1);
        REQUIRE(property->get(MyObject(20)).to<Inner>().x == 20);
    }
    
    SECTION("property values can be set")
    {
        MyObject object1(1);
        MyObject object2(10);
        property->set(&object1, Inner(2));
        property->set(&object2, Inner(20));
        
        // reverse order on purpose (to exhibit memory corruptions)
        REQUIRE(property->get(object1).to<Inner>().x ==  2);
        REQUIRE(object1.prop.x == 2);
        REQUIRE(property->get(object2).to<Inner>().x == 20);
        REQUIRE(object2.prop.x == 20);
    }
    
    SECTION("user properties wrap user objects")
    {
        MyObject object1(11);
        property->set(&object1, Inner(22));

        ponder::UserObject uobj(&object1);
        REQUIRE((uobj.cref<MyObject>().prop.x == object1.prop.x));
    }
}

TEST_CASE("Ponder has user properties (accessors: by ref)")
{
    const ponder::Class& metaclass = ponder::classByType<MyObject>();
    const ponder::Property& property = metaclass.property("prop2");
    
    IS_TRUE(property.kind() == ponder::ValueKind::User);

    // TODO: investigate
////    IS_TRUE(static_cast<const ponder::UserProperty&>(property).getClass()
//            == ponder::classByType<MyType>());
    
    SECTION("propery values can be got")
    {
        REQUIRE(property.get(MyObject(-1)).to<Inner>().x == -1);
        REQUIRE(property.get(MyObject(20)).to<Inner>().x == 20);
    }
    
    SECTION("property values can be set")
    {
        MyObject object1(1);
        MyObject object2(10);
        property.set(&object1, Inner(2));
        property.set(&object2, Inner(20));
        
        // reverse order on purpose (to exhibit memory corruptions)
        REQUIRE(property.get(object1).to<Inner>().x ==  2);
        REQUIRE(object1.prop.x == 2);
        REQUIRE(property.get(object2).to<Inner>().x == 20);
        REQUIRE(object2.prop.x == 20);
    }
    
    SECTION("user properties wrap user objects")
    {
        MyObject object1(11);
        property.set(&object1, Inner(22));
        REQUIRE(object1.prop.x == 22);

        ponder::UserObject uobj(&object1);
        REQUIRE((uobj.cref<MyObject>().prop.x == object1.prop.x));
        
        Inner mt(uobj.get("prop2").to<Inner>());
        REQUIRE(mt.x == 22);
    }
    
    SECTION("can get/set values on created objects")
    {
        const ponder::Class& metaclass = ponder::classByType<MyObject>();
        auto uobj = ponder::runtime::create(metaclass, 77);
        REQUIRE(uobj.get("prop2").to<Inner>().x == 77);
        
        Inner inr(-83);
        uobj.set("prop2", inr);
        REQUIRE(inr.x == -83);
        REQUIRE(uobj.get("prop2").to<Inner>().x == -83);
        
        ponder::Value cv = uobj.get("prop2");

        Inner inr2 = uobj.get("prop2").to<Inner>();
        REQUIRE(inr2.x == -83);
    }
}

TEST_CASE("Ponder has user properties (accessors: by value)")
{
    const ponder::Class& metaclass = ponder::classByType<MyObject>();
    const ponder::Property& property = metaclass.property("prop3");
    
    IS_TRUE(property.kind() == ponder::ValueKind::User);
    
//    IS_TRUE(static_cast<const ponder::UserProperty&>(property).getClass()
//            == ponder::classByType<MyObject>());
    
    SECTION("propery values can be got")
    {
        REQUIRE(property.get(MyObject(-1)).to<Inner>().x == -1);
        REQUIRE(property.get(MyObject(20)).to<Inner>().x == 20);
    }
    
    SECTION("property values can be set")
    {
        MyObject object1(1);
        MyObject object2(10);
        property.set(&object1, Inner(2));
        property.set(&object2, Inner(20));
        
        // reverse order on purpose (to exhibit memory corruptions)
        REQUIRE(property.get(object1).to<Inner>().x ==  2);
        REQUIRE(object1.prop.x == 2);
        REQUIRE(property.get(object2).to<Inner>().x == 20);
        REQUIRE(object2.prop.x == 20);
    }
    
    SECTION("user properties wrap user objects")
    {
        MyObject object1(11);
        property.set(&object1, Inner(22));
        REQUIRE(object1.prop.x == 22);
        
        ponder::UserObject uobj(&object1);
        REQUIRE((uobj.cref<MyObject>().prop.x == object1.prop.x));
        
        Inner mt(uobj.get("prop3").to<Inner>());
        REQUIRE(mt.x == 22);
    }
    
    SECTION("can get/set values on created objects")
    {
        const ponder::Class& metaclass = ponder::classByType<MyObject>();
        auto uobj = ponder::runtime::create(metaclass, 77);
        REQUIRE(uobj.get("prop3").to<Inner>().x == 77);
        
        Inner inr(-83);
        uobj.set("prop3", inr);
        REQUIRE(inr.x == -83);
        REQUIRE(uobj.get("prop3").to<Inner>().x == -83);
        
        ponder::Value cv = uobj.get("prop3");
        
        Inner inr2 = uobj.get("prop3").to<Inner>();
        REQUIRE(inr2.x == -83);
    }
}



