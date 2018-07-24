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
        static int instCount, copyCount;
        
        MyType(int x_) : x(x_) { ++instCount; }
        MyType(const MyType& o) : x(o.x) { ++instCount; ++copyCount; }
        ~MyType() { --instCount; }
        
        int x;
    };

    int MyType::instCount = 0, MyType::copyCount = 0;

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
        void member3() {}
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
        const MyType& member1() {return p4;}
        
        MyType p5;
        const MyType& member2() const {return p5;}
        
        // member3 is inherited
        ponder::Value member4(ponder::Value v) {return v;}
        
        void memberParams1() {}
        void memberParams2(bool) {}
        void memberParams3(float, double) {}
        void memberParams4(short, int, long) {}
        void memberParams5(const ponder::String&, ponder::String,
                           const ponder::String&, ponder::String) {}
        void memberParams6(MyEnum, MyEnum, MyEnum, MyEnum, MyEnum) {}
        
        struct Inner
        {
            void nested1() {}
            void nested2() const {}
            int  nested3() {return 16;}
            void nested4(int) const {}
            void nested5() {}
            void nested6() {}
        };
        
        Inner inner;
        const Inner& getInner() const {return inner;}
        
        Inner* innerPtr;
        const Inner* getInnerPtr() const {return innerPtr;}
        
        std::shared_ptr<Inner> innerSmartPtr;
        std::shared_ptr<Inner> getInnerSmartPtr() {return innerSmartPtr;}
        
        int funcWrapper1(int x) {return x;}
        int funcWrapper2(int x, int y) {return x + y;}
        int funcWrapper3(int x, int y, int z) {return x + y + z;}
        
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
    
    void nonMember1(MyClass& object)  // TODO - allow non-const refs
    {
        object.p1 = true;
    }
    
    int nonMember2(MyClass& object, int x)
    {
        return object.p2 + x;
    }
    
    const ponder::String& nonMember3(const MyClass* object)
    {
        return object->p3;
    }
    
    
    class DataHolder
    {
    public:
        DataHolder() : TestMember(0) {}
        int TestMember;
    };
    
    class DataModifier
    {
    public:
        void modifyData(DataHolder* obj) { obj->TestMember = 5; }
    };
    
    
    class Policy
    {
    public:
        Policy() : internalInst(99) {}
        
        const MyType& returnCopy() const { return internalInst; }
        
        const MyType& internalRefConst() const { return internalInst; }
        MyType& internalRef() { return internalInst; }
        
        MyType internalInst;
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
            .function("nonMember1", &nonMember1) // object by reference
            .function("nonMember2", &nonMember2) // object by value + parameter
            .function("nonMember3", &nonMember3) // object by pointer

            // ***** member functions *****
            .function("member1", &MyClass::member1) // non-const
            .function("member2", &MyClass::member2) // const
            .function("member3", &MyClass::member3) // inherited
            .function("member4", &MyClass::member4) // ponder::Value as return and parameter types

            // ***** parameters count ******
            .function("memberParams1", &MyClass::memberParams1)     // 0 parameter
            .function("memberParams2", &MyClass::memberParams2)     // 1 parameter
            .function("memberParams3", &MyClass::memberParams3)     // 2 parameters
            .function("memberParams4", &MyClass::memberParams4)     // 3 parameters
            .function("memberParams5", &MyClass::memberParams5)     // 4 parameters
            .function("memberParams6", &MyClass::memberParams6)     // 5 parameters

            // ***** nested functions *****
            .function("lambdaFunc1", [](MyClass& self){ self.inner.nested1(); })
            // getter returning an object
            .function("lambdaFunc2", [](MyClass& self){ self.getInner().nested2(); })
            // raw pointer
            .function("lambdaFunc3", [](MyClass& self){ return self.innerPtr->nested3(); })
            // getter returning a raw pointer
            .function("lambdaFunc4", [](MyClass& self){ self.getInnerPtr()->nested4(99); })
            // smart pointer
            .function("lambdaFunc5", [](MyClass& self){ self.innerSmartPtr.get()->nested5(); })
            // getter returning a smart pointer
            .function("lambdaFunc6", [](MyClass& self){self.getInnerSmartPtr().get()->nested6();})

            // ***** std::function *****
            .function("funcWrapper1",
                      std::function<int (MyClass, int)>(
                                                std::bind(&MyClass::funcWrapper1, _1, _2)))
            .function("funcWrapper2",
                      std::function<int (MyClass, int)>(
                                                std::bind(&MyClass::funcWrapper2, _1, _2, 20)))
            .function("funcWrapper3",
                      std::function<int (MyClass, int)>(
                          std::bind(std::bind(&MyClass::funcWrapper3, _1, _2, _3, 30), _1, _2, 20)))

            // ***** non-class function *****
            .function("nonClassFunc1", &MyClass::staticFunc)
            .function("nonClassFunc2", &MyClass::staticFunc2)

            .function("nonCopyRef", &MyClass::staticFuncRetRef, ponder::policy::ReturnInternalRef())
            .function("nonCopyPtr", &MyClass::staticFuncRetPtr, ponder::policy::ReturnInternalRef())
            ;

        ponder::Class::declare<DataHolder>()
            .constructor()
            .property("TestMember", &DataHolder::TestMember);

        ponder::Class::declare<DataModifier>()
            .constructor()
            .function("modifyData", &DataModifier::modifyData);

        ponder::Class::declare<Policy>()
            .constructor()
            .function("returnCopy", &Policy::returnCopy, ponder::policy::ReturnCopy())
            .function("constInternalRef", &Policy::internalRefConst, ponder::policy::ReturnInternalRef())
            .function("internalRef", &Policy::internalRef, ponder::policy::ReturnInternalRef())
            ;
    }
}

