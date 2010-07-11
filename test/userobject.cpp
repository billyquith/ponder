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

#include "userobject.hpp"
#include <camp/classget.hpp>
#include <camp/errors.hpp>
#include <camp/userobject.hpp>
#include <boost/test/unit_test.hpp>

using namespace UserObjectTest;

//-----------------------------------------------------------------------------
//                         Tests for camp::UserObject
//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(USEROBJECT)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(getObject)
{
    MyClass object(1);
    camp::UserObject userObject(object);

    BOOST_CHECK_EQUAL(userObject.get<MyClass>(),         object);
    BOOST_CHECK_EQUAL(&userObject.get<MyClass>(),        &object);
    BOOST_CHECK_EQUAL(userObject.get<MyClass&>(),        object);
    BOOST_CHECK_EQUAL(&userObject.get<MyClass&>(),       &object);
    BOOST_CHECK_EQUAL(userObject.get<const MyClass&>(),  object);
    BOOST_CHECK_EQUAL(&userObject.get<const MyClass&>(), &object);
    BOOST_CHECK_EQUAL(userObject.get<MyClass*>(),        &object);
    BOOST_CHECK_EQUAL(userObject.get<const MyClass*>(),  &object);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(getAsBase)
{
    MyClass object(2);
    camp::UserObject userObject(object);
    MyBase& base = object;

    BOOST_CHECK_EQUAL(userObject.get<MyBase>(),         base);
    BOOST_CHECK_EQUAL(&userObject.get<MyBase>(),        &base);
    BOOST_CHECK_EQUAL(userObject.get<MyBase&>(),        base);
    BOOST_CHECK_EQUAL(&userObject.get<MyBase&>(),       &base);
    BOOST_CHECK_EQUAL(userObject.get<const MyBase&>(),  base);
    BOOST_CHECK_EQUAL(&userObject.get<const MyBase&>(), &base);
    BOOST_CHECK_EQUAL(userObject.get<MyBase*>(),        &base);
    BOOST_CHECK_EQUAL(userObject.get<const MyBase*>(),  &base);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(getFromPointer)
{
    MyClass object(3);
    camp::UserObject userObject(&object);

    BOOST_CHECK_EQUAL(userObject.get<MyClass>(),         object);
    BOOST_CHECK_EQUAL(&userObject.get<MyClass>(),        &object);
    BOOST_CHECK_EQUAL(userObject.get<MyClass&>(),        object);
    BOOST_CHECK_EQUAL(&userObject.get<MyClass&>(),       &object);
    BOOST_CHECK_EQUAL(userObject.get<const MyClass&>(),  object);
    BOOST_CHECK_EQUAL(&userObject.get<const MyClass&>(), &object);
    BOOST_CHECK_EQUAL(userObject.get<MyClass*>(),        &object);
    BOOST_CHECK_EQUAL(userObject.get<const MyClass*>(),  &object);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(copy)
{
    MyClass object(4);
    camp::UserObject userObject = camp::UserObject::copy(object);

    BOOST_CHECK_EQUAL(userObject.get<MyClass>(), object);  // same value
    BOOST_CHECK_NE(userObject.get<MyClass*>(),   &object); // different address
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(ref)
{
    MyClass object(5);
    camp::UserObject userObject = camp::UserObject::ref(object);

    BOOST_CHECK_EQUAL(userObject.get<MyClass>(),  object);  // same value
    BOOST_CHECK_EQUAL(userObject.get<MyClass*>(), &object); // same address
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(getClass)
{
    MyClass object(6);
    MyBase base(6);
    MyBase& objectAsBase = object;

    BOOST_CHECK(camp::UserObject(base).getClass() == camp::classByType<MyBase>());
    BOOST_CHECK(camp::UserObject(object).getClass() == camp::classByType<MyClass>());
    BOOST_CHECK(camp::UserObject(objectAsBase).getClass() == camp::classByType<MyClass>());
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(get)
{
    MyClass object(7);
    camp::UserObject userObject(object);

    BOOST_CHECK_EQUAL(userObject.get("p"), camp::Value(7));
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(set)
{
    MyClass object(0);
    camp::UserObject userObject(object);
    userObject.set("p", 8);

    BOOST_CHECK_EQUAL(object.x, 8);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(call)
{
    MyClass object(9);
    camp::UserObject userObject(object);

    BOOST_CHECK_EQUAL(userObject.call("f"), camp::Value(9));
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(assignment)
{
    MyClass object1(10);
    camp::UserObject userObject1(object1);
    camp::UserObject userObject2;
    userObject2 = userObject1;

    BOOST_CHECK(userObject1.get<MyClass*>() == userObject2.get<MyClass*>());
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(equality)
{
    MyClass object1(11);
    MyClass object2(11);

    BOOST_CHECK_EQUAL(camp::UserObject(object1)  == camp::UserObject(object1),  true);
    BOOST_CHECK_EQUAL(camp::UserObject(object1)  == camp::UserObject(&object1), true);
    BOOST_CHECK_EQUAL(camp::UserObject(&object1) == camp::UserObject(&object1), true);
    BOOST_CHECK_EQUAL(camp::UserObject(object1)  == camp::UserObject(object2),  false);

    BOOST_CHECK_EQUAL(camp::UserObject(object1) == camp::UserObject::ref(object1),   true);
    BOOST_CHECK_EQUAL(camp::UserObject(object1) == camp::UserObject::ref(&object1),  true);
    BOOST_CHECK_EQUAL(camp::UserObject(object1) == camp::UserObject::copy(object1),  false);
    BOOST_CHECK_EQUAL(camp::UserObject(object1) == camp::UserObject::copy(&object1), false);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(nonCopyableClass)
{
    // This is a compile check
    MyNonCopyableClass object;
    camp::UserObject userObject(object);
    MyNonCopyableClass& ref = userObject.get<MyNonCopyableClass>();
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(abstractClass)
{
    // This is a compile check
    MyConcreteClass object;

    camp::UserObject userObject1(object);
    userObject1.get<MyAbstractClass>();

    camp::UserObject userObject2(static_cast<MyAbstractClass&>(object));
    userObject2.get<MyConcreteClass>();
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(composedObjects)
{
    const camp::UserProperty& p1 = static_cast<const camp::UserProperty&>(camp::classByType<Composed1>().property("p"));
    const camp::UserProperty& p2 = static_cast<const camp::UserProperty&>(camp::classByType<Composed2>().property("p"));

    Composed1 composed1;
    camp::UserObject composed2(composed1, p1);
    camp::UserObject composed3(composed2, p2);

    composed3.set("x", 1);
    BOOST_CHECK_EQUAL(composed1.get().get().x, 1);
    BOOST_CHECK_EQUAL(composed2.get<Composed2>().get().x, 1);
    BOOST_CHECK_EQUAL(composed3.get<Composed3>().x, 1);

    composed3.set("x", 2);
    BOOST_CHECK_EQUAL(composed1.get().get().x, 2);
    BOOST_CHECK_EQUAL(composed2.get<Composed2>().get().x, 2);
    BOOST_CHECK_EQUAL(composed3.get<Composed3>().x, 2);
}

BOOST_AUTO_TEST_SUITE_END()
