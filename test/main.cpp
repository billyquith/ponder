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


#include "test.hpp"

#define BOOST_TEST_NO_LIB
#include <boost/test/unit_test.hpp>


//////////////////////////////////////////////////////////////
/////                   PROPERTY BIND                    /////
//////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE(campPropertyBindTest)
{
    camp::Class::declare<ComparableBase>("ComparableBase");
    camp::Class::declare<Comparable>("Comparable")
        .base<ComparableBase>()
        ;
    camp::Class::declare<TestClass>("TestClass");
    camp::Enum::declare<TestEnum>("TestEnum")
        .value("One", One)
        .value("Zero", Zero)
        .value("Ten", Ten)
        .value("MinusTwo", MinusTwo)
        .value("Five", Five)
        ;

    camp::Class::declare<PropertyTest>("PropertyTest")

    // ***** constant value *****
    //.property("p0", "constant") // not implemented (to do or not?)

    // ***** non-member functions *****
    .property("p1", &PropertyTest_get)
    .property("p2", &PropertyTest_ref)
    .property("p3", &PropertyTest_get, &PropertyTest_set)

    // ***** member functions *****
    .property("p4", &PropertyTest::getP2)
    .property("p5", &PropertyTest::p1)
    .property("p6", &PropertyTest::p3)
    .property("p7", &PropertyTest::getP2, &PropertyTest::setP2)

    // ***** nested member functions *****
    .property("p8",  &PropertyTest::Nested::p4,    &PropertyTest::n1)
    .property("p9",  &PropertyTest::Nested::getP5, &PropertyTest::n2)
    .property("p10", &PropertyTest::Nested::getP5, &PropertyTest::Nested::setP5, &PropertyTest::n3)
    .property("p11", &PropertyTest::Nested::p5,    &PropertyTest::Nested::setP5, &PropertyTest::n3)
    //.property("p12", &PropertyTest::Nested::getP5, &PropertyTest::Nested::p5,    &PropertyTest::n3)

    // ***** boost::function *****
    .property("p13", boost::function<short (PropertyTest&)>(&PropertyTest::getP2))
    .property("p14", boost::function<std::string& (PropertyTest&)>(&PropertyTest_ref))
    .property("p15", boost::function<short (PropertyTest&)>(&PropertyTest::getP2),
                     boost::function<void (PropertyTest&, short)>(&PropertyTest::setP2))

    // ***** boost::bind *****
    .property("p16", boost::bind(&PropertyTest::getP2, _1))
    .property("p17", boost::bind(&PropertyTest_ref, _1))
    .property("p18", boost::bind(&PropertyTest::getP2, _1), boost::bind(&PropertyTest::setP2, _1, _2))
    ;

    const camp::Class& mc = camp::classByType<PropertyTest>();

    // ***** types *****
    //BOOST_CHECK_EQUAL(mc.property("p0").type(),  camp::stringType);
    BOOST_CHECK_EQUAL(mc.property("p1").type(),  camp::stringType);
    BOOST_CHECK_EQUAL(mc.property("p2").type(),  camp::stringType);
    BOOST_CHECK_EQUAL(mc.property("p3").type(),  camp::stringType);
    BOOST_CHECK_EQUAL(mc.property("p4").type(),  camp::intType);
    BOOST_CHECK_EQUAL(mc.property("p5").type(),  camp::boolType);
    BOOST_CHECK_EQUAL(mc.property("p6").type(),  camp::userType);
    BOOST_CHECK_EQUAL(mc.property("p7").type(),  camp::intType);
    BOOST_CHECK_EQUAL(mc.property("p8").type(),  camp::enumType);
    BOOST_CHECK_EQUAL(mc.property("p9").type(),  camp::userType);
    BOOST_CHECK_EQUAL(mc.property("p10").type(), camp::userType);
    BOOST_CHECK_EQUAL(mc.property("p11").type(), camp::userType);
    //BOOST_CHECK_EQUAL(mc.property("p12").type(), camp::userType);
    BOOST_CHECK_EQUAL(mc.property("p13").type(), camp::intType);
    BOOST_CHECK_EQUAL(mc.property("p14").type(), camp::stringType);
    BOOST_CHECK_EQUAL(mc.property("p15").type(), camp::intType);
    BOOST_CHECK_EQUAL(mc.property("p16").type(), camp::intType);
    BOOST_CHECK_EQUAL(mc.property("p17").type(), camp::stringType);
    BOOST_CHECK_EQUAL(mc.property("p18").type(), camp::intType);
}


