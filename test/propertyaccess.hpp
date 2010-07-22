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

#ifndef CAMPTEST_PROPERTYACCESS_HPP
#define CAMPTEST_PROPERTYACCESS_HPP

#include <camp/camptype.hpp>
#include <camp/class.hpp>

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
        camp::Class::declare<MyClass>("PropertyAccessTest::MyClass")

            // ***** constant value *****
            .property("p0", &MyClass::p).readable(false).writable(true)
            .property("p1", &MyClass::p).readable(true).writable(false)
            .property("p2", &MyClass::p).readable(false).writable(false)

            // ***** function *****
            .property("p3", &MyClass::p).readable(&MyClass::b1)
            .property("p4", &MyClass::p).readable(&MyClass::b2)
            .property("p5", &MyClass::p).readable(boost::bind(&MyClass::b1, _1))
            .property("p6", &MyClass::p).readable(&MyClass::m_b)
            .property("p7", &MyClass::p).readable(boost::function<bool (MyClass&)>(&MyClass::m_b))

            // ***** implicit - based on the availability of a getter/setter *****
            .property("p8",  &MyClass::get)
            .property("p9",  &MyClass::ref)
            .property("p10", &MyClass::get, &MyClass::set)
            ;
    }
}

CAMP_AUTO_TYPE(PropertyAccessTest::MyClass, &PropertyAccessTest::declare)

#endif // CAMPTEST_PROPERTYACCESS_HPP
