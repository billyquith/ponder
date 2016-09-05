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

#include <ponder/classbuilder.hpp>
#include <ponder/uses/runtime.hpp>
#include "test.hpp"

namespace FunctionTest
{
    using namespace std::placeholders;
    
    enum MyEnum
    {
        Zero = 0,
        One  = 1,
        Two  = 2
    };
    
    struct MyType
    {
        MyType(int x_) : x(x_) {}
        int x;
    };
    
    struct NonCopyable
    {
        NonCopyable() = default;
        NonCopyable(const NonCopyable&) = delete;
    };
    
    static bool operator == (const MyType& left, const MyType& right)
    {
        return left.x == right.x;
    }
    
    struct MyBase
    {
        void f6() {}
        char padding[10];
    };
    
    struct MyClass : MyBase
    {
        MyClass()
        : p1(true)
        , p2(2)
        , p3("3")
        , p4(MyType(4))
        , p5(MyType(5))
        , innerPtr(&inner)
        , innerSmartPtr(new Inner)
        {
        }
        
        bool p1;
        int p2;
        ponder::String p3;
        
        MyType p4;
        const MyType& f4() {return p4;}
        
        MyType p5;
        const MyType& f5() const {return p5;}
        
        // f6 is inherited
        ponder::Value f7(ponder::Value v) {return v;}
        
        void f8() {}
        void f9(bool) {}
        void f10(float, double) {}
        void f11(short, int, long) {}
        void f12(const ponder::String&, ponder::String, const ponder::String&, ponder::String) {}
        void f13(MyEnum, MyEnum, MyEnum, MyEnum, MyEnum) {}
        
        struct Inner
        {
            void f14() {}
            void f15() const {}
            int f16() {return 16;}
            void f17(int) const {}
            void f18() {}
            void f19() {}
        };
        
        Inner inner;
        const Inner& getInner() const {return inner;}
        
        Inner* innerPtr;
        const Inner* getInnerPtr() const {return innerPtr;}
        
        std::shared_ptr<Inner> innerSmartPtr;
        const std::shared_ptr<Inner> getInnerSmartPtr() {return innerSmartPtr;}
        
        int f20(int x) {return x;}
        int f21(int x, int y) {return x + y;}
        int f22(int x, int y, int z) {return x + y + z;}
        
        static int staticFunc()
        {
            return 77;
        }
        
        static float staticFunc2(float a, float b)
        {
            return a * b;
        }
        
        static NonCopyable& staticFuncRetRef()
        {
            static NonCopyable nc;
            return nc;
        }
        
        static NonCopyable* staticFuncRetPtr()
        {
            static NonCopyable nc;
            return &nc;
        }
    };
    
    void f1(MyClass* object)  // TODO - allow non-const refs
    {
        object->p1 = true;
    }
    
    int f2(MyClass object, int x)
    {
        return object.p2 + x;
    }
    
    const ponder::String& f3(const MyClass* object)
    {
        return object->p3;
    }
    
    
    class ClassA
    {
    public:
        ClassA() : TestMember(0) {}
        int TestMember;
    };
    
    class ClassB
    {
    public:
        void ModifyA(ClassA* obj) { obj->TestMember = 5; }
    };
    
