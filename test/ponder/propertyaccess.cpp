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
#include <ponder/property.hpp>
#include <ponder/class.hpp>
#include <ponder/classbuilder.hpp>
#include "test.hpp"

namespace PropertyAccessTest
{
    struct MyClass
    {
        MyClass(bool b = true)
        : m_b(b)
        {
        }
        
        void set(int x) {p = x;}
        int get() const {return p;}
        int& ref() {return p;}
        int p;
        
        bool m_b;
        bool b1() {return true;}
        bool b2() const {return false;}
    };
    
    void declare()
    {
        using namespace std::placeholders;
        
        ponder::Class::declare<MyClass>("PropertyAccessTest::MyClass")
        
            // ***** implicit - based on the availability of a getter/setter *****
            .property("p8",  &MyClass::get)
            .property("p9",  &MyClass::ref)
            .property("p10", &MyClass::get, &MyClass::set)
            ;
    }
}

PONDER_AUTO_TYPE(PropertyAccessTest::MyClass, &PropertyAccessTest::declare)

using namespace PropertyAccessTest;

//-----------------------------------------------------------------------------
//                  Tests for ponder::Property readable/writable
//-----------------------------------------------------------------------------

TEST_CASE("Properties can be accessed")
{
    MyClass object_t(true);
    MyClass object_f(false);
    
    const ponder::Class* metaclass = &ponder::classByType<MyClass>();
    
    SECTION("readableImplicit")
    {
        REQUIRE(metaclass->property("p8").isReadable() == true);
        REQUIRE(metaclass->property("p8").isReadable() == true);
        REQUIRE(metaclass->property("p9").isReadable() == true);
        REQUIRE(metaclass->property("p9").isReadable() == true);
        REQUIRE(metaclass->property("p10").isReadable() == true);
        REQUIRE(metaclass->property("p10").isReadable() == true);
    }

    SECTION("writableImplicit")
    {
        REQUIRE(metaclass->property("p8").isWritable() == false);
        REQUIRE(metaclass->property("p8").isWritable() == false);
        REQUIRE(metaclass->property("p9").isWritable() == true);
        REQUIRE(metaclass->property("p9").isWritable() == true);
        REQUIRE(metaclass->property("p10").isWritable() == true);
        REQUIRE(metaclass->property("p10").isWritable() == true);
    }
}

