/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
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

#include <ponder/classget.hpp>
#include <ponder/function.hpp>
#include <ponder/pondertype.hpp>
#include <ponder/class.hpp>
#include <ponder/classbuilder.hpp>
#include "catch.hpp"
#include <functional>

namespace FunctionAccessTest
{
    struct MyClass
    {
        MyClass(bool b = true)
            : m_b(b)
        {}
        
        void f() {}
        
        bool m_b;
        bool b1() {return true;}
        bool b2() const {return false;}
    };
    
    void declare()
    {
        ponder::Class::declare<MyClass>("FunctionAccessTest::MyClass")
        
            // ***** constant value *****
            .function("f0", &MyClass::f).callable(false)
            .function("f1", &MyClass::f).callable(true)
        
            // ***** function *****
            .function("f2", &MyClass::f).callable(&MyClass::b1)
            .function("f3", &MyClass::f).callable(&MyClass::b2)
            .function("f4", &MyClass::f).callable(std::bind(&MyClass::b1, std::placeholders::_1))
            .function("f5", &MyClass::f).callable(&MyClass::m_b)
            .function("f6", &MyClass::f).callable(std::function<bool (MyClass&)>(&MyClass::m_b))
            ;
    }
}

PONDER_AUTO_TYPE(FunctionAccessTest::MyClass, &FunctionAccessTest::declare);

using namespace FunctionAccessTest;

//                         Tests for ponder::Function callable

TEST_CASE("Functions can be inspected")
{
    MyClass object_t(true);
    MyClass object_f(false);
    
    const ponder::Class* metaclass = &ponder::classByType<MyClass>();;
    
    SECTION("callable can be constant")
    {
        REQUIRE(metaclass->function("f0").callable(object_t) == false);
        REQUIRE(metaclass->function("f0").callable(object_f) == false);
        
        REQUIRE(metaclass->function("f1").callable(object_t) == true);
        REQUIRE(metaclass->function("f1").callable(object_f) == true);
    }
        
    SECTION("callable can be dynamic, bound to function")
    {
        REQUIRE(metaclass->function("f2").callable(object_t) == true);
        REQUIRE(metaclass->function("f2").callable(object_f) == true);
        
        REQUIRE(metaclass->function("f3").callable(object_t) == false);
        REQUIRE(metaclass->function("f3").callable(object_f) == false);
        
        REQUIRE(metaclass->function("f4").callable(object_t) == true);
        REQUIRE(metaclass->function("f4").callable(object_f) == true);
    }    

    SECTION("callable can be dynamic, bound to variable")
    {
        REQUIRE(metaclass->function("f5").callable(object_t) == true);
        REQUIRE(metaclass->function("f5").callable(object_f) == false);
        
        REQUIRE(metaclass->function("f6").callable(object_t) == true);
        REQUIRE(metaclass->function("f6").callable(object_f) == false);
    }
}
