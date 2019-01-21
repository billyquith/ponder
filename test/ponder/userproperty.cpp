/****************************************************************************
**
** This file is part of the CAMP library.
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
#include <ponder/userproperty.hpp>
#include <ponder/classbuilder.hpp>
#include "test.hpp"


namespace UserPropertyTest
{
    struct MyType
    {
        MyType(int x_) : x(x_) {}
        int x;
    };
    
    struct MyClass
    {
        MyClass(int x) : prop(x) {}
        MyType prop;
        
        const MyType& get() const {return prop;}
        void set(const MyType& p) {prop = p;}
    };
    
    static void declare()
    {
        ponder::Class::declare<UserPropertyTest::MyType>();
        
        ponder::Class::declare<UserPropertyTest::MyClass>()
            .property("prop", &MyClass::prop)
            .property("prop2", &MyClass::get, &MyClass::set)
            ;
    }
}

PONDER_AUTO_TYPE(UserPropertyTest::MyType, &UserPropertyTest::declare)
PONDER_AUTO_TYPE(UserPropertyTest::MyClass, &UserPropertyTest::declare)

//-----------------------------------------------------------------------------
//                         Tests for ponder::UserProperty
//-----------------------------------------------------------------------------

using namespace UserPropertyTest;

TEST_CASE("Ponder has user properties (member variable)")
{
    const ponder::Class& metaclass = ponder::classByType<MyClass>();
    
    const ponder::Property* property = &metaclass.property("prop");
    
    IS_TRUE(property->kind() == ponder::ValueKind::User);
    
    IS_TRUE(static_cast<const ponder::UserProperty*>(property)->getClass()
            == ponder::classByType<MyType>());
    
    SECTION("propery values can be got")
    {
        REQUIRE(property->get(MyClass(-1)).to<MyType>().x == -1);
        REQUIRE(property->get(MyClass(20)).to<MyType>().x == 20);
    }
    
    SECTION("property values can be set")
    {
        MyClass object1(1);
        MyClass object2(10);
        property->set(&object1, MyType(2));
        property->set(&object2, MyType(20));
        
        // reverse order on purpose (to exhibit memory corruptions)
        REQUIRE(property->get(object1).to<MyType>().x ==  2);
        REQUIRE(object1.prop.x == 2);
        REQUIRE(property->get(object2).to<MyType>().x == 20);
        REQUIRE(object2.prop.x == 20);
    }
    
    SECTION("user properties wrap user objects")
    {
        MyClass object1(11);
        property->set(&object1, MyType(22));

        ponder::UserObject uobj(&object1);
        REQUIRE((uobj.cref<MyClass>().prop.x == object1.prop.x));
    }
}

TEST_CASE("Ponder has user properties (accessors)")
{
    const ponder::Class& metaclass = ponder::classByType<MyClass>();
    const ponder::Property& property = metaclass.property("prop2");
    
    IS_TRUE(property.kind() == ponder::ValueKind::User);

    // TODO: investigate
////    IS_TRUE(static_cast<const ponder::UserProperty&>(property).getClass()
//            == ponder::classByType<MyType>());
    
    SECTION("propery values can be got")
    {
        REQUIRE(property.get(MyClass(-1)).to<MyType>().x == -1);
        REQUIRE(property.get(MyClass(20)).to<MyType>().x == 20);
    }
    
    SECTION("property values can be set")
    {
        MyClass object1(1);
        MyClass object2(10);
        property.set(&object1, MyType(2));
        property.set(&object2, MyType(20));
        
        // reverse order on purpose (to exhibit memory corruptions)
        REQUIRE(property.get(object1).to<MyType>().x ==  2);
        REQUIRE(object1.prop.x == 2);
        REQUIRE(property.get(object2).to<MyType>().x == 20);
        REQUIRE(object2.prop.x == 20);
    }
    
    SECTION("user properties wrap user objects")
    {
        MyClass object1(11);
        property.set(&object1, MyType(22));
        REQUIRE(object1.prop.x == 22);

        ponder::UserObject uobj(&object1);
        REQUIRE((uobj.cref<MyClass>().prop.x == object1.prop.x));
        
        MyType mt(uobj.get("prop2").to<MyType>());
        REQUIRE(mt.x == 22);
    }
}



