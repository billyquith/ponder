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

#include <ponder/classget.hpp>
#include <ponder/userobject.hpp>
#include <ponder/pondertype.hpp>
#include <ponder/class.hpp>
#include <ponder/classbuilder.hpp>
#include "catch.hpp"
#include <ostream>

namespace UserObjectTest
{
    struct MyBaseWithPadding
    {
        virtual ~MyBaseWithPadding() {}
        char padding[15];
    };
    
    struct MyBase
    {
        MyBase(int b_) : b(b_) {}
        virtual ~MyBase() {}
        int b;
        PONDER_RTTI();
    };
    
    bool operator==(const MyBase& left, const MyBase& right)
    {
        return left.b == right.b;
    }
    
    std::ostream& operator<<(std::ostream& stream, const MyBase& b)
    {
        return stream << b.b;
    }
    
    struct MyClass : MyBaseWithPadding, MyBase
    {
        MyClass(int x_) : MyBase(x_ + 1), x(x_) {}
        int x;
        int f() const {return x;}
        PONDER_RTTI();
    };
    
    bool operator==(const MyClass& left, const MyClass& right)
    {
        return left.x == right.x;
    }
    
    std::ostream& operator<<(std::ostream& stream, const MyClass& m)
    {
        return stream << m.x;
    }
    
    struct MyNonCopyableClass : ponder::detail::noncopyable
    {
    };
    
    struct MyAbstractClass
    {
        virtual ~MyAbstractClass() {}
        virtual void f() = 0;
        PONDER_RTTI();
    };
    
    struct MyConcreteClass : MyAbstractClass
    {
        virtual void f() {}
        PONDER_RTTI();
    };
    
    struct Composed3
    {
        Composed3(int x_ = -1) : x(x_) {}
        int x;
    };
    
    struct Composed2
    {
        Composed3 get() const {return composed;}
        void set(Composed3 c) {composed = c;}
        Composed3 composed;
    };
    
    struct Composed1
    {
        Composed2 get() const {return composed;}
        void set(Composed2 c) {composed = c;}
        Composed2 composed;
    };
    
    struct Call
    {
        std::string lastCalled;
        int sum;
        
        Call() : lastCalled("unset"), sum(0) {}
        
        void meth1(int a0)
        {
            lastCalled = "meth1";
            sum += a0;
        }
        void meth2(int a0, int a1)
        {
            lastCalled = "meth2";
            sum += a0 + a1;
        }
        void meth3(int a0,int a1, int a2)
        {
            lastCalled = "meth3";
            sum += a0 + a1 + a2;
        }
        void meth8(int a0,int a1, int a2, int a3, int a4, int a5, int a6, int a7)
        {
            lastCalled = "meth8";
            sum += a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7;
        }
        
        double cos(double rad)
        {
            return std::cos(rad);
        }
        
        std::string concat(const std::string &a, const std::string &b)
        {
            return a+b;
        }
    };
    
    void declare()
    {
        ponder::Class::declare<MyBase>("UserObjectTest::MyBase");
        ponder::Class::declare<MyClass>("UserObjectTest::MyClass")
            .base<MyBase>()
            .property("p", &MyClass::x)
            .function("f", &MyClass::f);
        
        ponder::Class::declare<MyNonCopyableClass>("UserObjectTest::MyNonCopyableClass");
        
        ponder::Class::declare<MyAbstractClass>("UserObjectTest::MyAbstractClass");
        ponder::Class::declare<MyConcreteClass>("UserObjectTest::MyConcreteClass")
            .base<MyAbstractClass>();
        
        ponder::Class::declare<Composed3>("UserObjectTest::Composed3")
            .property("x", &Composed3::x);
        ponder::Class::declare<Composed2>("UserObjectTest::Composed2")
            .property("p", &Composed2::get, &Composed2::set);
        ponder::Class::declare<Composed1>("UserObjectTest::Composed1")
            .property("p", &Composed1::get, &Composed1::set);
        
        ponder::Class::declare<Call>("UserObjectTest::Call")
            .function("meth1", &Call::meth1)
            .function("meth2", &Call::meth2)
            .function("meth3", &Call::meth3)
            .function("cos", &Call::cos)
            .function("concat", &Call::concat)
            .function("meth8", &Call::meth8);
    }
}