PONDER_AUTO_TYPE(FunctionTest::MyEnum, &FunctionTest::declare)
PONDER_AUTO_TYPE(FunctionTest::MyType, &FunctionTest::declare)
PONDER_AUTO_TYPE(FunctionTest::MyClass, &FunctionTest::declare)
PONDER_AUTO_TYPE(FunctionTest::NonCopyable, &FunctionTest::declare)
PONDER_AUTO_TYPE(FunctionTest::MyBase, &FunctionTest::declare)
PONDER_AUTO_TYPE(FunctionTest::DataHolder, &FunctionTest::declare)
PONDER_AUTO_TYPE(FunctionTest::DataModifier, &FunctionTest::declare)
PONDER_AUTO_TYPE(FunctionTest::Policy, &FunctionTest::declare)

using namespace FunctionTest;

struct FunctionTestFixture
{
    FunctionTestFixture()
    :   metaclass(ponder::classByType<MyClass>())
    ,   fn_nonMember1(metaclass.function("nonMember1"))
    ,   fn_nonMember2(metaclass.function("nonMember2"))
    ,   fn_nonMember3(metaclass.function("nonMember3"))
    ,   fn_member1(metaclass.function("member1"))
    ,   fn_member2(metaclass.function("member2"))
    ,   fn_member3(metaclass.function("member3"))
    ,   fn_member4(metaclass.function("member4"))
    ,   fn_memberParams1(metaclass.function("memberParams1"))
    ,   fn_memberParams2(metaclass.function("memberParams2"))
    ,   fn_memberParams3(metaclass.function("memberParams3"))
    ,   fn_memberParams4(metaclass.function("memberParams4"))
    ,   fn_memberParams5(metaclass.function("memberParams5"))
    ,   fn_memberParams6(metaclass.function("memberParams6"))
    ,   fn_lambdaFunc1(metaclass.function("lambdaFunc1"))
    ,   fn_lambdaFunc2(metaclass.function("lambdaFunc2"))
    ,   fn_lambdaFunc3(metaclass.function("lambdaFunc3"))
    ,   fn_lambdaFunc4(metaclass.function("lambdaFunc4"))
    ,   fn_lambdaFunc5(metaclass.function("lambdaFunc5"))
    ,   fn_lambdaFunc6(metaclass.function("lambdaFunc6"))
    ,   fn_funcWrapper1(metaclass.function("funcWrapper1"))
    ,   fn_funcWrapper2(metaclass.function("funcWrapper2"))
    ,   fn_funcWrapper3(metaclass.function("funcWrapper3"))
    ,   fn_nonClassFunc1(metaclass.function("nonClassFunc1"))
    ,   fn_nonClassFunc2(metaclass.function("nonClassFunc2"))
    ,   fn_nonCopyRef(metaclass.function("nonCopyRef"))
    ,   fn_nonCopyPtr(metaclass.function("nonCopyPtr"))
    {}

    const ponder::Class &metaclass;
    const ponder::Function &fn_nonMember1;
    const ponder::Function &fn_nonMember2;
    const ponder::Function &fn_nonMember3;
    const ponder::Function &fn_member1;
    const ponder::Function &fn_member2;
    const ponder::Function &fn_member3;
    const ponder::Function &fn_member4;
    const ponder::Function &fn_memberParams1;
    const ponder::Function &fn_memberParams2;
    const ponder::Function &fn_memberParams3;
    const ponder::Function &fn_memberParams4;
    const ponder::Function &fn_memberParams5;
    const ponder::Function &fn_memberParams6;
    const ponder::Function &fn_lambdaFunc1;
    const ponder::Function &fn_lambdaFunc2;
    const ponder::Function &fn_lambdaFunc3;
    const ponder::Function &fn_lambdaFunc4;
    const ponder::Function &fn_lambdaFunc5;
    const ponder::Function &fn_lambdaFunc6;
    const ponder::Function &fn_funcWrapper1;
    const ponder::Function &fn_funcWrapper2;
    const ponder::Function &fn_funcWrapper3;
    const ponder::Function &fn_nonClassFunc1;
    const ponder::Function &fn_nonClassFunc2;
    const ponder::Function &fn_nonCopyRef;
    const ponder::Function &fn_nonCopyPtr;

