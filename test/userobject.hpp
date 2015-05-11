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

#ifndef CAMPTEST_USEROBJECT_HPP
#define CAMPTEST_USEROBJECT_HPP

#include <camp/camptype.hpp>
#include <camp/class.hpp>
#include <camp/classbuilder.hpp>
#include <ostream>

namespace UserObjectTest
{
    struct MyBaseWithPadding
    {
        virtual ~MyBaseWithPadding() {}
        char padding[15];
    };

    struct MyBase
    {
        MyBase(int b_) : b(b_) {}
        virtual ~MyBase() {}
        int b;
        CAMP_RTTI();
    };

    bool operator==(const MyBase& left, const MyBase& right) 
    {
        return left.b == right.b;
    }

    std::ostream& operator<<(std::ostream& stream, const MyBase& b)
    {
        return stream << b.b;
    }

    struct MyClass : MyBaseWithPadding, MyBase
    {
        MyClass(int x_) : MyBase(x_ + 1), x(x_) {}
        int x;
        int f() {return x;}
        CAMP_RTTI();
    };

    bool operator==(const MyClass& left, const MyClass& right) 
    {
        return left.x == right.x;
    }

    std::ostream& operator<<(std::ostream& stream, const MyClass& m)
    {
        return stream << m.x;
    }

    struct MyNonCopyableClass : camp::noncopyable
    {
    };

    struct MyAbstractClass
    {
        virtual ~MyAbstractClass() {}
        virtual void f() = 0;
        CAMP_RTTI();
    };

    struct MyConcreteClass : MyAbstractClass
    {
        virtual void f() {}
        CAMP_RTTI();
    };

    struct Composed3
    {
        Composed3(int x_ = -1) : x(x_) {}
        int x;
    };

    struct Composed2
    {
        Composed3 get() const {return composed;}
        void set(Composed3 c) {composed = c;}
        Composed3 composed;
    };

    struct Composed1
    {
        Composed2 get() const {return composed;}
        void set(Composed2 c) {composed = c;}
        Composed2 composed;
    };

    void declare()
    {
        camp::Class::declare<MyBase>("UserObjectTest::MyBase");
        camp::Class::declare<MyClass>("UserObjectTest::MyClass")
            .base<MyBase>()
            .property("p", &MyClass::x)
            .function("f", &MyClass::f);

        camp::Class::declare<MyNonCopyableClass>("UserObjectTest::MyNonCopyableClass");

        camp::Class::declare<MyAbstractClass>("UserObjectTest::MyAbstractClass");
        camp::Class::declare<MyConcreteClass>("UserObjectTest::MyConcreteClass")
            .base<MyAbstractClass>();

        camp::Class::declare<Composed3>("UserObjectTest::Composed3")
            .property("x", &Composed3::x);
        camp::Class::declare<Composed2>("UserObjectTest::Composed2")
            .property("p", &Composed2::get, &Composed2::set);
        camp::Class::declare<Composed1>("UserObjectTest::Composed1")
            .property("p", &Composed1::get, &Composed1::set);
    }
}

CAMP_AUTO_TYPE(UserObjectTest::MyBase, &UserObjectTest::declare)
CAMP_AUTO_TYPE(UserObjectTest::MyClass, &UserObjectTest::declare)
CAMP_AUTO_TYPE_NONCOPYABLE(UserObjectTest::MyNonCopyableClass, &UserObjectTest::declare)
CAMP_AUTO_TYPE_NONCOPYABLE(UserObjectTest::MyAbstractClass, &UserObjectTest::declare)
CAMP_AUTO_TYPE_NONCOPYABLE(UserObjectTest::MyConcreteClass, &UserObjectTest::declare)
CAMP_AUTO_TYPE(UserObjectTest::Composed3, &UserObjectTest::declare)
CAMP_AUTO_TYPE(UserObjectTest::Composed2, &UserObjectTest::declare)
CAMP_AUTO_TYPE(UserObjectTest::Composed1, &UserObjectTest::declare)

#endif // CAMPTEST_USEROBJECT_HPP
