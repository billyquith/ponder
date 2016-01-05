/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
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

#include "userobject.hpp"
#include <ponder/classget.hpp>
#include <ponder/userobject.hpp>
#include <boost/test/unit_test.hpp>

using namespace UserObjectTest;

//-----------------------------------------------------------------------------
//                         Tests for ponder::UserObject
//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(USEROBJECT)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(getObject)
{
    MyClass object(1);
    ponder::UserObject userObject(object);

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
    ponder::UserObject userObject(object);
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
    ponder::UserObject userObject(&object);

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
    ponder::UserObject userObject = ponder::UserObject::copy(object);

    BOOST_CHECK_EQUAL(userObject.get<MyClass>(), object);  // same value
    BOOST_CHECK_NE(userObject.get<MyClass*>(),   &object); // different address
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(ref)
{
    MyClass object(5);
    ponder::UserObject userObject = ponder::UserObject::ref(object);

    BOOST_CHECK_EQUAL(userObject.get<MyClass>(),  object);  // same value
    BOOST_CHECK_EQUAL(userObject.get<MyClass*>(), &object); // same address
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(getClass)
{
    MyClass object(6);
    MyBase base(6);
    MyBase& objectAsBase = object;

    BOOST_CHECK(ponder::UserObject(base).getClass() == ponder::classByType<MyBase>());
    BOOST_CHECK(ponder::UserObject(object).getClass() == ponder::classByType<MyClass>());
    BOOST_CHECK(ponder::UserObject(objectAsBase).getClass() == ponder::classByType<MyClass>());
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(get)
{
    MyClass object(7);
    ponder::UserObject userObject(object);

    BOOST_CHECK_EQUAL(userObject.get("p"), ponder::Value(7));
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(set)
{
    MyClass object(0);
    ponder::UserObject userObject(object);
    userObject.set("p", 8);

    BOOST_CHECK_EQUAL(object.x, 8);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(callBasic)
{
    MyClass object(9);
    ponder::UserObject userObject(object);

    BOOST_CHECK_EQUAL(userObject.call("f"), ponder::Value(9));
}

//----------------------------------------------------------------------------- ****
BOOST_AUTO_TEST_CASE(callMultiArgs1)
{
    Call object;
    ponder::UserObject userObject(object);

    BOOST_CHECK_THROW(userObject.call("meth1"), std::exception);
    
    userObject.call("meth1", ponder::Args(7));
    BOOST_CHECK_EQUAL(object.lastCalled, "meth1");
    BOOST_CHECK_EQUAL(object.sum, 7);
}

BOOST_AUTO_TEST_CASE(callMultiArgs2)
{
    Call object;
    ponder::UserObject userObject(object);
    
    BOOST_CHECK_THROW(userObject.call("meth2"), std::exception);
    BOOST_CHECK_THROW(userObject.call("meth2", ponder::Args(11)), std::exception);
//    BOOST_CHECK_THROW(userObject.call("meth2", ponder::Args(11,2,333)), std::exception);
    
    userObject.call("meth2", ponder::Args(7, 8));
    BOOST_CHECK_EQUAL(object.lastCalled, "meth2");
    BOOST_CHECK_EQUAL(object.sum, 7+8);
}

BOOST_AUTO_TEST_CASE(callMultiArgs3)
{
    Call object;
    ponder::UserObject userObject(object);
    
    BOOST_CHECK_THROW(userObject.call("meth3"), std::exception);
    
    userObject.call("meth3", ponder::Args(7, 8, -99));
    BOOST_CHECK_EQUAL(object.lastCalled, "meth3");
    BOOST_CHECK_EQUAL(object.sum, 7+8-99);
}

// todo: variadic template:
//BOOST_AUTO_TEST_CASE(callMultiArgs8)
//{
//    Call object;
//    ponder::UserObject userObject(object);
//    
//    BOOST_CHECK_THROW(userObject.call("meth8"), std::exception);
//    
//    userObject.call("meth8", ponder::Args(7, 8, -99, 77, 12, 76, 45, 3));
//    BOOST_CHECK_EQUAL(object.lastCalled, "meth8");
//    BOOST_CHECK_EQUAL(object.sum, 7+8-99+77-12+76+45+3);
//}
//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(callArgs1)
{
    Call object;
    ponder::UserObject userObject(object);
    
    BOOST_CHECK_THROW(userObject.call("cos"), std::exception);
    
    BOOST_CHECK_EQUAL(userObject.call("cos", ponder::Args(0.0)), ponder::Value(std::cos(0.0)));
//    BOOST_CHECK_EQUAL(userObject.call("cos", 0.0), ponder::Value(std::cos(0.0)));
}

BOOST_AUTO_TEST_CASE(callArgs2)
{
    Call object;
    ponder::UserObject userObject(object);
    
    BOOST_CHECK_THROW(userObject.call("concat"), std::exception);
    
    BOOST_CHECK_EQUAL(userObject.call("concat", ponder::Args("one", "two")), ponder::Value("onetwo"));
//    BOOST_CHECK_EQUAL(userObject.call("concat", "one", "two"), ponder::Value("onetwo"));
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(assignment)
{
    MyClass object1(10);
    ponder::UserObject userObject1(object1);
    ponder::UserObject userObject2;
    userObject2 = userObject1;

    BOOST_CHECK(userObject1.get<MyClass*>() == userObject2.get<MyClass*>());
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(equality)
{
    MyClass object1(11);
    MyClass object2(11);

    BOOST_CHECK_EQUAL(ponder::UserObject(object1)  == ponder::UserObject(object1),  true);
    BOOST_CHECK_EQUAL(ponder::UserObject(object1)  == ponder::UserObject(&object1), true);
    BOOST_CHECK_EQUAL(ponder::UserObject(&object1) == ponder::UserObject(&object1), true);
    BOOST_CHECK_EQUAL(ponder::UserObject(object1)  == ponder::UserObject(object2),  false);

    BOOST_CHECK_EQUAL(ponder::UserObject(object1) == ponder::UserObject::ref(object1),   true);
    BOOST_CHECK_EQUAL(ponder::UserObject(object1) == ponder::UserObject::ref(&object1),  true);
    BOOST_CHECK_EQUAL(ponder::UserObject(object1) == ponder::UserObject::copy(object1),  false);
    BOOST_CHECK_EQUAL(ponder::UserObject(object1) == ponder::UserObject::copy(&object1), false);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(nonCopyableClass)
{
    // This is a compile check
    MyNonCopyableClass object;
    ponder::UserObject userObject(object);
    MyNonCopyableClass& ref = userObject.get<MyNonCopyableClass>();
    (void)ref;
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(abstractClass)
{
    // This is a compile check
    MyConcreteClass object;

    ponder::UserObject userObject1(object);
    userObject1.get<MyAbstractClass>();

    ponder::UserObject userObject2(static_cast<MyAbstractClass&>(object));
    userObject2.get<MyConcreteClass>();
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(composedObjects)
{
    const ponder::UserProperty& p1 = static_cast<const ponder::UserProperty&>(ponder::classByType<Composed1>().property("p"));
    const ponder::UserProperty& p2 = static_cast<const ponder::UserProperty&>(ponder::classByType<Composed2>().property("p"));

    Composed1 composed1;
    ponder::UserObject composed2(composed1, p1);
    ponder::UserObject composed3(composed2, p2);

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
