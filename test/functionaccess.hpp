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

#ifndef CAMPTEST_FUNCTIONACCESS_HPP
#define CAMPTEST_FUNCTIONACCESS_HPP

#include <camp/camptype.hpp>
#include <camp/class.hpp>

namespace FunctionAccessTest
{
    struct MyClass
    {
        MyClass(bool b = true)
            : m_b(b)
        {
        }

        void f() {}

        bool m_b;
        bool b1() {return true;}
        bool b2() const {return false;}
    };

    void declare()
    {
        camp::Class::declare<MyClass>("FunctionAccessTest::MyClass")

            // ***** constant value *****
            .function("f0", &MyClass::f).callable(false)
            .function("f1", &MyClass::f).callable(true)

            // ***** function *****
            .function("f2", &MyClass::f).callable(&MyClass::b1)
            .function("f3", &MyClass::f).callable(&MyClass::b2)
            .function("f4", &MyClass::f).callable(boost::bind(&MyClass::b1, _1))
            .function("f5", &MyClass::f).callable(&MyClass::m_b)
            .function("f6", &MyClass::f).callable(boost::function<bool (MyClass&)>(&MyClass::m_b))
            ;
    }
}

CAMP_AUTO_TYPE(FunctionAccessTest::MyClass, &FunctionAccessTest::declare);

#endif // CAMPTEST_FUNCTIONACCESS_HPP