//////////////////////////////////////////////////////////////
/////                 PROPERTY GET / SET                 /////
//////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE(campPropertyGetSetTest)
{
    PropertyTest obj;
    const camp::Class& mc = camp::classByType<PropertyTest>();

    // ***** get / set *****
    mc.property("p2").set(obj, "aaa");
    BOOST_CHECK_EQUAL(mc.property("p2").get(obj).to<std::string>(), "aaa");
    mc.property("p3").set(obj, "bbb");
    BOOST_CHECK_EQUAL(mc.property("p3").get(obj).to<std::string>(), "bbb");
    mc.property("p5").set(obj, true);
    BOOST_CHECK_EQUAL(mc.property("p5").get(obj).to<bool>(), true);
    mc.property("p6").set(obj, Comparable(14));
    BOOST_CHECK_EQUAL(mc.property("p6").get(obj).to<Comparable>(), Comparable(14));
    mc.property("p7").set(obj, -89);
    BOOST_CHECK_EQUAL(mc.property("p7").get(obj).to<long>(), -89);
    mc.property("p8").set(obj, Ten);
    BOOST_CHECK_EQUAL(mc.property("p8").get(obj).to<TestEnum>(), Ten);
    mc.property("p10").set(obj, Comparable(66));
    BOOST_CHECK_EQUAL(mc.property("p10").get(obj).to<Comparable>(), Comparable(66));
    mc.property("p11").set(obj, Comparable(87));
    BOOST_CHECK_EQUAL(mc.property("p11").get(obj).to<Comparable>(), Comparable(87));
    //mc.property("p12").set(obj, Comparable(100));
    //BOOST_CHECK_EQUAL(mc.property("p12").get(obj).to<Comparable>(), Comparable(100));
    mc.property("p14").set(obj, "ccc");
    BOOST_CHECK_EQUAL(mc.property("p14").get(obj).to<std::string>(), "ccc");
    mc.property("p15").set(obj, 55);
    BOOST_CHECK_EQUAL(mc.property("p15").get(obj).to<long>(), 55);
    mc.property("p17").set(obj, "ddd");
    BOOST_CHECK_EQUAL(mc.property("p17").get(obj).to<std::string>(), "ddd");
    mc.property("p18").set(obj, 44);
    BOOST_CHECK_EQUAL(mc.property("p18").get(obj).to<long>(), 44);
}


//////////////////////////////////////////////////////////////
/////           PROPERTY READABLE / WRITABLE             /////
//////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE(campPropertyReadableWritableTest)
{
    camp::Class::declare<PropertyReadableWritableTest>("PropertyReadableWritableTest")

    // ***** constant value *****
    .property("p1", &PropertyReadableWritableTest::p).readable(false)
    .property("p2", &PropertyReadableWritableTest::p).writable(false)
    .property("p3", &PropertyReadableWritableTest::p).readable(false).writable(false)

    // ***** function *****
    .property("p4", &PropertyReadableWritableTest::p).readable(&PropertyReadableWritableTest::m_b)
    .property("p5", &PropertyReadableWritableTest::p).readable(&PropertyReadableWritableTest::b1)
    .property("p6", &PropertyReadableWritableTest::p).writable(&PropertyReadableWritableTest::b2)
    .property("p7", &PropertyReadableWritableTest::p).writable(boost::bind(&PropertyReadableWritableTest::b1, _1))
    .property("p8", &PropertyReadableWritableTest::p).writable(boost::function<bool (PropertyReadableWritableTest&)>(&PropertyReadableWritableTest::m_b))
    ;

    PropertyReadableWritableTest obj_t(true);
    PropertyReadableWritableTest obj_f(false);
    const camp::Class& mc = camp::classByType<PropertyReadableWritableTest>();

    // ***** readable *****
    BOOST_CHECK_EQUAL(mc.property("p1").readable(obj_t), false);
    BOOST_CHECK_EQUAL(mc.property("p1").readable(obj_f), false);
    BOOST_CHECK_EQUAL(mc.property("p2").readable(obj_t), true);
    BOOST_CHECK_EQUAL(mc.property("p2").readable(obj_f), true);
    BOOST_CHECK_EQUAL(mc.property("p3").readable(obj_t), false);
    BOOST_CHECK_EQUAL(mc.property("p3").readable(obj_f), false);
    BOOST_CHECK_EQUAL(mc.property("p4").readable(obj_t), true);
    BOOST_CHECK_EQUAL(mc.property("p4").readable(obj_f), false);
    BOOST_CHECK_EQUAL(mc.property("p5").readable(obj_t), true);
    BOOST_CHECK_EQUAL(mc.property("p5").readable(obj_f), true);

    // ***** writable *****
    BOOST_CHECK_EQUAL(mc.property("p1").writable(obj_t), true);
    BOOST_CHECK_EQUAL(mc.property("p1").writable(obj_f), true);
    BOOST_CHECK_EQUAL(mc.property("p2").writable(obj_t), false);
    BOOST_CHECK_EQUAL(mc.property("p2").writable(obj_f), false);
    BOOST_CHECK_EQUAL(mc.property("p3").writable(obj_t), false);
    BOOST_CHECK_EQUAL(mc.property("p3").writable(obj_f), false);
    BOOST_CHECK_EQUAL(mc.property("p6").writable(obj_t), false);
    BOOST_CHECK_EQUAL(mc.property("p6").writable(obj_f), false);
    BOOST_CHECK_EQUAL(mc.property("p7").writable(obj_t), true);
    BOOST_CHECK_EQUAL(mc.property("p7").writable(obj_f), true);
    BOOST_CHECK_EQUAL(mc.property("p8").writable(obj_t), true);
    BOOST_CHECK_EQUAL(mc.property("p8").writable(obj_f), false);

    PropertyTest obj;
    const camp::Class& mc2 = camp::classByType<PropertyTest>();

    // ***** implicitely writable (based on the validity of the setter) *****
    BOOST_CHECK_EQUAL(mc2.property("p1").writable(obj), false);
    BOOST_CHECK_EQUAL(mc2.property("p2").writable(obj), true);
    BOOST_CHECK_EQUAL(mc2.property("p3").writable(obj), true);
    BOOST_CHECK_EQUAL(mc2.property("p4").writable(obj), false);
    BOOST_CHECK_EQUAL(mc2.property("p5").writable(obj), true);
    BOOST_CHECK_EQUAL(mc2.property("p6").writable(obj), true);
    BOOST_CHECK_EQUAL(mc2.property("p7").writable(obj), true);
    BOOST_CHECK_EQUAL(mc2.property("p8").writable(obj), true);
    BOOST_CHECK_EQUAL(mc2.property("p9").writable(obj), false);
    BOOST_CHECK_EQUAL(mc2.property("p10").writable(obj), true);
    BOOST_CHECK_EQUAL(mc2.property("p11").writable(obj), true);
    //BOOST_CHECK_EQUAL(mc2.property("p12").writable(obj), true);
    BOOST_CHECK_EQUAL(mc2.property("p13").writable(obj), false);
    BOOST_CHECK_EQUAL(mc2.property("p14").writable(obj), true);
    BOOST_CHECK_EQUAL(mc2.property("p15").writable(obj), true);
    BOOST_CHECK_EQUAL(mc2.property("p16").writable(obj), false);
    BOOST_CHECK_EQUAL(mc2.property("p17").writable(obj), true);
    BOOST_CHECK_EQUAL(mc2.property("p18").writable(obj), true);
}


