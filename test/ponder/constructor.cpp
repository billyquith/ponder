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
#include <ponder/class.hpp>
#include <ponder/enum.hpp>
#include <ponder/classbuilder.hpp>
#include <ponder/uses/runtime.hpp>
#include "test.hpp"
#include <string>
#include <string.h> // memset

namespace ConstructorTest
{
    enum MyEnum
    {
        zero  = 0,
        one   = 1,
        two   = 2,
        three = 3,
        four  = 4,
        five  = 5
    };
    
    struct MyType
    {
        MyType(int x_) : x(x_) {}
        int x;
    };
    
    struct MyBase1
    {
        MyBase1() : base1("base1") {}
        virtual ~MyBase1() {}
        ponder::String base1;
    };
    
    struct MyBase2
    {
        MyBase2() : base2("base2") {}
        virtual ~MyBase2() {}
        ponder::String base2;
    };
    
    struct MyClass : MyBase1, MyBase2
    {
        static int instCount;
        
        MyClass()
            : l(0), r(0.), s("0"), e(zero), u(0) {++instCount;}
        
        MyClass(long l_)
            : l(l_), r(1.), s("1"), e(one), u(1) {++instCount;}
        
        MyClass(long l_, double r_)
            : l(l_), r(r_), s("2"), e(two), u(2) {++instCount;}
        
        MyClass(long l_, double r_, ponder::String s_)
            : l(l_), r(r_), s(s_), e(three), u(3) {++instCount;}
        
        MyClass(long l_, double r_, ponder::String s_, MyEnum e_)
            : l(l_), r(r_), s(s_), e(e_), u(4) {++instCount;}
        
        MyClass(long l_, double r_, ponder::String s_, MyEnum e_, MyType t_)
            : l(l_), r(r_), s(s_), e(e_), u(t_) {++instCount;}
        
        ~MyClass() {--instCount;}
        
        long l;
        double r;
        ponder::String s;
        MyEnum e;
        MyType u;
    };
    
    int MyClass::instCount = 0;
    
    void declare()
    {
        ponder::Enum::declare<MyEnum>("ConstructorTest::MyEnum")
            .value("zero",  zero)
            .value("one",   one)
            .value("two",   two)
            .value("three", three)
            .value("four",  four)
            .value("five",  five);
        
        ponder::Class::declare<MyType>("ConstructorTest::MyType");
        
        ponder::Class::declare<MyBase1>("ConstructorTest::MyBase1");
        ponder::Class::declare<MyBase2>("ConstructorTest::MyBase2");
        
        ponder::Class::declare<MyClass>("ConstructorTest::MyClass")
            .base<MyBase1>()
            .base<MyBase2>()
            .constructor()
            .constructor<long>()
            .constructor<long, double>()
            .constructor<long, double, ponder::String>()
            .constructor<long, double, ponder::String, MyEnum>()        
            // trying types that don't exactly match those declared
            .constructor<unsigned short, float, ponder::String, MyEnum, int>();
    }
}

PONDER_AUTO_TYPE(ConstructorTest::MyEnum, &ConstructorTest::declare)
PONDER_AUTO_TYPE(ConstructorTest::MyType, &ConstructorTest::declare)
PONDER_AUTO_TYPE(ConstructorTest::MyBase1, &ConstructorTest::declare)
PONDER_AUTO_TYPE(ConstructorTest::MyBase2, &ConstructorTest::declare)
PONDER_AUTO_TYPE(ConstructorTest::MyClass, &ConstructorTest::declare)


using namespace ConstructorTest;

//-----------------------------------------------------------------------------
//                      Tests for ponder::Class::construct
//-----------------------------------------------------------------------------

