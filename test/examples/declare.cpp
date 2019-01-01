/****************************************************************************
 **
 ** This file is part of the Ponder library, formerly CAMP.
 **
 ** The MIT License (MIT)
 **
 ** Copyright (C) 2015-2019 Nick Trout.
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

#include "test.hpp"

//! [eg_declare]

#include <ponder/classbuilder.hpp>
#include <ponder/uses/runtime.hpp>
#include <iostream>

//! [eg_declare_template1]

// Templated class.
template <typename T>
struct Data
{
    typedef T Type;
    Data(Type v) : m_value(v) {}
    Type m_value;
};

template <typename T>
static void declare_Data()
{
    // Declare type Data<T>
    ponder::Class::declare<Data<T>>()
        .template constructor<T>()
        .property("value", &Data<T>::m_value)
        ;
}

// Register the types with Ponder
PONDER_TYPE(Data<int>);
PONDER_TYPE(Data<float>);
PONDER_TYPE(Data<double>);

void declare()
{
    // Declare the type instances
    declare_Data<int>();
    declare_Data<float>();
    declare_Data<double>();
}

//! [eg_declare_template1]

//! [eg_declare]

TEST_CASE("declare")
{
    declare();
}