//////////////////////////////////////////////////////////////
/////                   USER PROPERTY                    /////
//////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE(campUserPropertyTest)
{
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
}


//////////////////////////////////////////////////////////////
/////                   CLASS VISITOR                    /////
//////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE(campClassVisitorTest)
{
    camp::Class::declare<ClassVisitorTest>("ClassVisitorTest")
        .property("simple",   &ClassVisitorTest::simpleProp)
        .property("array",    &ClassVisitorTest::arrayProp)
        .property("enum",     &ClassVisitorTest::enumProp)
        .property("user",     &ClassVisitorTest::userProp)
        .function("function", &ClassVisitorTest::function)
        ;

    const camp::Class& c = camp::classByType<ClassVisitorTest>();

    MyClassVisitor visitor;
    c.visit(visitor);

    // ***** check *****
    BOOST_CHECK_EQUAL(visitor.count, 5U);
    BOOST_CHECK_EQUAL(visitor.simpleName, "simple");
    BOOST_CHECK_EQUAL(visitor.arrayName, "array");
    BOOST_CHECK_EQUAL(visitor.enumName, "enum");
    BOOST_CHECK_EQUAL(visitor.userName, "user");
    BOOST_CHECK_EQUAL(visitor.functionName, "function");
}


//////////////////////////////////////////////////////////////
/////                   FUNCTION BIND                    /////
//////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE(campFunctionBindTest)
{
    camp::Class::declare<FunctionTest>("FunctionTest")

    // ***** non-member functions *****
    .function("f1", &FunctionTest_byRef)
    .function("f2", &FunctionTest_byPtr)

    // ***** member functions *****
    .function("f3", &FunctionTest::nonConstFunc)
    .function("f4", &FunctionTest::constFunc)
    .function("f5", static_cast<std::string (FunctionTest::*)()>(&FunctionTest::inheritedFunc))

    // ***** arguments count ******
    .function("f6",  &FunctionTest::f0)
    .function("f7",  &FunctionTest::f1)
    .function("f8",  &FunctionTest::f2)
    .function("f9",  &FunctionTest::f3)
    .function("f10", &FunctionTest::f4)
    .function("f11", &FunctionTest::f5)

    // ***** camp::Value as argument and return type *****
    .function("f12", &FunctionTest::abstractFunc)

    // ***** nested functions *****
    .function("f13", &FunctionTest::Nested::func, &FunctionTest::nested)
    .function("f14", &FunctionTest::Nested::func, &FunctionTest::getNested)
    .function("f15", &FunctionTest::Nested::func, &FunctionTest::nestedPtr)
    .function("f16", &FunctionTest::Nested::func, &FunctionTest::getNestedPtr)
    .function("f17", &FunctionTest::Nested::func, &FunctionTest::nestedSmartPtr)
    .function("f18", &FunctionTest::Nested::func, &FunctionTest::nestedMySmartPtr)

    // ***** boost::function *****
    .function("f19", boost::function<void (FunctionTest&, short, int, long)>(boost::bind(&FunctionTest::f3, _1, _2, _3, _4)))
    .function("f20", boost::function<void (FunctionTest&, short)>(boost::bind(&FunctionTest::f3, _1, _2, 20, 30L)))
    .function("f21", boost::function<void (FunctionTest&, short)>(boost::bind(boost::bind(&FunctionTest::f3, _1, _2, _3, 30L), _1, _2, 20)))

    // ***** constant value *****
    //.function("f22", 50) // not implemented (to do or not?)

    // ***** functor *****
    //.function("f23", FunctorTest()) // not implemented (to do or not?)
    ;

    // ***** return types *****
    const camp::Class& mc = camp::classByType<FunctionTest>();
    BOOST_CHECK_EQUAL(mc.function("f1").returnType(),  camp::noType);
    BOOST_CHECK_EQUAL(mc.function("f2").returnType(),  camp::intType);
    BOOST_CHECK_EQUAL(mc.function("f3").returnType(),  camp::realType);
    BOOST_CHECK_EQUAL(mc.function("f4").returnType(),  camp::boolType);
    BOOST_CHECK_EQUAL(mc.function("f5").returnType(),  camp::stringType);
    BOOST_CHECK_EQUAL(mc.function("f12").returnType(), camp::userType);
    BOOST_CHECK_EQUAL(mc.function("f13").returnType(), camp::enumType);

    // ***** argument count *****
    BOOST_CHECK_EQUAL(mc.function("f1").argCount(),  0U);
    BOOST_CHECK_EQUAL(mc.function("f2").argCount(),  0U);
    BOOST_CHECK_EQUAL(mc.function("f3").argCount(),  0U);
    BOOST_CHECK_EQUAL(mc.function("f7").argCount(),  1U);
    BOOST_CHECK_EQUAL(mc.function("f8").argCount(),  2U);
    BOOST_CHECK_EQUAL(mc.function("f9").argCount(),  3U);
    BOOST_CHECK_EQUAL(mc.function("f10").argCount(), 4U);
    BOOST_CHECK_EQUAL(mc.function("f11").argCount(), 5U);
    BOOST_CHECK_EQUAL(mc.function("f12").argCount(), 1U);
    BOOST_CHECK_EQUAL(mc.function("f13").argCount(), 1U);
    BOOST_CHECK_EQUAL(mc.function("f19").argCount(), 3U);
    BOOST_CHECK_EQUAL(mc.function("f20").argCount(), 1U);
    BOOST_CHECK_EQUAL(mc.function("f21").argCount(), 1U);

    // ***** argument types *****
    BOOST_CHECK_EQUAL(mc.function("f7").argType(0),  camp::boolType);
    BOOST_CHECK_EQUAL(mc.function("f8").argType(0),  camp::realType);
    BOOST_CHECK_EQUAL(mc.function("f8").argType(1),  camp::realType);
    BOOST_CHECK_EQUAL(mc.function("f9").argType(0),  camp::intType);
    BOOST_CHECK_EQUAL(mc.function("f9").argType(1),  camp::intType);
    BOOST_CHECK_EQUAL(mc.function("f9").argType(2),  camp::intType);
    BOOST_CHECK_EQUAL(mc.function("f10").argType(0), camp::stringType);
    BOOST_CHECK_EQUAL(mc.function("f10").argType(1), camp::stringType);
    BOOST_CHECK_EQUAL(mc.function("f10").argType(2), camp::stringType);
    BOOST_CHECK_EQUAL(mc.function("f10").argType(3), camp::stringType);
    BOOST_CHECK_EQUAL(mc.function("f11").argType(0), camp::enumType);
    BOOST_CHECK_EQUAL(mc.function("f12").argType(0), camp::userType);
}