TEST_CASE("Object factories can be used to create class instances") // and allocate dynamically
{
    const ponder::Class& metaclass = ponder::classByType<MyClass>();
    const std::size_t sz = metaclass.sizeOf();
    
    REQUIRE(sz > 0);
    REQUIRE(sz == sizeof(MyClass));

    SECTION("using constuct/destroy")
    {
        REQUIRE(MyClass::instCount == 0);
        {
            ponder::UserObject object;
            
            REQUIRE(MyClass::instCount == 0);
            IS_TRUE( object == ponder::UserObject::nothing );
            
            ponder::runtime::ObjectFactory fact(metaclass);
            object = fact.construct(ponder::Args(777, 99.05f, "wow"));
            
            REQUIRE(MyClass::instCount == 1);
            IS_TRUE( object != ponder::UserObject::nothing );
            
            MyClass* instance = object.get<MyClass*>();
            
            REQUIRE(instance->l == 777);
            REQUIRE(instance->r == Approx(99.05).epsilon(1E-5));
            REQUIRE(instance->s == "wow");
            REQUIRE(instance->e == three);
            REQUIRE(instance->u.x == 3);
            
            fact.destroy(object);
        }
        REQUIRE(MyClass::instCount == 0);        
    }
    
    SECTION("using placement new")
    {
        REQUIRE(MyClass::instCount == 0);
        {
            char buff[sizeof(MyClass) + 20];
            const char c_guard{ (char)0xcd };
            memset(buff, c_guard, sizeof(buff));
            char *p = buff + 4;
            
            REQUIRE(buff[0] == c_guard);
            REQUIRE(*p == c_guard);
            REQUIRE(p[sz] == c_guard);
            
            ponder::runtime::ObjectFactory fact(metaclass);
            ponder::UserObject object = fact.construct(ponder::Args(), p); // placement new
            
            IS_TRUE( object != ponder::UserObject::nothing );
            REQUIRE(MyClass::instCount == 1);
            
            REQUIRE(buff[0] == c_guard);
            REQUIRE(*p != c_guard);
            REQUIRE(p[sz] == c_guard);
            
            MyClass* instance = object.get<MyClass*>();
            
            REQUIRE(instance == reinterpret_cast<MyClass*>(p));
            
            REQUIRE(instance->l == 0);
            REQUIRE(instance->r == Approx(0.).epsilon(1E-5));
            REQUIRE(instance->s == "0");
            REQUIRE(instance->e == zero);
            REQUIRE(instance->u.x == 0);
            
            fact.destruct(object); // not destroy()
        }
        REQUIRE(MyClass::instCount == 0);
    }
    
    SECTION("using create/destroy")
    {
        REQUIRE(MyClass::instCount == 0);
        {
            ponder::UserObject object;
            
            IS_TRUE( object == ponder::UserObject::nothing );
            
            ponder::runtime::ObjectFactory fact(metaclass);
            object = fact.create(777, 99.05f, "wow");
            
            IS_TRUE( object != ponder::UserObject::nothing );
            REQUIRE(MyClass::instCount == 1);
            
            MyClass* instance = object.get<MyClass*>();
            
            REQUIRE(instance->l == 777);
            REQUIRE(instance->r == Approx(99.05).epsilon(1E-5));
            REQUIRE(instance->s == "wow");
            REQUIRE(instance->e == three);
            REQUIRE(instance->u.x == 3);
            
            fact.destroy(object);
        }
        REQUIRE(MyClass::instCount == 0);
    }
    
    SECTION("using create & auto pointer")
    {
        REQUIRE(MyClass::instCount == 0);
        {
            auto object = ponder::runtime::createUnique(metaclass, 777, 99.05f, "wow");
            
            IS_TRUE( *object.get() != ponder::UserObject::nothing );
            
            MyClass* instance = object.get()->get<MyClass*>();
            
            REQUIRE(instance->l == 777);
            REQUIRE(instance->r == Approx(99.05).epsilon(1E-5));
            REQUIRE(instance->s == "wow");
            REQUIRE(instance->e == three);
            REQUIRE(instance->u.x == 3);
        }
        REQUIRE(MyClass::instCount == 0);
    }
}

//-----------------------------------------------------------------------------
//                      Tests for ponder::Class::construct
//-----------------------------------------------------------------------------