    const ponder::Function *functions[23];
};

//-----------------------------------------------------------------------------
//                         Tests for ponder::Function
//-----------------------------------------------------------------------------

TEST_CASE_METHOD(FunctionTestFixture, "Registered function properties can be introspected")
{
    SECTION("functions are classified")
    {
        IS_TRUE(fn_nonMember1.kind() == ponder::FunctionKind::Function);
        IS_TRUE(fn_nonMember2.kind() == ponder::FunctionKind::Function);
        IS_TRUE(fn_nonMember3.kind() == ponder::FunctionKind::Function);

        IS_TRUE(fn_member1.kind() == ponder::FunctionKind::MemberFunction);
        IS_TRUE(fn_member2.kind() == ponder::FunctionKind::MemberFunction);
        IS_TRUE(fn_member3.kind() == ponder::FunctionKind::MemberFunction);
        IS_TRUE(fn_member4.kind() == ponder::FunctionKind::MemberFunction);

        IS_TRUE(fn_memberParams1.kind() == ponder::FunctionKind::MemberFunction);
        IS_TRUE(fn_memberParams2.kind() == ponder::FunctionKind::MemberFunction);
        IS_TRUE(fn_memberParams3.kind() == ponder::FunctionKind::MemberFunction);
        IS_TRUE(fn_memberParams4.kind() == ponder::FunctionKind::MemberFunction);
        IS_TRUE(fn_memberParams5.kind() == ponder::FunctionKind::MemberFunction);
        IS_TRUE(fn_memberParams6.kind() == ponder::FunctionKind::MemberFunction);

        IS_TRUE(fn_lambdaFunc1.kind() == ponder::FunctionKind::Lambda);
        IS_TRUE(fn_lambdaFunc2.kind() == ponder::FunctionKind::Lambda);
        IS_TRUE(fn_lambdaFunc3.kind() == ponder::FunctionKind::Lambda);
        IS_TRUE(fn_lambdaFunc4.kind() == ponder::FunctionKind::Lambda);
        IS_TRUE(fn_lambdaFunc5.kind() == ponder::FunctionKind::Lambda);
        IS_TRUE(fn_lambdaFunc6.kind() == ponder::FunctionKind::Lambda);

        IS_TRUE(fn_funcWrapper1.kind() == ponder::FunctionKind::FunctionWrapper);
        IS_TRUE(fn_funcWrapper2.kind() == ponder::FunctionKind::FunctionWrapper);
        IS_TRUE(fn_funcWrapper3.kind() == ponder::FunctionKind::FunctionWrapper);

        IS_TRUE(fn_nonCopyRef.kind() == ponder::FunctionKind::Function);
        IS_TRUE(fn_nonCopyPtr.kind() == ponder::FunctionKind::Function);
    }

    SECTION("functions have a return type")
    {
        REQUIRE(fn_nonMember1.returnType() ==  ponder::ValueKind::None);
        REQUIRE(fn_nonMember2.returnType() ==  ponder::ValueKind::Integer);
        REQUIRE(fn_nonMember3.returnType() ==  ponder::ValueKind::String);
        REQUIRE(fn_member1.returnType() ==  ponder::ValueKind::User);
        REQUIRE(fn_member2.returnType() ==  ponder::ValueKind::User);
        REQUIRE(fn_member3.returnType() ==  ponder::ValueKind::None);
        REQUIRE(fn_member4.returnType() ==  ponder::ValueKind::User);
        REQUIRE(fn_memberParams1.returnType() ==  ponder::ValueKind::None);
        REQUIRE(fn_memberParams2.returnType() ==  ponder::ValueKind::None);
        REQUIRE(fn_memberParams3.returnType() == ponder::ValueKind::None);
        REQUIRE(fn_memberParams4.returnType() == ponder::ValueKind::None);
        REQUIRE(fn_memberParams5.returnType() == ponder::ValueKind::None);
        REQUIRE(fn_memberParams6.returnType() == ponder::ValueKind::None);
        REQUIRE(fn_lambdaFunc1.returnType() == ponder::ValueKind::None);
        REQUIRE(fn_lambdaFunc2.returnType() == ponder::ValueKind::None);
        REQUIRE(fn_lambdaFunc3.returnType() == ponder::ValueKind::Integer);
        REQUIRE(fn_lambdaFunc4.returnType() == ponder::ValueKind::None);
        REQUIRE(fn_lambdaFunc5.returnType() == ponder::ValueKind::None);
        REQUIRE(fn_lambdaFunc6.returnType() == ponder::ValueKind::None);
        REQUIRE(fn_funcWrapper1.returnType() == ponder::ValueKind::Integer);
        REQUIRE(fn_funcWrapper2.returnType() == ponder::ValueKind::Integer);
        REQUIRE(fn_funcWrapper3.returnType() == ponder::ValueKind::Integer);

        REQUIRE(metaclass.function("nonCopyRef").returnType() == ponder::ValueKind::User);
        REQUIRE(metaclass.function("nonCopyPtr").returnType() == ponder::ValueKind::User);
    }

    SECTION("functions have a number of parameters")
    {
        REQUIRE(fn_nonMember1.paramCount() ==  1);
        REQUIRE(fn_nonMember2.paramCount() ==  2);
        REQUIRE(fn_nonMember3.paramCount() ==  1);
        REQUIRE(fn_member1.paramCount() ==  0);
        REQUIRE(fn_member2.paramCount() ==  0);
        REQUIRE(fn_member3.paramCount() ==  0);
        REQUIRE(fn_member4.paramCount() ==  1);
        REQUIRE(fn_memberParams1.paramCount() ==  0);
        REQUIRE(fn_memberParams2.paramCount() ==  1);
        REQUIRE(fn_memberParams3.paramCount() == 2);
        REQUIRE(fn_memberParams4.paramCount() == 3);
        REQUIRE(fn_memberParams5.paramCount() == 4);
        REQUIRE(fn_memberParams6.paramCount() == 5);

        REQUIRE(fn_lambdaFunc1.paramCount() == 1);
        REQUIRE(fn_lambdaFunc2.paramCount() == 1);
        REQUIRE(fn_lambdaFunc3.paramCount() == 1);
        REQUIRE(fn_lambdaFunc4.paramCount() == 1);
        REQUIRE(fn_lambdaFunc5.paramCount() == 1);
        REQUIRE(fn_lambdaFunc6.paramCount() == 1);

        REQUIRE(fn_funcWrapper1.paramCount() == 2);
        REQUIRE(fn_funcWrapper2.paramCount() == 2);
        REQUIRE(fn_funcWrapper3.paramCount() == 2);

        REQUIRE(metaclass.function("nonCopyRef").paramCount() == 0);
        REQUIRE(metaclass.function("nonCopyPtr").paramCount() == 0);
    }

    SECTION("function parameters have a type")
    {
        REQUIRE(fn_nonMember1.paramType(0) == ponder::ValueKind::User);
        REQUIRE(fn_nonMember2.paramType(0) == ponder::ValueKind::User);
        REQUIRE(fn_nonMember2.paramType(1) == ponder::ValueKind::Integer);
        REQUIRE(fn_nonMember3.paramType(0) == ponder::ValueKind::User);
        REQUIRE_THROWS_AS(fn_member1.paramType(0), ponder::OutOfRange);
        REQUIRE_THROWS_AS(fn_member2.paramType(0), ponder::OutOfRange);
        REQUIRE_THROWS_AS(fn_member3.paramType(0), ponder::OutOfRange);
        REQUIRE(fn_member4.paramType(0) ==  ponder::ValueKind::User);
        REQUIRE_THROWS_AS(fn_memberParams1.paramType(0), ponder::OutOfRange);
        REQUIRE(fn_memberParams2.paramType(0) ==  ponder::ValueKind::Boolean);
        REQUIRE(fn_memberParams3.paramType(0) == ponder::ValueKind::Real);
        REQUIRE(fn_memberParams3.paramType(1) == ponder::ValueKind::Real);
        REQUIRE(fn_memberParams4.paramType(0) == ponder::ValueKind::Integer);
        REQUIRE(fn_memberParams4.paramType(1) == ponder::ValueKind::Integer);
        REQUIRE(fn_memberParams4.paramType(2) == ponder::ValueKind::Integer);
        REQUIRE(fn_memberParams5.paramType(0) == ponder::ValueKind::String);
        REQUIRE(fn_memberParams5.paramType(1) == ponder::ValueKind::String);
        REQUIRE(fn_memberParams5.paramType(2) == ponder::ValueKind::String);
        REQUIRE(fn_memberParams5.paramType(3) == ponder::ValueKind::String);
        REQUIRE(fn_memberParams6.paramType(0) == ponder::ValueKind::Enum);
        REQUIRE(fn_memberParams6.paramType(1) == ponder::ValueKind::Enum);
        REQUIRE(fn_memberParams6.paramType(2) == ponder::ValueKind::Enum);
        REQUIRE(fn_memberParams6.paramType(3) == ponder::ValueKind::Enum);
        REQUIRE(fn_memberParams6.paramType(4) == ponder::ValueKind::Enum);

        REQUIRE(fn_lambdaFunc1.paramType(0) == ponder::ValueKind::User);
        REQUIRE(fn_lambdaFunc2.paramType(0) == ponder::ValueKind::User);
        REQUIRE(fn_lambdaFunc3.paramType(0) == ponder::ValueKind::User);
        REQUIRE(fn_lambdaFunc4.paramType(0) == ponder::ValueKind::User);
        REQUIRE(fn_lambdaFunc5.paramType(0) == ponder::ValueKind::User);
        REQUIRE(fn_lambdaFunc6.paramType(0) == ponder::ValueKind::User);

        REQUIRE(fn_funcWrapper1.paramType(0) == ponder::ValueKind::User);
        REQUIRE(fn_funcWrapper1.paramType(1) == ponder::ValueKind::Integer);
        REQUIRE(fn_funcWrapper2.paramType(0) == ponder::ValueKind::User);
        REQUIRE(fn_funcWrapper2.paramType(1) == ponder::ValueKind::Integer);
        REQUIRE(fn_funcWrapper3.paramType(0) == ponder::ValueKind::User);
        REQUIRE(fn_funcWrapper3.paramType(1) == ponder::ValueKind::Integer);
    }
}