    void declare()
    {
        ponder::Enum::declare<MyEnum>("FunctionTest::MyEnum")
            .value("Zero", Zero)
            .value("One",  One)
            .value("Two",  Two);
        
        ponder::Class::declare<MyType>("FunctionTest::MyType");

        ponder::Class::declare<NonCopyable>();

        ponder::Class::declare<MyBase>("FunctionTest::MyBase");
        
        ponder::Class::declare<MyClass>()
            .base<MyBase>()
            
            // ***** non-member functions *****
            .function("f1", &f1) // object by reference
            .function("f2", &f2) // object by value + parameter
            .function("f3", &f3) // object by pointer
            
            // ***** member functions *****
            .function("f4", &MyClass::f4) // non-const
            .function("f5", &MyClass::f5) // const
            .function("f6", &MyClass::f6) // inherited
            .function("f7", &MyClass::f7) // ponder::Value as return and parameter types
            
            // ***** parameters count ******
            .function("f8",  &MyClass::f8)  // 0 parameter
            .function("f9",  &MyClass::f9)  // 1 parameter
            .function("f10", &MyClass::f10) // 2 parameters
            .function("f11", &MyClass::f11) // 3 parameters
            .function("f12", &MyClass::f12) // 4 parameters
            .function("f13", &MyClass::f13) // 5 parameters
            
            // ***** nested functions *****
            .function("f14", [](MyClass self){ self.inner.f14(); }) // XXXX modifying copy
            // getter returning an object
            .function("f15", [](MyClass self){ self.getInner().f15(); })
            // raw pointer
            .function("f16", [](MyClass self){ return self.innerPtr->f16(); })
            // getter returning a raw pointer
            .function("f17", [](MyClass self){ self.getInnerPtr()->f17(99); })
            // smart pointer
            .function("f18", [](MyClass self){ self.innerSmartPtr.get()->f18(); })
            // getter returning a smart pointer
            .function("f19", [](MyClass self){ self.getInnerSmartPtr().get()->f19(); })
        
            // ***** std::function *****
            .function("f20",
                      std::function<int (MyClass, int)>(std::bind(&MyClass::f20, _1, _2)))
            .function("f21",
                      std::function<int (MyClass, int)>(std::bind(&MyClass::f21, _1, _2, 20)))
            .function("f22",
                      std::function<int (MyClass, int)>(
                          std::bind(std::bind(&MyClass::f22, _1, _2, _3, 30), _1, _2, 20)))
            .function("statFunc", &MyClass::staticFunc)
            .function("statFunc2", &MyClass::staticFunc2)
        
            .function("nonCopyRef", &MyClass::staticFuncRetRef)
            .function("nonCopyPtr", &MyClass::staticFuncRetPtr)
            ;
        
        ponder::Class::declare<ClassA>()
            .constructor()
            .property("TestMember", &ClassA::TestMember);
        
        ponder::Class::declare<ClassB>()
            .constructor()
            .function("ModifyA", &ClassB::ModifyA);
    }
}

PONDER_AUTO_TYPE(FunctionTest::MyEnum,  &FunctionTest::declare)
PONDER_AUTO_TYPE(FunctionTest::MyType,  &FunctionTest::declare)
PONDER_AUTO_TYPE(FunctionTest::MyClass, &FunctionTest::declare)
PONDER_AUTO_TYPE(FunctionTest::NonCopyable, &FunctionTest::declare)
PONDER_AUTO_TYPE(FunctionTest::MyBase,  &FunctionTest::declare)
PONDER_AUTO_TYPE(FunctionTest::ClassA,  &FunctionTest::declare)
PONDER_AUTO_TYPE(FunctionTest::ClassB,  &FunctionTest::declare)

using namespace FunctionTest;

//-----------------------------------------------------------------------------
//                         Tests for ponder::Function
//-----------------------------------------------------------------------------