TEST_CASE("Classes can have constructors") // and allocate dynamically
{
    const ponder::Class* metaclass = &ponder::classByType<MyClass>();
    
    REQUIRE(metaclass != nullptr);

    SECTION("with no parameters")
    {
        ponder::UserObject object;
        
        IS_TRUE( object == ponder::UserObject::nothing );
        
        ponder::runtime::ObjectFactory fact(*metaclass);
        object = fact.construct();
        
        IS_TRUE( object != ponder::UserObject::nothing );

        MyClass* instance = object.get<MyClass*>();

        REQUIRE(instance->l == 0);
        REQUIRE(instance->r == Approx(0.).epsilon(1E-5));
        REQUIRE(instance->s == "0");
        REQUIRE(instance->e == zero);
        REQUIRE(instance->u.x == 0);

        fact.destroy(object);
    }

    SECTION("with one parameter")
    {
        ponder::UserObject object;
        
        IS_TRUE( object == ponder::UserObject::nothing );
        
        ponder::runtime::ObjectFactory fact(*metaclass);
        object = fact.construct(ponder::Args(1));

        MyClass* instance = object.get<MyClass*>();

        REQUIRE(instance->l == 1);
        REQUIRE(instance->r == Approx(1.).epsilon(1E-5));
        REQUIRE(instance->s == "1");
        REQUIRE(instance->e == one);
        REQUIRE(instance->u.x == 1);

        fact.destroy(object);
    }

    SECTION("with two parameters")
    {
        ponder::UserObject object;
        
        IS_TRUE( object == ponder::UserObject::nothing );
        
        ponder::runtime::ObjectFactory fact(*metaclass);
        object = fact.construct(ponder::Args(2, 2.));

        MyClass* instance = object.get<MyClass*>();

        REQUIRE(instance->l == 2);
        REQUIRE(instance->r == Approx(2.).epsilon(1E-5));
        REQUIRE(instance->s == "2");
        REQUIRE(instance->e == two);
        REQUIRE(instance->u.x == 2);

        fact.destroy(object);
    }


    SECTION("with three parameters")
    {
        ponder::UserObject object;
        
        IS_TRUE( object == ponder::UserObject::nothing );
        
        ponder::runtime::ObjectFactory fact(*metaclass);
        object = fact.construct(ponder::Args(3, 3., "3"));

        MyClass* instance = object.get<MyClass*>();

        REQUIRE(instance->l == 3);
        REQUIRE(instance->r == Approx(3.).epsilon(1E-5));
        REQUIRE(instance->s == "3");
        REQUIRE(instance->e == three);
        REQUIRE(instance->u.x == 3);

        fact.destroy(object);
    }

    SECTION("with four parameters")
    {
        ponder::UserObject object;
        
        IS_TRUE( object == ponder::UserObject::nothing );
        
        ponder::runtime::ObjectFactory fact(*metaclass);
        object = fact.construct(ponder::Args(4, 4., "4", four));

        MyClass* instance = object.get<MyClass*>();

        REQUIRE(instance->l == 4);
        REQUIRE(instance->r == Approx(4.).epsilon(1E-5));
        REQUIRE(instance->s == "4");
        REQUIRE(instance->e == four);
        REQUIRE(instance->u.x == 4);

        fact.destroy(object);
    }

    SECTION("with five parameters")
    {
        ponder::UserObject object;
        
        IS_TRUE( object == ponder::UserObject::nothing );
        
        ponder::runtime::ObjectFactory fact(*metaclass);
        object = fact.construct(ponder::Args(5, 5., "5", five, 5));

        MyClass* instance = object.get<MyClass*>();

        REQUIRE(instance->l == 5);
        REQUIRE(instance->r == Approx(5.).epsilon(1E-5));
        REQUIRE(instance->s == "5");
        REQUIRE(instance->e == five);
        REQUIRE(instance->u.x == 5);

        fact.destroy(object);
    }

    SECTION("with invalid parameters")
    {
        ponder::runtime::ObjectFactory fact(*metaclass);

        IS_TRUE( fact.construct(ponder::Args("hello")) == ponder::UserObject::nothing );
        IS_TRUE( fact.construct(ponder::Args(MyType(10))) == ponder::UserObject::nothing );
        IS_TRUE( fact.construct(ponder::Args(two, MyType(10))) == ponder::UserObject::nothing );
        IS_TRUE( fact.construct(ponder::Args(5., "hello")) == ponder::UserObject::nothing );
    }
}


//TEST_CASE("Object factories can be used to create class instances") // and allocate dynamically
//{
//    const ponder::Class &metaclass = ponder::classByType<MyClass>();
//    
//    SECTION("with no parameters")
//    {
////        ponder::UserObject object;
//        auto object = ponder::runtime::createUnique(const ponder::Class &cls, A args...)
//        
//        IS_TRUE( object == ponder::UserObject::nothing );
//        
//        ponder::runtime::ObjectFactory fact(metaclass);
//        object = fact.create();
//        
//        IS_TRUE( object != ponder::UserObject::nothing );
//        
//        MyClass* instance = object.get<MyClass*>();
//        
//        REQUIRE(instance->l == 0);
//        REQUIRE(instance->r == Approx(0.).epsilon(1E-5));
//        REQUIRE(instance->s == "0");
//        REQUIRE(instance->e == zero);
//        REQUIRE(instance->u.x == 0);
//        
//        fact.destroy(object);
//    }
//    
//    SECTION("with one parameter")
//    {
//        ponder::runtime::ObjectFactory fact(metaclass);
//        ponder::UserObject object = fact.construct(ponder::Args(1));
//        
//        IS_TRUE( object != ponder::UserObject::nothing );
//        
//        MyClass* instance = object.get<MyClass*>();
//        
//        REQUIRE(instance->l == 1);
//        REQUIRE(instance->r == Approx(1.).epsilon(1E-5));
//        REQUIRE(instance->s == "1");
//        REQUIRE(instance->e == one);
//        REQUIRE(instance->u.x == 1);
//        
//        fact.destroy(object);
//    }

