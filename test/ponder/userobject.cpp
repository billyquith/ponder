/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2018 Nick Trout.
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
#include <ponder/class.hpp>
#include <ponder/classbuilder.hpp>
#include <ponder/uses/runtime.hpp>
#include "test.hpp"
#include <ostream>

#if 0
static bool g_log = false;
#   define DATA_LOG(FMT, ...) if (g_log) printf(FMT, ##__VA_ARGS__)
#   define DATA_ON(B) g_log = B
#else
#   define DATA_LOG(FMT, ...)
#   define DATA_ON(B)
#endif

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
        PONDER_POLYMORPHIC();
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
        PONDER_POLYMORPHIC();
    };
    
    bool operator==(const MyClass& left, const MyClass& right)
    {
        return left.x == right.x;
    }
    
    std::ostream& operator<<(std::ostream& stream, const MyClass& m)
    {
        return stream << m.x;
    }
    
    struct MyNonCopyableClass
    {
        MyNonCopyableClass() {}
        PONDER__NON_COPYABLE(MyNonCopyableClass);
    };
    
    struct MyAbstractClass
    {
        virtual ~MyAbstractClass() {}
        virtual void f() = 0;
        PONDER_POLYMORPHIC();
    };
    
    struct MyConcreteClass : MyAbstractClass
    {
        virtual void f() {}
        PONDER_POLYMORPHIC();
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
        ponder::String lastCalled;
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
        
        ponder::String concat(const ponder::String &a, const ponder::String &b)
        {
            return a+b;
        }
    };
    
    struct Data
    {
        int x;
        
        Data() : x(0) { DATA_LOG("Data:construct\n"); }
        Data(int i) : x(i) { DATA_LOG("Data:construct(%d)\n", x); }
        Data(const Data& d) : x(d.x) {}
        int value() const { return x; }
        
        Data addCopy(const Data& o)
        {
            DATA_LOG("add %d + %d", x, o.x);
            x += o.x;
            DATA_LOG(" = %d\n", x);
            return *this;
        }
        
        const Data& addRef(const Data& o)
        {
            DATA_LOG("add %d + %d", x, o.x);
            x += o.x;
            DATA_LOG(" = %d\n", x);
            return *this;
        }
    };
    
    void declare()
    {
        ponder::Class::declare<MyBase>("UserObjectTest::MyBase");
        ponder::Class::declare<MyClass>("UserObjectTest::MyClass")
            .base<MyBase>()
            .constructor<int>()
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
        
        ponder::Class::declare<Data>("UserObjectTest::Data")
            .constructor()
            .constructor<int>()
            .property("value", &Data::x)
            .function("getValue", &Data::value)
            .function("addCopy", &Data::addCopy)
            .function("addRef", &Data::addRef);
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
PONDER_AUTO_TYPE(UserObjectTest::Data, &UserObjectTest::declare)

using namespace UserObjectTest;

//-----------------------------------------------------------------------------
//                         Tests for ponder::UserObject
//-----------------------------------------------------------------------------

TEST_CASE("User objects reference or contain user data")
{
    SECTION("empty objects are nothing")
    {
        ponder::UserObject obj;
        IS_TRUE(obj == ponder::UserObject::nothing);

        IS_TRUE(ponder::UserObject::nothing == ponder::UserObject::nothing);
    }

    SECTION("destroyed objects are nothing")
    {
        ponder::UserObject obj;
        IS_TRUE(obj == ponder::UserObject::nothing);

        auto const& metaclass = ponder::classByType<MyClass>();
        ponder::runtime::ObjectFactory fact(metaclass);
        obj = fact.construct(ponder::Args(1));
        IS_TRUE(obj != ponder::UserObject::nothing);
        
        fact.destroy(obj);
        IS_TRUE(obj == ponder::UserObject::nothing);
    }

    SECTION("user objects reference objects")
    {
        MyClass object(1);
        ponder::UserObject userObject(&object);

        REQUIRE(userObject.ref<MyClass>() == object);
        REQUIRE(&userObject.ref<MyClass>() == &object);
        
        REQUIRE(userObject.cref<MyClass>() == object);
        REQUIRE(&userObject.cref<MyClass>() == &object);
    }

    SECTION("we can get the value of the user object")
    {
        MyClass object(3);
        ponder::UserObject userObject(&object);

        REQUIRE(userObject.get<MyClass>() == object);
        REQUIRE(userObject.get<MyClass>().x == 3);
    }

    SECTION("user object references should handle inheritance")
    {
        MyClass object(2);
        ponder::UserObject userObject(&object);
        MyBase& base = object;

        REQUIRE(userObject.get<MyBase>() == base);
        REQUIRE(&userObject.get<MyBase>() == &base);
    }
    
    SECTION("user objects can be assigned to")
    {
        MyClass object1(10);
        ponder::UserObject userObject1(&object1);
        ponder::UserObject userObject2;

        REQUIRE(userObject2 != userObject1);

        userObject2 = userObject1;

        REQUIRE(userObject2 == userObject1);
        REQUIRE(userObject2.get<MyClass>() == userObject1.get<MyClass>());
    }

    SECTION("user objects referencing the same object are equal")
    {
        // Note: UserObject equality is related to the object referenced.
        
        MyClass object1(11);    // note, same values here
        MyClass object2(11);

        IS_TRUE(ponder::UserObject(object1) != ponder::UserObject(object1)); // copy
        IS_TRUE(ponder::UserObject(&object1) == ponder::UserObject(&object1));
        IS_TRUE(ponder::UserObject(&object1) != ponder::UserObject(&object2));
    }

    SECTION("check we can reference non-copyable objects")
    {
        // This is a compile check
        MyNonCopyableClass object;
        ponder::UserObject userObject(&object);
        const MyNonCopyableClass& ref = userObject.get<MyNonCopyableClass>();
        (void)&ref;
    }

    SECTION("we can refer to concrete objects using abstract base references")
    {
        // This is a compile check
        MyConcreteClass object;

        ponder::UserObject userObject1(&object);
        userObject1.get<MyAbstractClass>();

        ponder::UserObject userObject2(static_cast<MyAbstractClass*>(&object));
        userObject2.get<MyConcreteClass>();
    }    

    SECTION("objects can be cloned/deep copied")
    {
        MyClass object(4);
        ponder::UserObject uobj1(&object);
        ponder::UserObject uobj2(ponder::UserObject::makeCopy(object));
        
        IS_TRUE(uobj1 != uobj2);

        REQUIRE(uobj1.get<MyClass>() == object);   // same value
        
        REQUIRE(object.x == 4);
        CHECK(uobj1.get<MyClass>().x == 4);
        CHECK(uobj2.get<MyClass>().x == 4);
        
        object.x = 7;
        REQUIRE(object.x == 7);
        CHECK(uobj1.get<MyClass>().x == 7);
        CHECK(uobj2.get<MyClass>().x == 4); // copy hasn't changed
    }

    SECTION("objects can referenced/shallow copied")
    {
        MyClass object(5);
        ponder::UserObject uobj1(&object);
        ponder::UserObject uobj2(ponder::UserObject::makeRef(object));
        
        IS_TRUE(uobj1 == uobj2);
        
        REQUIRE(uobj1.get<MyClass>() == object);   // same value
        
        REQUIRE(object.x == 5);
        CHECK(uobj1.get<MyClass>().x == 5);
        CHECK(uobj2.get<MyClass>().x == 5);
        
        object.x = 7;
        REQUIRE(object.x == 7);
        CHECK(uobj1.get<MyClass>().x == 7);
        CHECK(uobj2.get<MyClass>().x == 7); // copy has changed
    }

    SECTION("object type information can be inspected")
    {
        MyClass object(6);
        MyBase base(6);
        MyBase& objectAsBase = object;

        IS_TRUE(ponder::UserObject(base).getClass() == ponder::classByType<MyBase>());
        IS_TRUE(ponder::UserObject(object).getClass() == ponder::classByType<MyClass>());
        IS_TRUE(ponder::UserObject(objectAsBase).getClass() == ponder::classByType<MyBase>());
    }

    SECTION("we can get property values by name")
    {
        MyClass object(7);
        ponder::UserObject userObject(object);
        REQUIRE(userObject.get("p") == ponder::Value(7));
        
        REQUIRE_THROWS_AS(userObject.get("unfound"), ponder::PropertyNotFound);
    }

    SECTION("we can get property values by index")
    {
        MyClass object(3);
        ponder::UserObject userObject(object);
        REQUIRE(userObject.get(0) == ponder::Value(3));
        
        REQUIRE_THROWS_AS(userObject.get(-1), ponder::OutOfRange);
        REQUIRE_THROWS_AS(userObject.get(1), ponder::OutOfRange);
        REQUIRE_THROWS_AS(userObject.get(100), ponder::OutOfRange);
    }

    SECTION("we can set property values by name")
    {
        MyClass object(0);
        ponder::UserObject userObject(&object);
        REQUIRE(userObject.get("p") == ponder::Value(0));        
        userObject.set("p", 8);
        REQUIRE(object.x == 8);
        
        REQUIRE_THROWS_AS(userObject.set("unfound", 7), ponder::PropertyNotFound);
    }

    SECTION("we can set property values by index")
    {
        MyClass object(4);
        ponder::UserObject userObject(&object);
        REQUIRE(userObject.get(0) == ponder::Value(4));
        userObject.set(0, 8);
        REQUIRE(object.x == 8);
        
        REQUIRE_THROWS_AS(userObject.set(-1, 11), ponder::OutOfRange);
        REQUIRE_THROWS_AS(userObject.set(1, 27), ponder::OutOfRange);
    }

    SECTION("we can iterate over properties")
    {
        MyClass object(3);
        ponder::UserObject userObject(&object);
        
        int index = 0;
        for (auto&& prop : ponder::classByType<MyClass>().propertyIterator())
        {
            switch (index) {
            case 0:
                REQUIRE(prop.name() == ponder::String("p"));
                REQUIRE(prop.value()->name() == ponder::String("p"));
                break;
            default: ;
            }
        }
    }

//    SECTION("object methods can have one parameter")
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

//    SECTION("object methods can have two parameter")
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
//    SECTION("object methods can have three parameter")
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
//    SECTION("objects methods with parameter can return values")
//    {
//        Call object;
//        ponder::UserObject userObject(object);
//    
//        REQUIRE_THROWS_AS(userObject.call("cos"), std::exception);
//    
//        REQUIRE(userObject.call("cos", ponder::Args(0.0)) == ponder::Value(std::cos(0.0)));
//        REQUIRE(userObject.call("cos", ponder::Args(1.0)) == ponder::Value(std::cos(1.0)));
//    //    REQUIRE(userObject.call("cos", 0.0), ponder::Value(std::cos(0.0)));
//    }
//
//    SECTION("objects methods can return user objects")
//    {
//        Call object;
//        ponder::UserObject userObject(object);
//    
//        REQUIRE_THROWS_AS(userObject.call("concat"), std::exception);
//    
//        REQUIRE(userObject.call("concat", ponder::Args("one", "two")) == ponder::Value("onetwo"));
//    //    REQUIRE(userObject.call("concat", "one", "two"), ponder::Value("onetwo")); TODO - Args optional
//    }
    
    SECTION("objects can created from existing user data")
    {
        MyClass object(77);
        
        auto const& metacls = ponder::classByType<MyClass>();
        void* ptr = &object;
        
        ponder::UserObject uo( metacls.getUserObjectFromPointer(ptr) );
        REQUIRE(uo.get("p") == ponder::Value(77));
        
        uo.set("p", 21);
        REQUIRE(object.x == 21);
        REQUIRE(ponder::Value(object.x) == uo.get("p"));
    }
}


TEST_CASE("User objects wrap C++ objects")
{
    auto const& metacls = ponder::classByType<Data>();
    
//    SECTION("constructors")
//    {
//        ponder::UserObject uo = metacls.construct();
//        IS_TRUE(uo != ponder::UserObject::nothing);
//        REQUIRE(uo.get<Data>().x == 0);
//    }
//
//    SECTION("constructors with args")
//    {
//        ponder::UserObject uo = metacls.construct(77);
//        IS_TRUE(uo != ponder::UserObject::nothing);
//        REQUIRE(uo.get<Data>().x == 77);
//    }
//
//    SECTION("return user object by copy")
//    {
//        //DATA_ON(true);
//        
//        ponder::UserObject uo1 = metacls.construct(3);
//        ponder::UserObject uo2 = metacls.construct(6);
//        IS_TRUE(uo1 != ponder::UserObject::nothing);
//        IS_TRUE(uo2 != ponder::UserObject::nothing);
//        IS_TRUE(uo1 != uo2);
//        REQUIRE(uo1.get<Data>().x == 3);
//        REQUIRE(uo2.get<Data>().x == 6);
//        
//        ponder::Value uoa = uo1.call("addCopy", ponder::Args(uo2));
//        REQUIRE(uoa.kind() == ponder::ValueKind::User);
//        REQUIRE(uoa.to<Data*>()->x == 9);
//    }
//
//    SECTION("return user object by ref")
//    {
//        //DATA_ON(true);
//        
//        ponder::UserObject uo1 = metacls.construct(3);
//        ponder::UserObject uo2 = metacls.construct(6);
//        IS_TRUE(uo1 != ponder::UserObject::nothing);
//        IS_TRUE(uo2 != ponder::UserObject::nothing);
//        IS_TRUE(uo1 != uo2);
//        REQUIRE(uo1.get<Data>().x == 3);
//        REQUIRE(uo2.get<Data>().x == 6);
//        
//        ponder::Value uoa = uo1.call("addRef", ponder::Args(uo2));
//        REQUIRE(uoa.kind() == ponder::ValueKind::User);
//        REQUIRE(uoa.to<Data*>()->x == 9);
//    }
}