//-----------------------------------------------------------------------------
//                         Functions policies
//-----------------------------------------------------------------------------

TEST_CASE_METHOD(FunctionTestFixture, "Functions can have policies")
{
    auto const& clsPolicy = ponder::classByType<Policy>();

    const ponder::Function &fn_policyCopy(clsPolicy.function("returnCopy")),
                           &fn_policyInternalRefConst(clsPolicy.function("constInternalRef")),
                           &fn_policyInternalRef(clsPolicy.function("internalRef"));

    SECTION("All functions have default return policy")
    {
        REQUIRE(fn_nonMember1.returnPolicy() == ponder::policy::ReturnKind::NoReturn); // void
        REQUIRE(fn_nonMember2.returnPolicy() == ponder::policy::ReturnKind::Copy); // int
        REQUIRE(fn_nonMember3.returnPolicy() == ponder::policy::ReturnKind::Copy); // const&
        REQUIRE(fn_member1.returnPolicy() == ponder::policy::ReturnKind::Copy); // const&
    }

    SECTION("Policy kinds")
    {
        REQUIRE(fn_policyCopy.returnPolicy() == ponder::policy::ReturnKind::Copy);
        REQUIRE(fn_policyInternalRefConst.returnPolicy() == ponder::policy::ReturnKind::InternalRef);
        REQUIRE(fn_policyInternalRef.returnPolicy() == ponder::policy::ReturnKind::InternalRef);
    }

    SECTION("Return copy")
    {
        using namespace ponder;
        MyType::instCount = MyType::copyCount = 0;
        {
            Policy pobj;
            REQUIRE(pobj.internalRefConst().x == 99);
            REQUIRE(pobj.internalInst.x == 99);
            REQUIRE(MyType::instCount == 1);
            REQUIRE(MyType::copyCount == 0);

            UserObject puo = UserObject::makeRef(pobj);
            REQUIRE(MyType::instCount == 1);
            REQUIRE(MyType::copyCount == 0);

            UserObject retuo = runtime::call(fn_policyCopy, puo).to<UserObject>();
            REQUIRE(MyType::instCount == 2);
            REQUIRE(MyType::copyCount == 1);

            REQUIRE(pobj.internalInst.x == 99);
            REQUIRE(retuo.get<MyType>().x == 99); // check copy

            pobj.internalInst.x = 123;
            REQUIRE(pobj.internalInst.x == 123);
            REQUIRE(retuo.get<MyType>().x == 99); // check copy not changed
        }
        REQUIRE(MyType::instCount == 0);
        REQUIRE(MyType::copyCount == 1);
    }

    SECTION("Return internal ref const")
    {
        using namespace ponder;
        MyType::instCount = MyType::copyCount = 0;
        {
            Policy pobj;
            REQUIRE(pobj.internalRefConst().x == 99);
            REQUIRE(pobj.internalInst.x == 99);
            REQUIRE(MyType::instCount == 1);
            REQUIRE(MyType::copyCount == 0);

            UserObject puo = UserObject::makeRef(pobj);
            REQUIRE(MyType::instCount == 1);
            REQUIRE(MyType::copyCount == 0);

            Value ret = runtime::call(fn_policyInternalRefConst, puo);
            REQUIRE(ret.kind() == ValueKind::User);
            REQUIRE(MyType::instCount == 1);
            REQUIRE(MyType::copyCount == 0);

            const MyType& mt = ret.cref<UserObject>().cref<MyType>();
            REQUIRE(MyType::instCount == 1);
            REQUIRE(MyType::copyCount == 0);

            REQUIRE(pobj.internalInst.x == 99);
            REQUIRE(mt.x == 99); // check ref

            pobj.internalInst.x = 123;
            REQUIRE(pobj.internalInst.x == 123);
            REQUIRE(mt.x == 123); // check ref has changed
        }
        REQUIRE(MyType::instCount == 0);
        REQUIRE(MyType::copyCount == 0);
    }
}