TEST_CASE("Functions can be registered")
{
    const ponder::Function* functions[23];
    
    const ponder::Class& metaclass = ponder::classByType<MyClass>();
    for (int i = 1; i <= 22; ++i)
    {
        functions[i] = &metaclass.function("f" + std::to_string(i));
    }

    SECTION("functions are classified")
    {
        IS_TRUE(functions[1]->classification() == ponder::FunctionFamily::Function);
        IS_TRUE(functions[2]->classification() == ponder::FunctionFamily::Function);
        IS_TRUE(functions[3]->classification() == ponder::FunctionFamily::Function);
        
        IS_TRUE(functions[4]->classification() == ponder::FunctionFamily::MemberFunction);
        IS_TRUE(functions[5]->classification() == ponder::FunctionFamily::MemberFunction);
        IS_TRUE(functions[6]->classification() == ponder::FunctionFamily::MemberFunction);
        IS_TRUE(functions[7]->classification() == ponder::FunctionFamily::MemberFunction);
        
        IS_TRUE(functions[8]->classification() == ponder::FunctionFamily::MemberFunction);
        IS_TRUE(functions[9]->classification() == ponder::FunctionFamily::MemberFunction);
        IS_TRUE(functions[10]->classification() == ponder::FunctionFamily::MemberFunction);
        IS_TRUE(functions[11]->classification() == ponder::FunctionFamily::MemberFunction);
        IS_TRUE(functions[12]->classification() == ponder::FunctionFamily::MemberFunction);
        IS_TRUE(functions[13]->classification() == ponder::FunctionFamily::MemberFunction);
        
        IS_TRUE(functions[14]->classification() == ponder::FunctionFamily::Lambda);
        IS_TRUE(functions[15]->classification() == ponder::FunctionFamily::Lambda);
        IS_TRUE(functions[16]->classification() == ponder::FunctionFamily::Lambda);
        IS_TRUE(functions[17]->classification() == ponder::FunctionFamily::Lambda);
        IS_TRUE(functions[18]->classification() == ponder::FunctionFamily::Lambda);
        IS_TRUE(functions[19]->classification() == ponder::FunctionFamily::Lambda);
        
        IS_TRUE(functions[20]->classification() == ponder::FunctionFamily::FunctionWrapper);
        IS_TRUE(functions[21]->classification() == ponder::FunctionFamily::FunctionWrapper);
        IS_TRUE(functions[22]->classification() == ponder::FunctionFamily::FunctionWrapper);

        IS_TRUE(metaclass.function("nonCopyRef").classification()
                == ponder::FunctionFamily::Function);
        IS_TRUE(metaclass.function("nonCopyPtr").classification()
                == ponder::FunctionFamily::Function);
    }

    SECTION("functions have a return type")
    {
        REQUIRE(functions[1]->returnType() ==  ponder::ValueType::None);
        REQUIRE(functions[2]->returnType() ==  ponder::ValueType::Integer);
        REQUIRE(functions[3]->returnType() ==  ponder::ValueType::String);
        REQUIRE(functions[4]->returnType() ==  ponder::ValueType::User);
        REQUIRE(functions[5]->returnType() ==  ponder::ValueType::User);
        REQUIRE(functions[6]->returnType() ==  ponder::ValueType::None);
        REQUIRE(functions[7]->returnType() ==  ponder::ValueType::User);
        REQUIRE(functions[8]->returnType() ==  ponder::ValueType::None);
        REQUIRE(functions[9]->returnType() ==  ponder::ValueType::None);
        REQUIRE(functions[10]->returnType() == ponder::ValueType::None);
        REQUIRE(functions[11]->returnType() == ponder::ValueType::None);
        REQUIRE(functions[12]->returnType() == ponder::ValueType::None);
        REQUIRE(functions[13]->returnType() == ponder::ValueType::None);
        REQUIRE(functions[14]->returnType() == ponder::ValueType::None);
        REQUIRE(functions[15]->returnType() == ponder::ValueType::None);
        REQUIRE(functions[16]->returnType() == ponder::ValueType::Integer);
        REQUIRE(functions[17]->returnType() == ponder::ValueType::None);
        REQUIRE(functions[18]->returnType() == ponder::ValueType::None);
        REQUIRE(functions[19]->returnType() == ponder::ValueType::None);
        REQUIRE(functions[20]->returnType() == ponder::ValueType::Integer);
        REQUIRE(functions[21]->returnType() == ponder::ValueType::Integer);
        REQUIRE(functions[22]->returnType() == ponder::ValueType::Integer);
        
        REQUIRE(metaclass.function("nonCopyRef").returnType() == ponder::ValueType::User);
        REQUIRE(metaclass.function("nonCopyPtr").returnType() == ponder::ValueType::User);
    }

    SECTION("functions have a number of parameters")
    {
        REQUIRE(functions[1]->paramCount() ==  1);
        REQUIRE(functions[2]->paramCount() ==  2);
        REQUIRE(functions[3]->paramCount() ==  1);
        REQUIRE(functions[4]->paramCount() ==  0);
        REQUIRE(functions[5]->paramCount() ==  0);
        REQUIRE(functions[6]->paramCount() ==  0);
        REQUIRE(functions[7]->paramCount() ==  1);
        REQUIRE(functions[8]->paramCount() ==  0);
        REQUIRE(functions[9]->paramCount() ==  1);
        REQUIRE(functions[10]->paramCount() == 2);
        REQUIRE(functions[11]->paramCount() == 3);
        REQUIRE(functions[12]->paramCount() == 4);
        REQUIRE(functions[13]->paramCount() == 5);
        
        REQUIRE(functions[14]->paramCount() == 1);
        REQUIRE(functions[15]->paramCount() == 1);
        REQUIRE(functions[16]->paramCount() == 1);
        REQUIRE(functions[17]->paramCount() == 1);
        REQUIRE(functions[18]->paramCount() == 1);
        REQUIRE(functions[19]->paramCount() == 1);
        
        REQUIRE(functions[20]->paramCount() == 2);
        REQUIRE(functions[21]->paramCount() == 2);
        REQUIRE(functions[22]->paramCount() == 2);
        
        REQUIRE(metaclass.function("nonCopyRef").paramCount() == 0);
        REQUIRE(metaclass.function("nonCopyPtr").paramCount() == 0);
    }
    
    SECTION("function parameters have a type")
    {
        REQUIRE(functions[1]->paramType(0) == ponder::ValueType::User);
        REQUIRE(functions[2]->paramType(0) == ponder::ValueType::User);
        REQUIRE(functions[2]->paramType(1) == ponder::ValueType::Integer);
        REQUIRE(functions[3]->paramType(0) == ponder::ValueType::User);
        REQUIRE_THROWS_AS(functions[4]->paramType(0), ponder::OutOfRange);
        REQUIRE_THROWS_AS(functions[5]->paramType(0), ponder::OutOfRange);
        REQUIRE_THROWS_AS(functions[6]->paramType(0), ponder::OutOfRange);
        REQUIRE(functions[7]->paramType(0) ==  ponder::ValueType::User);
        REQUIRE_THROWS_AS(functions[8]->paramType(0), ponder::OutOfRange);
        REQUIRE(functions[9]->paramType(0) ==  ponder::ValueType::Boolean);
        REQUIRE(functions[10]->paramType(0) == ponder::ValueType::Real);
        REQUIRE(functions[10]->paramType(1) == ponder::ValueType::Real);
        REQUIRE(functions[11]->paramType(0) == ponder::ValueType::Integer);
        REQUIRE(functions[11]->paramType(1) == ponder::ValueType::Integer);
        REQUIRE(functions[11]->paramType(2) == ponder::ValueType::Integer);
        REQUIRE(functions[12]->paramType(0) == ponder::ValueType::String);
        REQUIRE(functions[12]->paramType(1) == ponder::ValueType::String);
        REQUIRE(functions[12]->paramType(2) == ponder::ValueType::String);
        REQUIRE(functions[12]->paramType(3) == ponder::ValueType::String);
        REQUIRE(functions[13]->paramType(0) == ponder::ValueType::Enum);
        REQUIRE(functions[13]->paramType(1) == ponder::ValueType::Enum);
        REQUIRE(functions[13]->paramType(2) == ponder::ValueType::Enum);
        REQUIRE(functions[13]->paramType(3) == ponder::ValueType::Enum);
        REQUIRE(functions[13]->paramType(4) == ponder::ValueType::Enum);
        
        REQUIRE(functions[14]->paramType(0) == ponder::ValueType::User);
        REQUIRE(functions[15]->paramType(0) == ponder::ValueType::User);
        REQUIRE(functions[16]->paramType(0) == ponder::ValueType::User);
        REQUIRE(functions[17]->paramType(0) == ponder::ValueType::User);
        REQUIRE(functions[18]->paramType(0) == ponder::ValueType::User);
        REQUIRE(functions[19]->paramType(0) == ponder::ValueType::User);
        
        REQUIRE(functions[20]->paramType(0) == ponder::ValueType::User);
        REQUIRE(functions[20]->paramType(1) == ponder::ValueType::Integer);
        REQUIRE(functions[21]->paramType(0) == ponder::ValueType::User);
        REQUIRE(functions[21]->paramType(1) == ponder::ValueType::Integer);
        REQUIRE(functions[22]->paramType(0) == ponder::ValueType::User);
        REQUIRE(functions[22]->paramType(1) == ponder::ValueType::Integer);
    }
}