//////////////////////////////////////////////////////////////
/////                   FUNCTION CALL                    /////
//////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE(campFunctionCallTest)
{
    FunctionTest obj;
    const camp::Class& mc = camp::classByObject(obj);
    camp::Value v1  = mc.function("f1").call(obj);
    camp::Value v2  = mc.function("f2").call(obj);
    camp::Value v3  = mc.function("f3").call(obj);
    camp::Value v4  = mc.function("f4").call(obj);
    camp::Value v5  = mc.function("f5").call(obj);
    camp::Value v6  = mc.function("f6").call(obj);
    camp::Value v7  = mc.function("f7").call(obj, camp::Args(true));
    camp::Value v8  = mc.function("f8").call(obj, camp::Args(5.5f, 6.6));
    camp::Value v9  = mc.function("f9").call(obj, camp::Args(1, 2, 3));
    camp::Value v10 = mc.function("f10").call(obj, camp::Args("aaa", "bbb", "ccc", "ddd"));
    camp::Value v11 = mc.function("f11").call(obj, camp::Args(One, Zero, Ten, MinusTwo, Five));
    camp::Value v12 = mc.function("f12").call(obj, camp::Args(v5));
    camp::Value v13 = mc.function("f13").call(obj, camp::Args(13));
    camp::Value v14 = mc.function("f14").call(obj, camp::Args(14));
    camp::Value v15 = mc.function("f15").call(obj, camp::Args(15));
    camp::Value v16 = mc.function("f16").call(obj, camp::Args(16));
    camp::Value v17 = mc.function("f17").call(obj, camp::Args(17));
    camp::Value v18 = mc.function("f18").call(obj, camp::Args(18));
    camp::Value v19 = mc.function("f19").call(obj, camp::Args(4, 5, 6));
    camp::Value v20 = mc.function("f20").call(obj, camp::Args(7));
    camp::Value v21 = mc.function("f21").call(obj, camp::Args(8));

    // ***** object properly modified *****
    BOOST_CHECK_EQUAL(obj.s, "hello");

    // ***** returned type *****
    BOOST_CHECK_EQUAL(v1.type(),  camp::noType);
    BOOST_CHECK_EQUAL(v2.type(),  camp::intType);
    BOOST_CHECK_EQUAL(v3.type(),  camp::realType);
    BOOST_CHECK_EQUAL(v4.type(),  camp::boolType);
    BOOST_CHECK_EQUAL(v5.type(),  camp::stringType);
    BOOST_CHECK_EQUAL(v13.type(), camp::enumType);

    // ***** returned value *****
    BOOST_CHECK_EQUAL(v2.to<int>(), 24);
    BOOST_CHECK_CLOSE(v3.to<float>(), 5.5f, 1E-5f);
    BOOST_CHECK_EQUAL(v4.to<bool>(), false);
    BOOST_CHECK_EQUAL(v5.to<std::string>(), "inherited");
    BOOST_CHECK_EQUAL(v12.to<camp::Value>() == v5, true);
    BOOST_CHECK_EQUAL(v13.to<TestEnum>(), MinusTwo);
    BOOST_CHECK_EQUAL(v14.to<TestEnum>(), MinusTwo);
    BOOST_CHECK_EQUAL(v15.to<TestEnum>(), MinusTwo);
    BOOST_CHECK_EQUAL(v16.to<TestEnum>(), MinusTwo);
    BOOST_CHECK_EQUAL(v17.to<TestEnum>(), MinusTwo);
    BOOST_CHECK_EQUAL(v18.to<TestEnum>(), MinusTwo);
}