//-----------------------------------------------------------------------------
//                  Tests for ponder::runtime::FunctionCaller
//-----------------------------------------------------------------------------

// Leave tests here for now. Possibly move in future as runtime is uses module.

TEST_CASE_METHOD(FunctionTestFixture, "Registered functions can be called with the runtime")
{
    SECTION("FunctionCaller can be called with Args")
    {
        using ponder::Value;
        using ponder::runtime::ObjectCaller;
        using ponder::runtime::FunctionCaller;

        MyClass object;

        IS_TRUE(FunctionCaller(fn_nonMember1).call(ponder::Args(&object)) == Value::nothing);
        IS_TRUE(FunctionCaller(fn_nonMember2).call(ponder::Args(&object, 10)) == Value(12));
        IS_TRUE(FunctionCaller(fn_nonMember3).call(ponder::Args(&object)) == Value("3"));

        IS_TRUE(ObjectCaller(fn_member1).call(&object, ponder::Args()).to<MyType>() == MyType(4));
        IS_TRUE(ObjectCaller(fn_member2).call(&object, ponder::Args()).to<MyType>() == MyType(5));
        IS_TRUE(ObjectCaller(fn_member3).call(&object, ponder::Args()) == Value::nothing);
        IS_TRUE(ObjectCaller(fn_member4).call(&object, ponder::Args("hi")) == Value("hi"));

        IS_TRUE(ObjectCaller(fn_memberParams1)
                    .call(&object, ponder::Args()) == Value::nothing);
        IS_TRUE(ObjectCaller(fn_memberParams2)
                    .call(&object, ponder::Args(true)) == Value::nothing);
        IS_TRUE(ObjectCaller(fn_memberParams3)
                    .call(&object, ponder::Args(1., 2.f)) == Value::nothing);
        IS_TRUE(ObjectCaller(fn_memberParams4)
                    .call(&object, ponder::Args(1, 2, 3)) == Value::nothing);
        IS_TRUE(ObjectCaller(fn_memberParams5)
                    .call(&object, ponder::Args("1", "2", "3", "4")) == Value::nothing);
        IS_TRUE(ObjectCaller(fn_memberParams6)
                    .call(&object, ponder::Args(Zero, One, Two, Zero, One))
                        == Value::nothing);

        IS_TRUE(FunctionCaller(fn_lambdaFunc1).call(ponder::Args(&object)) == Value::nothing);
        IS_TRUE(FunctionCaller(fn_lambdaFunc2).call(ponder::Args(&object)) == Value::nothing);
        IS_TRUE(FunctionCaller(fn_lambdaFunc3).call(ponder::Args(&object)) == Value(16));
        IS_TRUE(FunctionCaller(fn_lambdaFunc4).call(ponder::Args(&object, 20)) == Value::nothing);
        IS_TRUE(FunctionCaller(fn_lambdaFunc5).call(ponder::Args(&object)) == Value::nothing);
        IS_TRUE(FunctionCaller(fn_lambdaFunc6).call(ponder::Args(&object)) == Value::nothing);

        IS_TRUE(FunctionCaller(fn_funcWrapper1).call(ponder::Args(&object, 10)) == Value(10));
        IS_TRUE(FunctionCaller(fn_funcWrapper2).call(ponder::Args(&object, 10)) == Value(30));
        IS_TRUE(FunctionCaller(fn_funcWrapper3).call(ponder::Args(&object, 10)) == Value(60));

        auto const& mc = ponder::classByType<MyClass>();
        Value r = FunctionCaller(fn_nonClassFunc1).call();
        IS_TRUE(r.to<int>() == 77);

        Value r2 = FunctionCaller(fn_nonClassFunc2).call(ponder::Args(2.5f, 3.0f));
        REQUIRE(r2.to<float>() == 7.5f);

        Value ncr = FunctionCaller(fn_nonCopyRef).call();
        Value ncp = FunctionCaller(fn_nonCopyPtr).call();
    }

    SECTION("Function call helpers can be used with ponder::Args")
    {
        using ponder::Value;
        using namespace ponder::runtime;

        MyClass object;

        IS_TRUE(callStatic(fn_nonMember1, ponder::Args(&object)) == Value::nothing);
        IS_TRUE(callStatic(fn_nonMember2, ponder::Args(&object, 10)) == Value(12));
        IS_TRUE(callStatic(fn_nonMember3, ponder::Args(&object)) == Value("3"));

        IS_TRUE(call(fn_member1, &object, ponder::Args()).to<MyType>() == MyType(4));
        IS_TRUE(call(fn_member2, &object, ponder::Args()).to<MyType>() == MyType(5));
        IS_TRUE(call(fn_member3, &object, ponder::Args()) == Value::nothing);
        IS_TRUE(call(fn_member4, &object, ponder::Args("hi")) == Value("hi"));
        IS_TRUE(call(fn_memberParams1, &object, ponder::Args()) == Value::nothing);
        IS_TRUE(call(fn_memberParams2, &object, ponder::Args(true)) == Value::nothing);
        IS_TRUE(call(fn_memberParams3, &object, ponder::Args(1., 2.f)) == Value::nothing);
        IS_TRUE(call(fn_memberParams4, &object, ponder::Args(1, 2, 3)) == Value::nothing);
        IS_TRUE(call(fn_memberParams5, &object, ponder::Args("1", "2", "3", "4"))
                    == Value::nothing);
        IS_TRUE(call(fn_memberParams6, &object, ponder::Args(Zero, One, Two, Zero, One))
                    == Value::nothing);

        IS_TRUE(callStatic(fn_lambdaFunc1, ponder::Args(&object)) == Value::nothing);
        IS_TRUE(callStatic(fn_lambdaFunc2, ponder::Args(&object)) == Value::nothing);
        IS_TRUE(callStatic(fn_lambdaFunc3, ponder::Args(&object)) == Value(16));
        IS_TRUE(callStatic(fn_lambdaFunc4, ponder::Args(&object, 20)) == Value::nothing);
        IS_TRUE(callStatic(fn_lambdaFunc5, ponder::Args(&object)) == Value::nothing);
        IS_TRUE(callStatic(fn_lambdaFunc6, ponder::Args(&object)) == Value::nothing);

        IS_TRUE(callStatic(fn_funcWrapper1, ponder::Args(&object, 10)) == Value(10));
        IS_TRUE(callStatic(fn_funcWrapper2, ponder::Args(&object, 10)) == Value(30));
        IS_TRUE(callStatic(fn_funcWrapper3, ponder::Args(&object, 10)) == Value(60));

        auto const& mc = ponder::classByType<MyClass>();
        Value r = FunctionCaller(fn_nonClassFunc1).call();
        IS_TRUE(r.to<int>() == 77);

        Value r2 = callStatic(fn_nonClassFunc2, ponder::Args(2.5f, 3.0f));
        REQUIRE(r2.to<float>() == 7.5f);

        Value ncr = callStatic(fn_nonCopyRef);
        Value ncp = callStatic(fn_nonCopyPtr);
    }

    SECTION("Function call helpers can be used direct")
    {
        using ponder::Value;
        using namespace ponder::runtime;

        MyClass object;

        IS_TRUE(callStatic(fn_nonMember1, &object) == Value::nothing);
        IS_TRUE(callStatic(fn_nonMember2, &object, 10) == Value(12));
        IS_TRUE(callStatic(fn_nonMember3, &object) == Value("3"));

        IS_TRUE(call(fn_member1, &object).to<MyType>() == MyType(4));
        IS_TRUE(call(fn_member2, &object).to<MyType>() == MyType(5));
        IS_TRUE(call(fn_member3, &object) == Value::nothing);
        IS_TRUE(call(fn_member4, &object, "hi") == Value("hi"));
        IS_TRUE(call(fn_memberParams1, &object) == Value::nothing);
        IS_TRUE(call(fn_memberParams2, &object, true) == Value::nothing);
        IS_TRUE(call(fn_memberParams3, &object, 1., 2.f) == Value::nothing);
        IS_TRUE(call(fn_memberParams4, &object, 1, 2, 3) == Value::nothing);
        IS_TRUE(call(fn_memberParams5, &object, "1", "2", "3", "4") == Value::nothing);
        IS_TRUE(call(fn_memberParams6, &object, Zero, One, Two, Zero, One) == Value::nothing);

        IS_TRUE(callStatic(fn_lambdaFunc1, &object) == Value::nothing);
        IS_TRUE(callStatic(fn_lambdaFunc2, &object) == Value::nothing);
        IS_TRUE(callStatic(fn_lambdaFunc3, &object) == Value(16));
        IS_TRUE(callStatic(fn_lambdaFunc4, &object, 20) == Value::nothing);
        IS_TRUE(callStatic(fn_lambdaFunc5, &object) == Value::nothing);
        IS_TRUE(callStatic(fn_lambdaFunc6, &object) == Value::nothing);

        IS_TRUE(callStatic(fn_funcWrapper1, &object, 10) == Value(10));
        IS_TRUE(callStatic(fn_funcWrapper2, &object, 10) == Value(30));
        IS_TRUE(callStatic(fn_funcWrapper3, &object, 10) == Value(60));

        auto const& mc = ponder::classByType<MyClass>();
        Value r = FunctionCaller(fn_nonClassFunc1).call();
        IS_TRUE(r.to<int>() == 77);

        Value r2 = callStatic(fn_nonClassFunc2, 2.5f, 3.0f);
        REQUIRE(r2.to<float>() == 7.5f);

        Value ncr = callStatic(fn_nonCopyRef);
        Value ncp = callStatic(fn_nonCopyPtr);
    }

    SECTION("calling null functions is an error")
    {
        using ponder::runtime::ObjectCaller;
        using ponder::runtime::FunctionCaller;

        REQUIRE_THROWS_AS(FunctionCaller(fn_nonMember1).call(ponder::Args(ponder::UserObject())),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(FunctionCaller(fn_nonMember3).call(ponder::Args(ponder::UserObject())),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(ObjectCaller(fn_member1).call(ponder::UserObject(), ponder::Args()),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(ObjectCaller(fn_member2).call(ponder::UserObject(), ponder::Args()),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(ObjectCaller(fn_member3).call(ponder::UserObject(), ponder::Args()),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(ObjectCaller(fn_memberParams1).call(ponder::UserObject(), ponder::Args()),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(FunctionCaller(fn_lambdaFunc2).call(ponder::Args(ponder::UserObject())),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(FunctionCaller(fn_lambdaFunc3).call(ponder::Args(ponder::UserObject())),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(FunctionCaller(fn_lambdaFunc5).call(ponder::Args(ponder::UserObject())),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(FunctionCaller(fn_lambdaFunc6).call(ponder::Args(ponder::UserObject())),
                          ponder::NullObject);
    }

    SECTION("calling a function without enough parameters is an error")
    {
        using ponder::runtime::call;

        MyClass object;

        REQUIRE_THROWS_AS(call(fn_nonMember2, &object, ponder::Args()),
                          ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(call(fn_member4, &object, ponder::Args()),
                          ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(call(fn_memberParams2, &object, ponder::Args()),
                        ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(call(fn_memberParams3, &object, ponder::Args(1.)),
                          ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(call(fn_memberParams4, &object, ponder::Args(1, 2)),
                          ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(call(fn_memberParams5, &object, ponder::Args("1", "2", "3")),
                          ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(call(fn_memberParams6, &object, ponder::Args(Zero, One, Two, Zero)),
                          ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(call(fn_funcWrapper1, &object, ponder::Args()),
                          ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(call(fn_funcWrapper2, &object, ponder::Args()),
                          ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(call(fn_funcWrapper3, &object, ponder::Args()),
                          ponder::NotEnoughArguments);
    }

   SECTION("functions must be called with the correct parameters")
   {
       using ponder::runtime::call;
       using ponder::runtime::callStatic;

       MyClass object;
       MyType arg(0);

       REQUIRE_THROWS_AS(callStatic(fn_nonMember2, ponder::Args(&object, arg, arg)),
                         ponder::BadArgument);
       REQUIRE_THROWS_AS(call(fn_memberParams3, &object, ponder::Args(arg, arg)),
                         ponder::BadArgument);
       REQUIRE_THROWS_AS(call(fn_memberParams4, &object, ponder::Args(arg, arg, arg)),
                         ponder::BadArgument);
       REQUIRE_THROWS_AS(call(fn_memberParams5, &object, ponder::Args(arg, arg, arg, arg)),
                         ponder::BadArgument);
       REQUIRE_THROWS_AS(call(fn_memberParams6, &object, ponder::Args(arg, arg, arg, arg, arg)),
                         ponder::BadArgument);
       REQUIRE_THROWS_AS(callStatic(fn_funcWrapper1, ponder::Args(&object, arg)),
                         ponder::BadArgument);
       REQUIRE_THROWS_AS(callStatic(fn_funcWrapper2, ponder::Args(&object, arg)),
                         ponder::BadArgument);
       REQUIRE_THROWS_AS(callStatic(fn_funcWrapper3, ponder::Args(&object, arg)),
                         ponder::BadArgument);
   }
}

TEST_CASE("Functions can modify objects")
{
    // modifyData() is called on an object of class A with the intent to modify that object:
    DataHolder objectA;

    const ponder::Class& metaClassB = ponder::classByType<FunctionTest::DataModifier>();
    ponder::runtime::ObjectFactory bfact(metaClassB);
    ponder::UserObject wrapperB =  bfact.construct();
    ponder::runtime::ObjectCaller functionB(metaClassB.function("modifyData"));

    REQUIRE(objectA.TestMember == 0);
    functionB.call(wrapperB, &objectA);
    REQUIRE(objectA.TestMember == 5);

    bfact.destroy(wrapperB);
}

