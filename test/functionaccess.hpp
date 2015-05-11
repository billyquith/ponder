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

#ifndef CAMPTEST_FUNCTIONACCESS_HPP
#define CAMPTEST_FUNCTIONACCESS_HPP

#include <camp/camptype.hpp>
#include <camp/class.hpp>
#include <camp/classbuilder.hpp>

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
