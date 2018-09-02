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
    };
    
    static void declare()
    {
        ponder::Class::declare<UserPropertyTest::MyType>("UserPropertyTest::MyType");
        
        ponder::Class::declare<UserPropertyTest::MyClass>("UserPropertyTest::MyClass")
            .property("prop", &MyClass::prop);
    }
}

PONDER_AUTO_TYPE(UserPropertyTest::MyType, &UserPropertyTest::declare)
PONDER_AUTO_TYPE(UserPropertyTest::MyClass, &UserPropertyTest::declare)

//-----------------------------------------------------------------------------
//                         Tests for ponder::UserProperty
//-----------------------------------------------------------------------------

TEST_CASE("Ponder has user properties")
{
    const ponder::Class& metaclass = ponder::classByType<UserPropertyTest::MyClass>();
    
    const ponder::Property* property = &metaclass.property("prop");
    
    IS_TRUE(property->kind() == ponder::ValueKind::User);
    
    IS_TRUE(static_cast<const ponder::UserProperty*>(property)->getClass()
            == ponder::classByType<UserPropertyTest::MyType>());
    
    SECTION("propery values can be got")
    {
        REQUIRE(property->get(UserPropertyTest::MyClass(-1)).to<UserPropertyTest::MyType>().x == -1);
        REQUIRE(property->get(UserPropertyTest::MyClass(20)).to<UserPropertyTest::MyType>().x == 20);
    }
    
    SECTION("property values can be set")
    {
        UserPropertyTest::MyClass object1(1);
        UserPropertyTest::MyClass object2(10);
        property->set(&object1, UserPropertyTest::MyType(2));
        property->set(&object2, UserPropertyTest::MyType(20));
        
        // reverse order on purpose (to exhibit memory corruptions)
        REQUIRE(property->get(object2).to<UserPropertyTest::MyType>().x == 20);
        REQUIRE(property->get(object1).to<UserPropertyTest::MyType>().x ==  2);
    }
    
    SECTION("user properties wrap user objects")
    {
        UserPropertyTest::MyClass object1(11);
        property->set(&object1, UserPropertyTest::MyType(22));

        auto uobj = static_cast<const ponder::UserProperty*>(property)->getObject(object1);
        
        IS_TRUE(uobj.cref<UserPropertyTest::MyClass>().prop.x == object1.prop.x);
    }
}