//    SECTION("with two parameters")
//    {
//        ponder::UserObject object = metaclass->construct(ponder::Args(2, 2.));
//        
//        IS_TRUE( object != ponder::UserObject::nothing );
//        
//        MyClass* instance = object.get<MyClass*>();
//        
//        REQUIRE(instance->l == 2);
//        REQUIRE(instance->r == Approx(2.).epsilon(1E-5));
//        REQUIRE(instance->s == "2");
//        REQUIRE(instance->e == two);
//        REQUIRE(instance->u.x == 2);
//        
//        metaclass->destroy(object);
//    }
//    
//    
//    SECTION("with three parameters")
//    {
//        ponder::UserObject object = metaclass->construct(ponder::Args(3, 3., "3"));
//        
//        IS_TRUE( object != ponder::UserObject::nothing );
//        
//        MyClass* instance = object.get<MyClass*>();
//        
//        REQUIRE(instance->l == 3);
//        REQUIRE(instance->r == Approx(3.).epsilon(1E-5));
//        REQUIRE(instance->s == "3");
//        REQUIRE(instance->e == three);
//        REQUIRE(instance->u.x == 3);
//        
//        metaclass->destroy(object);
//    }
//    
//    SECTION("with four parameters")
//    {
//        ponder::UserObject object = metaclass->construct(ponder::Args(4, 4., "4", four));
//        
//        IS_TRUE( object != ponder::UserObject::nothing );
//        
//        MyClass* instance = object.get<MyClass*>();
//        
//        REQUIRE(instance->l == 4);
//        REQUIRE(instance->r == Approx(4.).epsilon(1E-5));
//        REQUIRE(instance->s == "4");
//        REQUIRE(instance->e == four);
//        REQUIRE(instance->u.x == 4);
//        
//        metaclass->destroy(object);
//    }
//    
//    SECTION("with five parameters")
//    {
//        ponder::UserObject object = metaclass->construct(ponder::Args(5, 5., "5", five, 5));
//        
//        IS_TRUE( object != ponder::UserObject::nothing );
//        
//        MyClass* instance = object.get<MyClass*>();
//        
//        REQUIRE(instance->l == 5);
//        REQUIRE(instance->r == Approx(5.).epsilon(1E-5));
//        REQUIRE(instance->s == "5");
//        REQUIRE(instance->e == five);
//        REQUIRE(instance->u.x == 5);
//        
//        metaclass->destroy(object);
//    }
    
//}


TEST_CASE("Object factory constructors can use placement new")
{
    const ponder::Class &metaclass = ponder::classByType<MyClass>();
    const std::size_t sz = metaclass.sizeOf();
    
    REQUIRE(sz > 0);
    REQUIRE(sz == sizeof(MyClass));
    
    SECTION("with no parameters")
    {
        char buff[sizeof(MyClass) + 20];
        const char c_guard{ (char)0xcd };
        memset(buff, c_guard, sizeof(buff));
        char *p = buff + 4;
        
        REQUIRE(buff[0] == c_guard);
        REQUIRE(*p == c_guard);
        REQUIRE(p[sz] == c_guard);
        
        ponder::runtime::ObjectFactory fact(metaclass);
        ponder::UserObject object = fact.construct(ponder::Args(), p); // placement new
        
        IS_TRUE( object != ponder::UserObject::nothing );
        
        REQUIRE(buff[0] == c_guard);
        REQUIRE(*p != c_guard);
        REQUIRE(p[sz] == c_guard);
        
        MyClass* instance = object.get<MyClass*>();
        
        REQUIRE(instance == reinterpret_cast<MyClass*>(p));
        
        REQUIRE(instance->l == 0);
        REQUIRE(instance->r == Approx(0.).epsilon(1E-5));
        REQUIRE(instance->s == "0");
        REQUIRE(instance->e == zero);
        REQUIRE(instance->u.x == 0);
        
        fact.destruct(object); // not destroy()
    }
}