PONDER_AUTO_TYPE(UserObjectTest::MyBase, &UserObjectTest::declare)
PONDER_AUTO_TYPE(UserObjectTest::MyClass, &UserObjectTest::declare)
PONDER_AUTO_TYPE_NONCOPYABLE(UserObjectTest::MyNonCopyableClass, &UserObjectTest::declare)
PONDER_AUTO_TYPE_NONCOPYABLE(UserObjectTest::MyAbstractClass, &UserObjectTest::declare)
PONDER_AUTO_TYPE_NONCOPYABLE(UserObjectTest::MyConcreteClass, &UserObjectTest::declare)
PONDER_AUTO_TYPE(UserObjectTest::Composed3, &UserObjectTest::declare)
PONDER_AUTO_TYPE(UserObjectTest::Composed2, &UserObjectTest::declare)
PONDER_AUTO_TYPE(UserObjectTest::Composed1, &UserObjectTest::declare)
PONDER_AUTO_TYPE(UserObjectTest::Call, &UserObjectTest::declare)

using namespace UserObjectTest;

//-----------------------------------------------------------------------------
//                         Tests for ponder::UserObject
//-----------------------------------------------------------------------------

//TEST_CASE("Ponder supports user objects")
//{
//    SECTION("user objects reference other objects")
//    {
//        MyClass object(1);
//        ponder::UserObject userObject(object); //reference
//
//        REQUIRE(userObject.get<MyClass>() ==         object);
//        REQUIRE(&userObject.get<MyClass>() ==        &object);
//        REQUIRE(userObject.get<MyClass&>() ==        object);
//        REQUIRE(&userObject.get<MyClass&>() ==       &object);
//        REQUIRE(userObject.get<const MyClass&>() ==  object);
//        REQUIRE(&userObject.get<const MyClass&>() == &object);
//        REQUIRE(userObject.get<MyClass*>() ==        &object);
//        REQUIRE(userObject.get<const MyClass*>() ==  &object);
//    }
//
//    SECTION("user objects can reference other objects using pointers")
//    {
//        MyClass object(3);
//        ponder::UserObject userObject(&object); // pointer
//
//        REQUIRE(userObject.get<MyClass>() ==         object);
//        REQUIRE(&userObject.get<MyClass>() ==        &object);
//        REQUIRE(userObject.get<MyClass&>() ==        object);
//        REQUIRE(&userObject.get<MyClass&>() ==       &object);
//        REQUIRE(userObject.get<const MyClass&>() ==  object);
//        REQUIRE(&userObject.get<const MyClass&>() == &object);
//        REQUIRE(userObject.get<MyClass*>() ==        &object);
//        REQUIRE(userObject.get<const MyClass*>() ==  &object);
//    }
//
//    SECTION("user object references should handle inheritance")
//    {
//        MyClass object(2);
//        ponder::UserObject userObject(object);
//        MyBase& base = object;
//
//        REQUIRE(userObject.get<MyBase>() ==         base);
//        REQUIRE(&userObject.get<MyBase>() ==        &base);
//        REQUIRE(userObject.get<MyBase&>() ==        base);
//        REQUIRE(&userObject.get<MyBase&>() ==       &base);
//        REQUIRE(userObject.get<const MyBase&>() ==  base);
//        REQUIRE(&userObject.get<const MyBase&>() == &base);
//        REQUIRE(userObject.get<MyBase*>() ==        &base);
//        REQUIRE(userObject.get<const MyBase*>() ==  &base);
//    }
//    
//    SECTION("user objects can be assigned to")
//    {
//        MyClass object1(10);
//        ponder::UserObject userObject1(object1);
//        ponder::UserObject userObject2;
//        
//        userObject2 = userObject1;
//
//        REQUIRE(userObject1.get<MyClass*>() == userObject2.get<MyClass*>());
//    }
//
//    SECTION("objects can be tested for equality")
//    {
//        MyClass object1(11);
//        MyClass object2(11);
//
//        REQUIRE((ponder::UserObject(object1)  == ponder::UserObject(object1)));
//        REQUIRE((ponder::UserObject(object1)  == ponder::UserObject(&object1)));
//        REQUIRE((ponder::UserObject(&object1) == ponder::UserObject(&object1)));
//        REQUIRE((ponder::UserObject(object1)  != ponder::UserObject(object2)));
//
//        REQUIRE((ponder::UserObject(object1) == ponder::UserObject::ref(object1)));
//        REQUIRE((ponder::UserObject(object1) == ponder::UserObject::ref(&object1)));
//        REQUIRE((ponder::UserObject(object1) != ponder::UserObject::copy(object1)));
//        REQUIRE((ponder::UserObject(object1) != ponder::UserObject::copy(&object1)));
//    }
//
//    SECTION("check we can reference non-copyable objects")
//    {
//        // This is a compile check
//        MyNonCopyableClass object;
//        ponder::UserObject userObject(object);
//        MyNonCopyableClass& ref = userObject.get<MyNonCopyableClass>();
//        (void)ref;
//    }
//
//    SECTION("we can refer to concrete objects using abstract base references")
//    {
//        // This is a compile check
//        MyConcreteClass object;
//
//        ponder::UserObject userObject1(object);
//        userObject1.get<MyAbstractClass>();
//
//        ponder::UserObject userObject2(static_cast<MyAbstractClass&>(object));
//        userObject2.get<MyConcreteClass>();
//    }    
//
//    SECTION("objects can be cloned/deep copied")
//    {
//        MyClass object(4);
//        ponder::UserObject userObject = ponder::UserObject::copy(object);
//
//        REQUIRE(userObject.get<MyClass>() == object);  // same value
//        REQUIRE(userObject.get<MyClass*>() != &object); // different address
//    }
//
//    SECTION("objects can referenced/shallow copied")
//    {
//        MyClass object(5);
//        ponder::UserObject userObject = ponder::UserObject::ref(object);
//
//        REQUIRE(userObject.get<MyClass>() == object);  // same value
//        REQUIRE(userObject.get<MyClass*>() ==&object); // same address
//    }
//
//    SECTION("object type information can be inspected")
//    {
//        MyClass object(6);
//        MyBase base(6);
//        MyBase& objectAsBase = object;
//
//        REQUIRE((ponder::UserObject(base).getClass() == ponder::classByType<MyBase>()));
//        REQUIRE((ponder::UserObject(object).getClass() == ponder::classByType<MyClass>()));
//        REQUIRE((ponder::UserObject(objectAsBase).getClass() == ponder::classByType<MyClass>()));
//    }
//
//    SECTION("we can get object values")
//    {
//        MyClass object(7);
//        ponder::UserObject userObject(object);
//
//        REQUIRE(userObject.get("p") == ponder::Value(7));
//    }
//
//    SECTION("we can set object values")
//    {
//        MyClass object(0);
//        ponder::UserObject userObject(object);
//        REQUIRE(userObject.get("p") == ponder::Value(0));
//        
//        userObject.set("p", 8);
//        REQUIRE(object.x == 8);
//    }
//
//    SECTION("object methods can have one argument")
//    {
//        Call object;
//        ponder::UserObject userObject(object);
//
//        REQUIRE_THROWS_AS(userObject.call("meth1"), std::exception); // TODO - wrong num args.
//    
//        userObject.call("meth1", ponder::Args(7));
//        REQUIRE(object.lastCalled == "meth1");
//        REQUIRE(object.sum == 7);
//    }
//
//    SECTION("object methods can have two argument")
//    {
//        Call object;
//        ponder::UserObject userObject(object);
//    
//        REQUIRE_THROWS_AS(userObject.call("meth2"), std::exception);
//        REQUIRE_THROWS_AS(userObject.call("meth2", ponder::Args(11)), std::exception);
////        REQUIRE_THROWS_AS(userObject.call("meth2", ponder::Args(11,2,333)), std::exception); TODO - fix?
//    
//        userObject.call("meth2", ponder::Args(7, 8));
//        REQUIRE(object.lastCalled == "meth2");
//        REQUIRE(object.sum == 7+8);
//    }
//
//    SECTION("object methods can have three argument")
//    {
//        Call object;
//        ponder::UserObject userObject(object);
//    
//        REQUIRE_THROWS_AS(userObject.call("meth3"), std::exception);
//    
//        userObject.call("meth3", ponder::Args(7, 8, -99));
//        REQUIRE(object.lastCalled == "meth3");
//        REQUIRE(object.sum == 7+8-99);
//    }
//
//    SECTION("callMultiArgs8")
//    {
//        Call object;
//        ponder::UserObject userObject(object);
//        
//        REQUIRE_THROWS_AS(userObject.call("meth8"), std::exception);
//        
//        userObject.call("meth8", ponder::Args(7, 8, -99, 77, 12, 76, 45, 3));
//        REQUIRE(object.lastCalled == "meth8");
//        REQUIRE(object.sum == 7+8-99+77+12+76+45+3);
//    }
//
//    SECTION("objects methods can return values")
//    {
//        MyClass object(9);
//        ponder::UserObject userObject(object);
//
//        REQUIRE(userObject.call("f") == ponder::Value(9));
//    }
//
//    SECTION("objects methods with argument can return values")
//    {
//        Call object;
//        ponder::UserObject userObject(object);
//    
//        REQUIRE_THROWS_AS(userObject.call("cos"), std::exception);
//    
//        REQUIRE(userObject.call("cos", ponder::Args(0.0)) == ponder::Value(std::cos(0.0)));
//    //    REQUIRE(userObject.call("cos", 0.0), ponder::Value(std::cos(0.0))); 
//    }
//
//    SECTION("objects methods with 2 args can return values")
//    {
//        Call object;
//        ponder::UserObject userObject(object);
//    
//        REQUIRE_THROWS_AS(userObject.call("concat"), std::exception);
//    
//        REQUIRE(userObject.call("concat", ponder::Args("one", "two")) == ponder::Value("onetwo"));
//    //    REQUIRE(userObject.call("concat", "one", "two"), ponder::Value("onetwo")); TODO - Args optional
//    }
//
//    SECTION("objects can be composed of other objects")
//    {
//        const ponder::UserProperty& p1 =
//            static_cast<const ponder::UserProperty&>(
//                ponder::classByType<Composed1>().property("p") );
//        
//        const ponder::UserProperty& p2 =
//            static_cast<const ponder::UserProperty&>(
//                ponder::classByType<Composed2>().property("p") );
//
//        Composed1 composed1;
//        ponder::UserObject composed2(composed1, p1);
//        ponder::UserObject composed3(composed2, p2);
//
//        composed3.set("x", 1);
//        REQUIRE(composed1.get().get().x ==1);
//        REQUIRE(composed2.get<Composed2>().get().x ==1);
//        REQUIRE(composed3.get<Composed3>().x ==1);
//
//        composed3.set("x", 2);
//        REQUIRE(composed1.get().get().x ==2);
//        REQUIRE(composed2.get<Composed2>().get().x ==2);
//        REQUIRE(composed3.get<Composed3>().x ==2);
//    }    
//}
//
