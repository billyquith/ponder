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
#include <camp/class.hpp>
#include <camp/classget.hpp>
#include <camp/userproperty.hpp>
#include <boost/test/unit_test.hpp>

using namespace UserPropertyTest;

//-----------------------------------------------------------------------------
struct UserPropertyFixture
{
    UserPropertyFixture()
    {
        camp::Class::declare<MyType>("MyType");

        camp::Class::declare<MyClass>("MyClass")
            .property("prop", &MyClass::prop);
    }

    ~UserPropertyFixture()
    {
        camp::Class::undeclare<MyClass>();
        camp::Class::undeclare<MyType>();
    }
};


//-----------------------------------------------------------------------------
//                         Tests for camp::UserProperty
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(USERPROPERTY, UserPropertyFixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(xxx)
{
}

BOOST_AUTO_TEST_SUITE_END()



/*
    camp::Class::declare<UserPropTest>("UserPropTest")
        .property("c1", &UserPropTest::c1)
        .property("c2", &UserPropTest::c2)
        .property("c3", &UserPropTest::c3)
        .property("c4", &UserPropTest::getC4, &UserPropTest::setC4)
        ;

    UserPropTest obj;
    const camp::Class& c = camp::classByType<UserPropTest>();
    const camp::UserProperty& c1 = static_cast<const camp::UserProperty&>(c.property("c1"));
    const camp::UserProperty& c2 = static_cast<const camp::UserProperty&>(c.property("c2"));
    const camp::UserProperty& c3 = static_cast<const camp::UserProperty&>(c.property("c3"));
    const camp::UserProperty& c4 = static_cast<const camp::UserProperty&>(c.property("c4"));

    // ***** type *****
    BOOST_CHECK_EQUAL(c1.type(), camp::userType);
    BOOST_CHECK_EQUAL(c2.type(), camp::userType);
    BOOST_CHECK_EQUAL(c3.type(), camp::userType);
    BOOST_CHECK_EQUAL(c4.type(), camp::userType);

    // ***** getClass *****
    BOOST_CHECK_EQUAL(c1.getClass() == camp::classByType<Comparable>(), true);
    BOOST_CHECK_EQUAL(c2.getClass() == camp::classByType<Comparable>(), true);
    BOOST_CHECK_EQUAL(c3.getClass() == camp::classByType<Comparable>(), true);
    BOOST_CHECK_EQUAL(c4.getClass() == camp::classByType<Comparable>(), true);

    // ***** get *****
    BOOST_CHECK_EQUAL(c1.get(obj).to<Comparable>(), Comparable(10));
    BOOST_CHECK_EQUAL(c2.get(obj).to<Comparable>(), Comparable(20));
    BOOST_CHECK_EQUAL(c3.get(obj).to<Comparable>(), Comparable(30));
    BOOST_CHECK_EQUAL(c4.get(obj).to<Comparable>(), Comparable(40));

    // ***** set *****
    c1.set(obj, Comparable(100));
    c2.set(obj, Comparable(200));
    c3.set(obj, Comparable(300));
    c4.set(obj, Comparable(400));
    camp::Value v4 = c4.get(obj); // reverse order on purpose (to exhibit memory corruptions)
    camp::Value v3 = c3.get(obj);
    camp::Value v2 = c2.get(obj);
    camp::Value v1 = c1.get(obj);
    BOOST_CHECK_EQUAL(v1.to<Comparable>(), Comparable(100));
    BOOST_CHECK_EQUAL(v2.to<Comparable>(), Comparable(200));
    BOOST_CHECK_EQUAL(v3.to<Comparable>(), Comparable(300));
    BOOST_CHECK_EQUAL(v4.to<Comparable>(), Comparable(400));
*/