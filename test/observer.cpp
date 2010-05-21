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


#define BOOST_TEST_NO_LIB
#include <boost/test/unit_test.hpp>

#include "observer.hpp"

using namespace ObserverTest;

//-----------------------------------------------------------------------------
struct ObserverFixture
{
    ObserverFixture()
    {
        camp::addObserver(&observer);
    }

    ~ObserverFixture()
    {
        camp::removeObserver(&observer);
    }

    MyObserver observer;
};


//-----------------------------------------------------------------------------
//                         Tests for camp::Observer
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(OBSERVER, ObserverFixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(classes)
{
    BOOST_CHECK_EQUAL(observer.lastClassAdded, "");

    camp::Class::declare<MyClass>("MyClass");
    BOOST_CHECK_EQUAL(observer.lastClassAdded, "MyClass");

    camp::Class::undeclare<MyClass>();
    BOOST_CHECK_EQUAL(observer.lastClassRemoved, "MyClass");
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(enums)
{
    BOOST_CHECK_EQUAL(observer.lastEnumAdded, "");

    camp::Enum::declare<MyEnum>("MyEnum");
    BOOST_CHECK_EQUAL(observer.lastEnumAdded, "MyEnum");

    camp::Enum::undeclare<MyEnum>();
    BOOST_CHECK_EQUAL(observer.lastEnumRemoved, "MyEnum");
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(removeObserver)
{
    MyObserver observer2;
    camp::addObserver(&observer2);

    camp::Class::declare<MyClass>("MyClass");
    BOOST_CHECK_EQUAL(observer2.lastClassAdded, "MyClass");

    camp::removeObserver(&observer2);

    camp::Class::undeclare<MyClass>();
    BOOST_CHECK_EQUAL(observer2.lastClassRemoved, "");
}

BOOST_AUTO_TEST_SUITE_END()
