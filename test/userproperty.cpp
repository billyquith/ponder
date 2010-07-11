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

#include "userproperty.hpp"
#include <camp/classget.hpp>
#include <camp/userproperty.hpp>
#include <boost/test/unit_test.hpp>

using namespace UserPropertyTest;

//-----------------------------------------------------------------------------
struct UserPropertyFixture
{
    UserPropertyFixture()
    {
        const camp::Class& metaclass = camp::classByType<MyClass>();
        property = static_cast<const camp::UserProperty*>(&metaclass.property("prop"));
    }

    const camp::UserProperty* property;
};

//-----------------------------------------------------------------------------
//                         Tests for camp::UserProperty
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(USERPROPERTY, UserPropertyFixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(type)
{
    BOOST_CHECK_EQUAL(property->type(), camp::userType);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(getClass)
{
    BOOST_CHECK(property->getClass() == camp::classByType<MyType>());
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(get)
{
    BOOST_CHECK_EQUAL(property->get(MyClass(-1)).to<MyType>().x, -1);
    BOOST_CHECK_EQUAL(property->get(MyClass(20)).to<MyType>().x, 20);
}

//-----------------------------------------------------------------------------
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