//////////////////////////////////////////////////////////////
/////                 FUNCTION CALLABLE                  /////
//////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE(campFunctionCallableTest)
{
    camp::Class::declare<FunctionCallableTest>("FunctionCallableTest")

    // ***** constant value *****
    .function("f1", &FunctionCallableTest::f).callable(true)

    // ***** function *****
    .function("f2", &FunctionCallableTest::f).callable(&FunctionCallableTest::m_b)
    .function("f3", &FunctionCallableTest::f).callable(&FunctionCallableTest::b1)
    .function("f4", &FunctionCallableTest::f).callable(&FunctionCallableTest::b2)
    .function("f5", &FunctionCallableTest::f).callable(boost::bind(&FunctionCallableTest::b1, _1))
    .function("f6", &FunctionCallableTest::f).callable(boost::function<bool (FunctionCallableTest&)>(&FunctionCallableTest::m_b))
    ;

    FunctionCallableTest obj_t(true);
    FunctionCallableTest obj_f(false);
    const camp::Class& mc = camp::classByType<FunctionCallableTest>();

    // ***** callable *****
    BOOST_CHECK_EQUAL(mc.function("f1").callable(obj_t), true);
    BOOST_CHECK_EQUAL(mc.function("f1").callable(obj_f), true);
    BOOST_CHECK_EQUAL(mc.function("f2").callable(obj_t), true);
    BOOST_CHECK_EQUAL(mc.function("f2").callable(obj_f), false);
    BOOST_CHECK_EQUAL(mc.function("f3").callable(obj_t), true);
    BOOST_CHECK_EQUAL(mc.function("f3").callable(obj_f), true);
    BOOST_CHECK_EQUAL(mc.function("f4").callable(obj_t), false);
    BOOST_CHECK_EQUAL(mc.function("f4").callable(obj_f), false);
    BOOST_CHECK_EQUAL(mc.function("f5").callable(obj_t), true);
    BOOST_CHECK_EQUAL(mc.function("f5").callable(obj_f), true);
    BOOST_CHECK_EQUAL(mc.function("f6").callable(obj_t), true);
    BOOST_CHECK_EQUAL(mc.function("f6").callable(obj_f), false);
}


