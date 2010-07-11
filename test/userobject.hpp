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

#ifndef CAMPTEST_USEROBJECT_HPP
#define CAMPTEST_USEROBJECT_HPP

#include <camp/camptype.hpp>
#include <camp/class.hpp>
#include <boost/noncopyable.hpp>
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

    struct MyNonCopyableClass : boost::noncopyable
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

CAMP_AUTO_TYPE(UserObjectTest::MyBase, &UserObjectTest::declare);
CAMP_AUTO_TYPE(UserObjectTest::MyClass, &UserObjectTest::declare);
CAMP_AUTO_TYPE_NONCOPYABLE(UserObjectTest::MyNonCopyableClass, &UserObjectTest::declare);
CAMP_AUTO_TYPE_NONCOPYABLE(UserObjectTest::MyAbstractClass, &UserObjectTest::declare);
CAMP_AUTO_TYPE_NONCOPYABLE(UserObjectTest::MyConcreteClass, &UserObjectTest::declare);
CAMP_AUTO_TYPE(UserObjectTest::Composed3, &UserObjectTest::declare);
CAMP_AUTO_TYPE(UserObjectTest::Composed2, &UserObjectTest::declare);
CAMP_AUTO_TYPE(UserObjectTest::Composed1, &UserObjectTest::declare);

#endif // CAMPTEST_USEROBJECT_HPP