//-----------------------------------------------------------------------------
//                     Tests for ponder::Class::create
//-----------------------------------------------------------------------------

//TEST_CASE("Classes can create object") // no arg list
//{
//    const ponder::Class* metaclass = &ponder::classByType<MyClass>();
//    
//    REQUIRE(metaclass != nullptr);
//    
//    SECTION("with no parameters")
//    {
//        ponder::UserObject object;
//        
//        IS_TRUE( object == ponder::UserObject::nothing );
//        
//        object = metaclass->create();
//        
//        IS_TRUE( object != ponder::UserObject::nothing );
//        
//        MyClass *instance = object.get<MyClass*>();
//        
//        REQUIRE(instance->l == 0);
//        REQUIRE(instance->r == Approx(0.).epsilon(1E-5));
//        REQUIRE(instance->s == "0");
//        REQUIRE(instance->e == zero);
//        REQUIRE(instance->u.x == 0);
//        
//        metaclass->destroy(object);
//    }
//    
//    SECTION("with one parameter")
//    {
//        ponder::UserObject object = metaclass->create(1);
//        
//        IS_TRUE( object != ponder::UserObject::nothing );
//        
//        MyClass* instance = object.get<MyClass*>();
//        
//        REQUIRE(instance->l == 1);
//        REQUIRE(instance->r == Approx(1.).epsilon(1E-5));
//        REQUIRE(instance->s == "1");
//        REQUIRE(instance->e == one);
//        REQUIRE(instance->u.x == 1);
//        
//        metaclass->destroy(object);
//    }
//    
//    SECTION("with two parameters")
//    {
//        ponder::UserObject object = metaclass->create(2, 2.);
//        
//        IS_TRUE( object != ponder::UserObject::nothing );
//        
//        MyClass* instance = object.get<MyClass*>();
//        
//        REQUIRE(instance->l == 2);
//        REQUIRE(instance->r == Approx(2.).epsilon(1E-5));
//        REQUIRE(instance->s == "2");
//        REQUIRE(instance->e == two);
//        REQUIRE(instance->u.x == 2);
//        
//        metaclass->destroy(object);
//    }
//    
//    
//    SECTION("with three parameters")
//    {
//        ponder::UserObject object = metaclass->create(3, 3., "3");
//        
//        IS_TRUE( object != ponder::UserObject::nothing );
//        
//        MyClass* instance = object.get<MyClass*>();
//        
//        REQUIRE(instance->l == 3);
//        REQUIRE(instance->r == Approx(3.).epsilon(1E-5));
//        REQUIRE(instance->s == "3");
//        REQUIRE(instance->e == three);
//        REQUIRE(instance->u.x == 3);
//        
//        metaclass->destroy(object);
//    }
//    
//    SECTION("with four parameters")
//    {
//        ponder::UserObject object = metaclass->create(4, 4., "4", four);
//        
//        IS_TRUE( object != ponder::UserObject::nothing );
//        
//        MyClass* instance = object.get<MyClass*>();
//        
//        REQUIRE(instance->l == 4);
//        REQUIRE(instance->r == Approx(4.).epsilon(1E-5));
//        REQUIRE(instance->s == "4");
//        REQUIRE(instance->e == four);
//        REQUIRE(instance->u.x == 4);
//        
//        metaclass->destroy(object);
//    }
//    
//    SECTION("with five parameters")
//    {
//        ponder::UserObject object = metaclass->create(5, 5., "5", five, 5);
//        
//        IS_TRUE( object != ponder::UserObject::nothing );
//        
//        MyClass* instance = object.get<MyClass*>();
//        
//        REQUIRE(instance->l == 5);
//        REQUIRE(instance->r == Approx(5.).epsilon(1E-5));
//        REQUIRE(instance->s == "5");
//        REQUIRE(instance->e == five);
//        REQUIRE(instance->u.x == 5);
//        
//        metaclass->destroy(object);
//    }
//    
//    SECTION("with invalid parameters")
//    {
//        IS_TRUE( metaclass->create("hello") == ponder::UserObject::nothing );
//        IS_TRUE( metaclass->create(MyType(10)) == ponder::UserObject::nothing );
//        IS_TRUE( metaclass->create(two, MyType(10)) == ponder::UserObject::nothing );
//        IS_TRUE( metaclass->create(5., "hello") == ponder::UserObject::nothing );
//    }    
//}