//////////////////////////////////////////////////////////////
/////                     CONSTRUCTOR                    /////
//////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE(campConstructorTest)
{
    camp::Class::declare<ConstructorTestBase1>("ConstructorTestBase1");
    camp::Class::declare<ConstructorTestBase2>("ConstructorTestBase2");

    camp::Class::declare<ConstructorTest>("ConstructorTest")
        .base<ConstructorTestBase1>()
        .base<ConstructorTestBase2>()
        .constructor0()
        .constructor1<ConstructorTest>()
        .constructor1<bool>()
        .constructor2<bool, int>()
        .constructor3<bool, int, std::string>()
        .constructor4<bool, int, std::string, TestEnum>()
        .constructor5<bool, int, std::string, TestEnum, Comparable>()
        ;

    ConstructorTest t(false, 24, "hello", MinusTwo, Comparable(80));
    const camp::Class& c = camp::classByType<ConstructorTest>();

    ConstructorTest* t0 = c.construct<ConstructorTest>();
    ConstructorTest* t1 = c.construct<ConstructorTest>(camp::Args(true));
    ConstructorTest* t2 = c.construct<ConstructorTest>(camp::Args(true, 10));
    ConstructorTest* t3 = c.construct<ConstructorTest>(camp::Args(true, 10, "test"));
    ConstructorTest* t4 = c.construct<ConstructorTest>(camp::Args(true, 10, "test", Ten));
    ConstructorTest* t5 = c.construct<ConstructorTest>(camp::Args(true, 10, "test", Ten, Comparable(50)));
    ConstructorTest* tc = static_cast<ConstructorTest*>(c.construct<void>(camp::Args(t)));
    ConstructorTestBase1* b1 = c.construct<ConstructorTestBase1>();
    ConstructorTestBase2* b2 = c.construct<ConstructorTestBase2>();

    // ***** valid instances *****
    BOOST_CHECK_EQUAL(t0 != 0, true);
    BOOST_CHECK_EQUAL(t1 != 0, true);
    BOOST_CHECK_EQUAL(t2 != 0, true);
    BOOST_CHECK_EQUAL(t3 != 0, true);
    BOOST_CHECK_EQUAL(t4 != 0, true);
    BOOST_CHECK_EQUAL(t5 != 0, true);
    BOOST_CHECK_EQUAL(tc != 0, true);
    BOOST_CHECK_EQUAL(b1 != 0, true);
    BOOST_CHECK_EQUAL(b2 != 0, true);

    // ***** inheritance *****
    BOOST_CHECK_EQUAL(b1->str1, "base1");
    BOOST_CHECK_EQUAL(b2->str2, "base2");

    // ***** args count *****
    BOOST_CHECK_EQUAL(t0->args, 0);
    BOOST_CHECK_EQUAL(t1->args, 1);
    BOOST_CHECK_EQUAL(t2->args, 2);
    BOOST_CHECK_EQUAL(t3->args, 3);
    BOOST_CHECK_EQUAL(t4->args, 4);
    BOOST_CHECK_EQUAL(t5->args, 5);
    BOOST_CHECK_EQUAL(tc->args, 5);

    // ***** args values *****
    BOOST_CHECK_EQUAL(t1->arg1, true);
    BOOST_CHECK_EQUAL(t2->arg2, 10);
    BOOST_CHECK_EQUAL(t3->arg3, "test");
    BOOST_CHECK_EQUAL(t4->arg4, Ten);
    BOOST_CHECK_EQUAL(t5->arg5, Comparable(50));
    BOOST_CHECK_EQUAL(tc->arg1, false);
    BOOST_CHECK_EQUAL(tc->arg2, 24);
    BOOST_CHECK_EQUAL(tc->arg3, "hello");
    BOOST_CHECK_EQUAL(tc->arg4, MinusTwo);
    BOOST_CHECK_EQUAL(tc->arg5, Comparable(80));

    c.destroy(t0);
    c.destroy(t1);
    c.destroy(t2);
    c.destroy(t3);
    c.destroy(t4);
    c.destroy(t5);
    c.destroy(tc);
    c.destroy(b1);
    c.destroy(b2);
}


//////////////////////////////////////////////////////////////
/////                  INHERITED CLASSES                 /////
//////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE(campInheritedClassesTest)
{
    camp::Class::declare<Base1>("Base1")
        .function("f1", &Base1::f1)
        .property("p1", &Base1::p1);

    camp::Class::declare<Base2>("Base2")
        .function("f2", &Base2::f2)
        .function("fv", &Base2::fv)
        .property("p2", &Base2::p2);

    camp::Class::declare<Base3>("Base3")
        .base<Base1>()
        .base<Base2>()
        .function("f3", &Base3::f3)
        .property("p3", &Base3::p3);

    camp::Class::declare<Base4>("Base4")
        .base<Base3>()
        .function("f4", &Base4::f4)
        .property("p4", &Base4::p4);

    Base1 b1;
    Base2 b2;
    Base3 b3;
    Base4 b4;

    const camp::Class& c1 = camp::classByType<Base1>();
    const camp::Class& c2 = camp::classByType<Base2>();
    const camp::Class& c3 = camp::classByType<Base3>();
    const camp::Class& c4 = camp::classByType<Base4>();

    // ***** function *****
    BOOST_CHECK_EQUAL(c1.function("f1").call(b1).to<int>(), 1);
    BOOST_CHECK_EQUAL(c3.function("f1").call(b3).to<int>(), 1);
    BOOST_CHECK_EQUAL(c4.function("f1").call(b4).to<int>(), 1);
    BOOST_CHECK_EQUAL(c2.function("f2").call(b2).to<int>(), 2);
    BOOST_CHECK_EQUAL(c3.function("f2").call(b3).to<int>(), 2);
    BOOST_CHECK_EQUAL(c4.function("f2").call(b4).to<int>(), 2);
    BOOST_CHECK_EQUAL(c3.function("f3").call(b3).to<int>(), 3);
    BOOST_CHECK_EQUAL(c4.function("f3").call(b4).to<int>(), 3);
    BOOST_CHECK_EQUAL(c4.function("f4").call(b4).to<int>(), 4);

    // ***** virtual function *****
    BOOST_CHECK_EQUAL(c2.function("fv").call(b2).to<int>(), 20);
    BOOST_CHECK_EQUAL(c3.function("fv").call(b3).to<int>(), 30);
    BOOST_CHECK_EQUAL(c4.function("fv").call(b4).to<int>(), 40);
    BOOST_CHECK_EQUAL(c2.function("fv").call(b2).to<int>(), 20);
    BOOST_CHECK_EQUAL(c2.function("fv").call(b3).to<int>(), 30);
    BOOST_CHECK_EQUAL(c2.function("fv").call(b4).to<int>(), 40);

    // ***** properties *****
    BOOST_CHECK_EQUAL(c1.property("p1").get(b1).to<int>(), 10);
    BOOST_CHECK_EQUAL(c3.property("p1").get(b3).to<int>(), 10);
    BOOST_CHECK_EQUAL(c4.property("p1").get(b4).to<int>(), 10);
    BOOST_CHECK_EQUAL(c2.property("p2").get(b2).to<int>(), 20);
    BOOST_CHECK_EQUAL(c3.property("p2").get(b3).to<int>(), 20);
    BOOST_CHECK_EQUAL(c4.property("p2").get(b4).to<int>(), 20);
    BOOST_CHECK_EQUAL(c3.property("p3").get(b3).to<int>(), 30);
    BOOST_CHECK_EQUAL(c4.property("p3").get(b4).to<int>(), 30);
    BOOST_CHECK_EQUAL(c4.property("p4").get(b4).to<int>(), 40);

    // ***** up/down-cast with offset *****
    Base2* bb3 = &b3;
    Base2* bb4 = &b4;
    BOOST_CHECK_EQUAL(c2.property("p2").get(bb3).to<int>(), 20);
    BOOST_CHECK_EQUAL(c2.property("p2").get(bb4).to<int>(), 20);
    BOOST_CHECK_EQUAL(c3.property("p3").get(bb3).to<int>(), 30);
    BOOST_CHECK_EQUAL(c3.property("p3").get(bb4).to<int>(), 30);
    BOOST_CHECK_EQUAL(c4.property("p4").get(bb4).to<int>(), 40);
}