//-----------------------------------------------------------------------------
//                  Tests for ponder::runtime::FunctionCaller
//-----------------------------------------------------------------------------

// Leave tests here for now. Possibly move in future as runtime is user module.

TEST_CASE("Registered functions can be called with the runtime")
{
    const ponder::Function* functions[23];
    
    const ponder::Class& metaclass = ponder::classByType<MyClass>();
    for (int i = 1; i <= 22; ++i)
    {
        functions[i] = &metaclass.function("f" + std::to_string(i));
    }

    SECTION("FunctionCaller can be called with Args")
    {
        using ponder::runtime::ObjectCaller;
        using ponder::runtime::FunctionCaller;
        
        MyClass object;
        
        IS_TRUE(FunctionCaller(*functions[1]).call(ponder::Args(&object)) == ponder::Value::nothing);
        IS_TRUE(FunctionCaller(*functions[2]).call(ponder::Args(&object, 10)) == ponder::Value(12));
        IS_TRUE(FunctionCaller(*functions[3]).call(ponder::Args(&object)) == ponder::Value("3"));
        IS_TRUE(ObjectCaller(*functions[4]).call(object, ponder::Args()).to<MyType>() == MyType(4));
        IS_TRUE(ObjectCaller(*functions[5]).call(object, ponder::Args()).to<MyType>() == MyType(5));
        IS_TRUE(ObjectCaller(*functions[6]).call(object, ponder::Args()) == ponder::Value::nothing);
        IS_TRUE(ObjectCaller(*functions[7]).call(object, ponder::Args("hi")) == ponder::Value("hi"));
        IS_TRUE(ObjectCaller(*functions[8]).call(object, ponder::Args()) == ponder::Value::nothing);
        IS_TRUE(ObjectCaller(*functions[9]).call(object, ponder::Args(true)) == ponder::Value::nothing);
        IS_TRUE(ObjectCaller(*functions[10]).call(object, ponder::Args(1., 2.f)) == ponder::Value::nothing);
        IS_TRUE(ObjectCaller(*functions[11]).call(object, ponder::Args(1, 2, 3)) == ponder::Value::nothing);
        IS_TRUE(ObjectCaller(*functions[12]).call(object, ponder::Args("1", "2", "3", "4")) == ponder::Value::nothing);
        IS_TRUE(ObjectCaller(*functions[13]).call(object, ponder::Args(Zero, One, Two, Zero, One)) == ponder::Value::nothing);
        
        IS_TRUE(FunctionCaller(*functions[14]).call(ponder::Args(object)) == ponder::Value::nothing);
        IS_TRUE(FunctionCaller(*functions[15]).call(ponder::Args(object)) == ponder::Value::nothing);
        IS_TRUE(FunctionCaller(*functions[16]).call(ponder::Args(object)) == ponder::Value(16));
        IS_TRUE(FunctionCaller(*functions[17]).call(ponder::Args(object, 20)) == ponder::Value::nothing);
        IS_TRUE(FunctionCaller(*functions[18]).call(ponder::Args(object)) == ponder::Value::nothing);
        IS_TRUE(FunctionCaller(*functions[19]).call(ponder::Args(object)) == ponder::Value::nothing);
        
        IS_TRUE(FunctionCaller(*functions[20]).call(ponder::Args(object, 10)) == ponder::Value(10));
        IS_TRUE(FunctionCaller(*functions[21]).call(ponder::Args(object, 10)) == ponder::Value(30));
        IS_TRUE(FunctionCaller(*functions[22]).call(ponder::Args(object, 10)) == ponder::Value(60));
        
        auto const& mc = ponder::classByType<MyClass>();
        ponder::Value r = FunctionCaller(mc.function("statFunc")).call();
        IS_TRUE(r.to<int>() == 77);
        
        ponder::Value r2 = FunctionCaller(mc.function("statFunc2")).call(ponder::Args(2.5f, 3.0f));
        REQUIRE(r2.to<float>() == 7.5f);
        
        ponder::Value ncr = FunctionCaller(mc.function("nonCopyRef")).call();
        ponder::Value ncp = FunctionCaller(mc.function("nonCopyPtr")).call();
    }

    SECTION("Function call helpers can be used with ponder::Args")
    {
        using namespace ponder::runtime;
        
        MyClass object;
        
        IS_TRUE(callStatic(*functions[1], ponder::Args(&object)) == ponder::Value::nothing);
        IS_TRUE(callStatic(*functions[2], ponder::Args(&object, 10)) == ponder::Value(12));
        IS_TRUE(callStatic(*functions[3], ponder::Args(&object)) == ponder::Value("3"));
        
        IS_TRUE(call(*functions[4], object, ponder::Args()).to<MyType>() == MyType(4));
        IS_TRUE(call(*functions[5], object, ponder::Args()).to<MyType>() == MyType(5));
        IS_TRUE(call(*functions[6], object, ponder::Args()) == ponder::Value::nothing);
        IS_TRUE(call(*functions[7], object, ponder::Args("hi")) == ponder::Value("hi"));
        IS_TRUE(call(*functions[8], object, ponder::Args()) == ponder::Value::nothing);
        IS_TRUE(call(*functions[9], object, ponder::Args(true)) == ponder::Value::nothing);
        IS_TRUE(call(*functions[10], object, ponder::Args(1., 2.f)) == ponder::Value::nothing);
        IS_TRUE(call(*functions[11], object, ponder::Args(1, 2, 3)) == ponder::Value::nothing);
        IS_TRUE(call(*functions[12], object, ponder::Args("1", "2", "3", "4")) == ponder::Value::nothing);
        IS_TRUE(call(*functions[13], object, ponder::Args(Zero, One, Two, Zero, One)) == ponder::Value::nothing);
        
        IS_TRUE(callStatic(*functions[14], ponder::Args(object)) == ponder::Value::nothing);
        IS_TRUE(callStatic(*functions[15], ponder::Args(object)) == ponder::Value::nothing);
        IS_TRUE(callStatic(*functions[16], ponder::Args(object)) == ponder::Value(16));
        IS_TRUE(callStatic(*functions[17], ponder::Args(object, 20)) == ponder::Value::nothing);
        IS_TRUE(callStatic(*functions[18], ponder::Args(object)) == ponder::Value::nothing);
        IS_TRUE(callStatic(*functions[19], ponder::Args(object)) == ponder::Value::nothing);
        
        IS_TRUE(callStatic(*functions[20], ponder::Args(object, 10)) == ponder::Value(10));
        IS_TRUE(callStatic(*functions[21], ponder::Args(object, 10)) == ponder::Value(30));
        IS_TRUE(callStatic(*functions[22], ponder::Args(object, 10)) == ponder::Value(60));
        
        auto const& mc = ponder::classByType<MyClass>();
        ponder::Value r = callStatic(mc.function("statFunc"));
        IS_TRUE(r.to<int>() == 77);
        
        ponder::Value r2 = callStatic(mc.function("statFunc2"), ponder::Args(2.5f, 3.0f));
        REQUIRE(r2.to<float>() == 7.5f);
        
        ponder::Value ncr = callStatic(mc.function("nonCopyRef"));
        ponder::Value ncp = callStatic(mc.function("nonCopyPtr"));
    }
    
    SECTION("Function call helpers can be used direct")
    {
        using namespace ponder::runtime;
        
        MyClass object;
        
        IS_TRUE(callStatic(*functions[1], &object) == ponder::Value::nothing);
        IS_TRUE(callStatic(*functions[2], &object, 10) == ponder::Value(12));
        IS_TRUE(callStatic(*functions[3], &object) == ponder::Value("3"));
        
        IS_TRUE(call(*functions[4], object).to<MyType>() == MyType(4));
        IS_TRUE(call(*functions[5], object).to<MyType>() == MyType(5));
        IS_TRUE(call(*functions[6], object) == ponder::Value::nothing);
        IS_TRUE(call(*functions[7], object, "hi") == ponder::Value("hi"));
        IS_TRUE(call(*functions[8], object) == ponder::Value::nothing);
        IS_TRUE(call(*functions[9], object, true) == ponder::Value::nothing);
        IS_TRUE(call(*functions[10], object, 1., 2.f) == ponder::Value::nothing);
        IS_TRUE(call(*functions[11], object, 1, 2, 3) == ponder::Value::nothing);
        IS_TRUE(call(*functions[12], object, "1", "2", "3", "4") == ponder::Value::nothing);
        IS_TRUE(call(*functions[13], object, Zero, One, Two, Zero, One) == ponder::Value::nothing);
        
        IS_TRUE(callStatic(*functions[14], object) == ponder::Value::nothing);
        IS_TRUE(callStatic(*functions[15], object) == ponder::Value::nothing);
        IS_TRUE(callStatic(*functions[16], object) == ponder::Value(16));
        IS_TRUE(callStatic(*functions[17], object, 20) == ponder::Value::nothing);
        IS_TRUE(callStatic(*functions[18], object) == ponder::Value::nothing);
        IS_TRUE(callStatic(*functions[19], object) == ponder::Value::nothing);
        
        IS_TRUE(callStatic(*functions[20], object, 10) == ponder::Value(10));
        IS_TRUE(callStatic(*functions[21], object, 10) == ponder::Value(30));
        IS_TRUE(callStatic(*functions[22], object, 10) == ponder::Value(60));
        
        auto const& mc = ponder::classByType<MyClass>();
        ponder::Value r = callStatic(mc.function("statFunc"));
        IS_TRUE(r.to<int>() == 77);
        
        ponder::Value r2 = callStatic(mc.function("statFunc2"), 2.5f, 3.0f);
        REQUIRE(r2.to<float>() == 7.5f);
        
        ponder::Value ncr = callStatic(mc.function("nonCopyRef"));
        ponder::Value ncp = callStatic(mc.function("nonCopyPtr"));
    }

    SECTION("calling null functions is an error")
    {
        using ponder::runtime::ObjectCaller;
        using ponder::runtime::FunctionCaller;

        REQUIRE_THROWS_AS(FunctionCaller(*functions[1]).call(ponder::Args(ponder::UserObject())),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(FunctionCaller(*functions[3]).call(ponder::Args(ponder::UserObject())),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(ObjectCaller(*functions[4]).call(ponder::UserObject(), ponder::Args()),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(ObjectCaller(*functions[5]).call(ponder::UserObject(), ponder::Args()),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(ObjectCaller(*functions[6]).call(ponder::UserObject(), ponder::Args()),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(ObjectCaller(*functions[8]).call(ponder::UserObject(), ponder::Args()),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(FunctionCaller(*functions[15]).call(ponder::Args(ponder::UserObject())),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(FunctionCaller(*functions[16]).call(ponder::Args(ponder::UserObject())),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(FunctionCaller(*functions[18]).call(ponder::Args(ponder::UserObject())),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(FunctionCaller(*functions[19]).call(ponder::Args(ponder::UserObject())),
                          ponder::NullObject);
    }
    
    SECTION("calling a function without enough parameters is an error")
    {
        using ponder::runtime::call;
        
        MyClass object;
        
        REQUIRE_THROWS_AS(call(*functions[2], object, ponder::Args()),
                          ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(call(*functions[7], object, ponder::Args()),
                          ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(call(*functions[9], object, ponder::Args()),
                        ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(call(*functions[10], object, ponder::Args(1.)),
                          ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(call(*functions[11], object, ponder::Args(1, 2)),
                          ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(call(*functions[12], object, ponder::Args("1", "2", "3")),
                          ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(call(*functions[13], object, ponder::Args(Zero, One, Two, Zero)),
                          ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(call(*functions[20], object, ponder::Args()),
                          ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(call(*functions[21], object, ponder::Args()),
                          ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(call(*functions[22], object, ponder::Args()),
                          ponder::NotEnoughArguments);
    }

   SECTION("functions must be called with the correct parameters")
   {
       using ponder::runtime::call;
       using ponder::runtime::callStatic;
       
       MyClass object;
       MyType arg(0);

       REQUIRE_THROWS_AS(callStatic(*functions[2], ponder::Args(&object, arg, arg)),
                         ponder::BadArgument);
       REQUIRE_THROWS_AS(call(*functions[10], object, ponder::Args(arg, arg)),
                         ponder::BadArgument);
       REQUIRE_THROWS_AS(call(*functions[11], object, ponder::Args(arg, arg, arg)),
                         ponder::BadArgument);
       REQUIRE_THROWS_AS(call(*functions[12], object, ponder::Args(arg, arg, arg, arg)),
                         ponder::BadArgument);
       REQUIRE_THROWS_AS(call(*functions[13], object, ponder::Args(arg, arg, arg, arg, arg)),
                         ponder::BadArgument);
       REQUIRE_THROWS_AS(callStatic(*functions[20], ponder::Args(object, arg)),
                         ponder::BadArgument);
       REQUIRE_THROWS_AS(callStatic(*functions[21], ponder::Args(object, arg)),
                         ponder::BadArgument);
       REQUIRE_THROWS_AS(callStatic(*functions[22], ponder::Args(object, arg)),
                         ponder::BadArgument);
   }
}

TEST_CASE("Functions can modify objects")
{
    // ModifyA() is called on an object of class A with the intent to modify that object:
    ClassA objectA;
    
    const ponder::Class& metaClassB = ponder::classByType<FunctionTest::ClassB>();
    ponder::runtime::ObjectFactory bfact(metaClassB);
    ponder::UserObject wrapperB =  bfact.construct();
    ponder::runtime::ObjectCaller functionB(metaClassB.function("ModifyA"));
    
    REQUIRE(objectA.TestMember == 0);
    functionB.call(wrapperB, &objectA);
    REQUIRE(objectA.TestMember == 5);
    
    bfact.destroy(wrapperB);
}

