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

#include "userproperty.hpp"
#include <ponder/classget.hpp>
#include <ponder/userproperty.hpp>
#include "catch.hpp"

using namespace UserPropertyTest;

struct UserPropertyFixture
{
    UserPropertyFixture()
    {
        const ponder::Class& metaclass = ponder::classByType<MyClass>();
        property = static_cast<const ponder::UserProperty*>(&metaclass.property("prop"));
    }

    const ponder::UserProperty* property;
};

//-----------------------------------------------------------------------------
//                         Tests for ponder::UserProperty
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(USERPROPERTY, UserPropertyFixture)

BOOST_AUTO_TEST_CASE(type)
{
    BOOST_CHECK_EQUAL(property->type(), ponder::userType);
}

BOOST_AUTO_TEST_CASE(getClass)
{
    BOOST_CHECK(property->getClass() == ponder::classByType<MyType>());
}

BOOST_AUTO_TEST_CASE(get)
{
    BOOST_CHECK_EQUAL(property->get(MyClass(-1)).to<MyType>().x, -1);
    BOOST_CHECK_EQUAL(property->get(MyClass(20)).to<MyType>().x, 20);
}

BOOST_AUTO_TEST_CASE(set)
{
    MyClass object1(1);
    MyClass object2(10);
    property->set(object1, MyType(2));
    property->set(object2, MyType(20));

    // reverse order on purpose (to exhibit memory corruptions)
    BOOST_CHECK_EQUAL(property->get(object2).to<MyType>().x, 20);
    BOOST_CHECK_EQUAL(property->get(object1).to<MyType>().x, 2);
}

BOOST_AUTO_TEST_SUITE_END()