//////////////////////////////////////////////////////////////
/////                 EXTERNAL MAPPING                   /////
//////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE(campExternalMappingTest)
{
    camp::Class::declare<MyTestMapping>("MyTestMapping")
        .external<MyTestMapper>();

    MyTestMapping obj;
    const camp::Class& mc = camp::classByType<MyTestMapping>();

    // ***** count *****
    BOOST_CHECK_EQUAL(mc.propertyCount(), 5U);
    BOOST_CHECK_EQUAL(mc.functionCount(), 3U);

    // ***** type *****
    BOOST_CHECK_EQUAL(mc.property(0).type(), camp::intType);
    BOOST_CHECK_EQUAL(mc.function(0).returnType(), camp::stringType);

    // ***** args count *****
    BOOST_CHECK_EQUAL(mc.function(0).argCount(), 0U);

    // ***** get *****
    BOOST_CHECK_EQUAL(mc.property("prop0").get(obj).to<int>(), 0);
    BOOST_CHECK_EQUAL(mc.property("prop1").get(obj).to<int>(), 10);
    BOOST_CHECK_EQUAL(mc.property("prop2").get(obj).to<int>(), 20);
    BOOST_CHECK_EQUAL(mc.property("prop3").get(obj).to<int>(), 30);
    BOOST_CHECK_EQUAL(mc.property("prop4").get(obj).to<int>(), 40);

    // ***** set *****
    mc.property("prop0").set(obj, 0);
    mc.property("prop1").set(obj, 100);
    mc.property("prop2").set(obj, 200);
    mc.property("prop3").set(obj, 300);
    mc.property("prop4").set(obj, 400);
    BOOST_CHECK_EQUAL(obj.m_props["prop0"], 0);
    BOOST_CHECK_EQUAL(obj.m_props["prop1"], 100);
    BOOST_CHECK_EQUAL(obj.m_props["prop2"], 200);
    BOOST_CHECK_EQUAL(obj.m_props["prop3"], 300);
    BOOST_CHECK_EQUAL(obj.m_props["prop4"], 400);

    // ***** call *****
    BOOST_CHECK_EQUAL(mc.function("func0").call(obj).to<std::string>(), "func0_called");
    BOOST_CHECK_EQUAL(mc.function("func1").call(obj).to<std::string>(), "func1_called");
    BOOST_CHECK_EQUAL(mc.function("func2").call(obj).to<std::string>(), "func2_called");
}


