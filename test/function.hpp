/****************************************************************************
**
** This file is part of the CAMP library.
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

#ifndef CAMPTEST_FUNCTION_HPP
#define CAMPTEST_FUNCTION_HPP

#include <camp/camptype.hpp>
#include <camp/enum.hpp>
#include <camp/class.hpp>
#include <camp/value.hpp>
#include <string>

namespace FunctionTest
{
    using namespace std::placeholders;
    
    enum MyEnum
    {
        Zero = 0,
        One  = 1,
        Two  = 2
    };

    struct MyType
    {
        MyType(int x_) : x(x_) {}
        int x;
    };

    bool operator==(const MyType& left, const MyType& right) {return left.x == right.x;}
    bool operator<(const MyType& left, const MyType& right) {return left.x < right.x;}
    std::ostream& operator<<(std::ostream& stream, const MyType& object) {return stream << object.x;}

    struct MyBase
    {
        void f6() {}
        char padding[10];
    };

    struct MyClass : MyBase
    {
        MyClass()
            : p1(true)
            , p2(2)
            , p3("3")
            , p4(MyType(4))
            , p5(MyType(5))
            , innerPtr(&inner)
            , innerSmartPtr(new Inner)
        {
        }

        bool p1;
        int p2;
        std::string p3;

        MyType p4; const MyType& f4() {return p4;}
        MyType p5; const MyType& f5() const {return p5;}
        // f6 is inherited
        camp::Value f7(camp::Value v) {return v;}

        void f8() {}
        void f9(bool) {}
        void f10(float, double) {}
        void f11(short, int, long) {}
        void f12(const std::string&, std::string, const std::string&, std::string) {}
        void f13(MyEnum, MyEnum, MyEnum, MyEnum, MyEnum) {}

        struct Inner
        {
            void f14() {}
            void f15() const {}
            int f16() {return 16;}
            void f17(int) {}
            void f18() {}
            void f19() {}
        };
        Inner inner;
        const Inner& getInner() const {return inner;}
        Inner* innerPtr;
        const Inner* getInnerPtr() const {return innerPtr;}
        std::shared_ptr<Inner> innerSmartPtr;
        const std::shared_ptr<Inner> getInnerSmartPtr() {return innerSmartPtr;}

        int f20(int x) {return x;}
        int f21(int x, int y) {return x + y;}
        int f22(int x, int y, int z) {return x + y + z;}
    };

    void f1(MyClass& object)
    {
        object.p1 = true;
    }

    int f2(MyClass object, int x)
    {
        return object.p2 + x;
    }

    const std::string& f3(const MyClass* object)
    {
        return object->p3;
    }

    void declare()
    {
        camp::Enum::declare<MyEnum>("FunctionTest::MyEnum")
            .value("Zero", Zero)
            .value("One",  One)
            .value("Two",  Two);

        camp::Class::declare<MyType>("FunctionTest::MyType");

        camp::Class::declare<MyBase>("FunctionTest::MyBase");

        camp::Class::declare<MyClass>("FunctionTest::MyClass")
            .base<MyBase>()

            // ***** non-member functions *****
            .function("f1", &f1) // object by reference
            .function("f2", &f2) // object by value + parameter
            .function("f3", &f3) // object by pointer

            // ***** member functions *****
            .function("f4", &MyClass::f4) // non-const
            .function("f5", &MyClass::f5) // const
            .function("f6", &MyClass::f6) // inherited
            .function("f7", &MyClass::f7) // camp::Value as return and argument types

            // ***** arguments count ******
            .function("f8",  &MyClass::f8)  // 0 argument
            .function("f9",  &MyClass::f9)  // 1 argument
            .function("f10", &MyClass::f10) // 2 arguments
            .function("f11", &MyClass::f11) // 3 arguments
            .function("f12", &MyClass::f12) // 4 arguments
            .function("f13", &MyClass::f13) // 5 arguments

            // ***** nested functions *****
            // TOFIX .function("f14", &MyClass::Inner::f14, &MyClass::inner)            // object
            .function("f15", &MyClass::Inner::f15, &MyClass::getInner)         // getter returning an object
            .function("f16", &MyClass::Inner::f16, &MyClass::innerPtr)         // raw pointer
            // TOFIX .function("f17", &MyClass::Inner::f17, &MyClass::getInnerPtr)      // getter returning a raw pointer
            .function("f18", &MyClass::Inner::f18, &MyClass::innerSmartPtr)    // smart pointer
            .function("f19", &MyClass::Inner::f19, &MyClass::getInnerSmartPtr) // getter returning a smart pointer

            // ***** std::function *****
            .function("f20", std::function<int (MyClass&, int)>(std::bind(&MyClass::f20, _1, _2)))
            .function("f21", std::function<int (MyClass&, int)>(std::bind(&MyClass::f21, _1, _2, 20)))
            .function("f22", std::function<int (MyClass&, int)>(std::bind(std::bind(&MyClass::f22, _1, _2, _3, 30), _1, _2, 20)))
            ;
    }
}

CAMP_AUTO_TYPE(FunctionTest::MyEnum,  &FunctionTest::declare)
CAMP_AUTO_TYPE(FunctionTest::MyType,  &FunctionTest::declare)
CAMP_AUTO_TYPE(FunctionTest::MyClass, &FunctionTest::declare)
CAMP_AUTO_TYPE(FunctionTest::MyBase,  &FunctionTest::declare)

#endif // CAMPTEST_FUNCTION_HPP