//////////////////////////////////////////////////////////////
/////                     USEROBJECT                     /////
//////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE(campUserObjectTest)
{
    Comparable test(2);
    camp::UserObject obj(test);
    camp::UserObject ptr(&test);

    // ***** get *****
    BOOST_CHECK_EQUAL(obj.get<Comparable>(), test);
    BOOST_CHECK_EQUAL(ptr.get<Comparable>(), test);
    BOOST_CHECK_EQUAL(obj.get<Comparable*>(), &test);
    BOOST_CHECK_EQUAL(ptr.get<Comparable*>(), &test);
    BOOST_CHECK_EQUAL(obj.get<const Comparable*>(), &test);
    BOOST_CHECK_EQUAL(ptr.get<const Comparable*>(), &test);

    // ***** get with inheritance *****
    camp::Class::declare<TestClass2>("Test2");
    camp::Class::declare<DerivedMultiple>("DerivedMultiple")
        .base<TestClass>()
        .base<TestClass2>();
    camp::Class::declare<DerivedDeep>("DerivedDeep")
        .base<DerivedMultiple>();
    TestClass baseObj;
    DerivedDeep derivedObj;
    camp::UserObject base(baseObj);
    camp::UserObject derived(derivedObj);
    BOOST_CHECK_EQUAL(derived.get<DerivedMultiple*>(), &static_cast<DerivedMultiple&>(derivedObj));
    BOOST_CHECK_EQUAL(derived.get<TestClass2*>(), &static_cast<TestClass2&>(derivedObj));
    BOOST_CHECK_EQUAL(derived.get<TestClass*>(), &static_cast<TestClass&>(derivedObj));

    // ***** getClass *****
    BOOST_CHECK_EQUAL(obj.getClass().name(), "Comparable");
    BOOST_CHECK_EQUAL(ptr.getClass().name(), "Comparable");

    camp::Class::declare<ObjectTest>("ObjectTest")
        .property("prop", &ObjectTest::prop)
        .function("func", &ObjectTest::func);

    // ***** convenience interface (get / set / call) *****
    ObjectTest objTest;
    camp::UserObject object = objTest;
    object.set("prop", 20);
    BOOST_CHECK_EQUAL(object.get("prop").to<int>(), objTest.prop);
    BOOST_CHECK_EQUAL(objTest.prop, 20);
    BOOST_CHECK_EQUAL(object.call("func", camp::Args(60)).to<std::string>(), "60");

    // ***** getClass with polymorphic object *****
    camp::UserObject polymorphic(static_cast<ComparableBase&>(test));
    BOOST_CHECK_EQUAL(polymorphic.getClass().name(), "Comparable");

    // ***** operator == *****
    Comparable other(2);
    BOOST_CHECK_EQUAL(obj == camp::UserObject(test), true);
    BOOST_CHECK_EQUAL(ptr == camp::UserObject(&test), true);
    BOOST_CHECK_EQUAL(obj == camp::UserObject(&test), true);
    BOOST_CHECK_EQUAL(ptr == camp::UserObject(test), true);
    BOOST_CHECK_EQUAL(obj == camp::UserObject(other), false);
    BOOST_CHECK_EQUAL(ptr == camp::UserObject(&other), false);
    BOOST_CHECK_EQUAL(obj == camp::UserObject::ref(test), true);
    BOOST_CHECK_EQUAL(obj == camp::UserObject::copy(test), false);
    BOOST_CHECK_EQUAL(obj == camp::UserObject::ref(&test), true);
    BOOST_CHECK_EQUAL(obj == camp::UserObject::copy(&test), false);

    // ***** operator = *****
    camp::UserObject copy;
    copy = derived;
    BOOST_CHECK_EQUAL(copy == derived, true);

    // ***** storage of abstract classes *****
    camp::Class::declare<AbstractClass>("AbstractClass");
    camp::Class::declare<ConcreteClass>("ConcreteClass")
        .base<AbstractClass>();
    ConcreteClass c;
    camp::UserObject obj1(c);
    obj1.get<AbstractClass>();
    camp::UserObject obj2(static_cast<AbstractClass&>(c));
    obj2.get<ConcreteClass>();

    // ***** composed objects *****
    camp::Class::declare<Owner>("Owner")
        .property("p", &Owner::get, &Owner::set);
    camp::Class::declare<SuperOwner>("SuperOwner")
        .property("p", &SuperOwner::get, &SuperOwner::set);
    const camp::UserProperty& p1 = static_cast<const camp::UserProperty&>(camp::classByType<SuperOwner>().property("p"));
    const camp::UserProperty& p2 = static_cast<const camp::UserProperty&>(camp::classByType<Owner>().property("p"));
    SuperOwner sowner;
    camp::UserObject owner(sowner, p1);
    camp::UserObject owned(owner, p2);
    BOOST_CHECK_EQUAL(sowner.m_x, 0);
    sowner.setValue(Comparable(10));
    BOOST_CHECK_EQUAL(sowner.getValue(), Comparable(10));
    BOOST_CHECK_EQUAL(owner.get<Owner>().get(), Comparable(10));
    BOOST_CHECK_EQUAL(owned.get<Comparable>(), Comparable(10));
    sowner.setValue(Comparable(20));
    BOOST_CHECK_EQUAL(sowner.getValue(), Comparable(20));
    BOOST_CHECK_EQUAL(owner.get<Owner>().get(), Comparable(20));
    BOOST_CHECK_EQUAL(owned.get<Comparable>(), Comparable(20));
    p2.set(owner, Comparable(30));
    BOOST_CHECK_EQUAL(sowner.m_x, 1);
    BOOST_CHECK_EQUAL(sowner.getValue(), Comparable(30));
    BOOST_CHECK_EQUAL(owner.get<Owner>().get(), Comparable(30));
    BOOST_CHECK_EQUAL(owned.get<Comparable>(), Comparable(30));
    p2.set(owner, Comparable(40));
    BOOST_CHECK_EQUAL(sowner.m_x, 2);
    BOOST_CHECK_EQUAL(sowner.getValue(), Comparable(40));
    BOOST_CHECK_EQUAL(owner.get<Owner>().get(), Comparable(40));
    BOOST_CHECK_EQUAL(owned.get<Comparable>(), Comparable(40));
}

