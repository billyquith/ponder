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
    
    static bool operator == (const MyType& left, const MyType& right)
    {
        return left.x == right.x;
    }
    
    static std::ostream& operator << (std::ostream& stream, const MyType& object)
    {
        return stream << object.x;
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
        std::string p3;
        
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
        void f12(const std::string&, std::string, const std::string&, std::string) {}
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
    };
    
    void f1(MyClass& object)
    {
        object.p1 = true;
    }
    
    int f2(MyClass object, int x)
    {
        return object.p2 + x;
    }
    
    const std::string& f3(const MyClass* object)
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
        
        ponder::Class::declare<MyBase>("FunctionTest::MyBase");
        
        ponder::Class::declare<MyClass>("FunctionTest::MyClass")
            .base<MyBase>()
            
            // ***** non-member functions *****
            .function("f1", &f1) // object by reference
            .function("f2", &f2) // object by value + parameter
            .function("f3", &f3) // object by pointer
            
            // ***** member functions *****
            .function("f4", &MyClass::f4) // non-const
            .function("f5", &MyClass::f5) // const
            .function("f6", &MyClass::f6) // inherited
            .function("f7", &MyClass::f7) // ponder::Value as return and argument types
            
            // ***** arguments count ******
            .function("f8",  &MyClass::f8)  // 0 argument
            .function("f9",  &MyClass::f9)  // 1 argument
            .function("f10", &MyClass::f10) // 2 arguments
            .function("f11", &MyClass::f11) // 3 arguments
            .function("f12", &MyClass::f12) // 4 arguments
            .function("f13", &MyClass::f13) // 5 arguments
            
            // ***** nested functions *****
            //
            // These were previously indirected
            //
            .function("f14",
                      std::function<void(MyClass&)>([](MyClass& self){ self.inner.f14(); }))
            // getter returning an object
            .function("f15",
                      std::function<void(MyClass&)>([](MyClass& self){ self.getInner().f15(); }))
            // raw pointer
            .function("f16",
                      std::function<int(MyClass&)>([](MyClass& self){
                          return self.innerPtr->f16();
                      }))
            // getter returning a raw pointer
            .function("f17",
                      std::function<void(MyClass const&)>([](MyClass const& self){
                          self.getInnerPtr()->f17(99);
                      }))
            // smart pointer
            .function("f18", std::function<void(MyClass&)>([](MyClass& self){
                          self.innerSmartPtr.get()->f18();
                      }))
            // getter returning a smart pointer
            .function("f19", std::function<void(MyClass&)>([](MyClass& self){
                          self.getInnerSmartPtr().get()->f19();
                      }))
        
            // ***** std::function *****
            .function("f20",
                      std::function<int (MyClass&, int)>(std::bind(&MyClass::f20, _1, _2)))
            .function("f21",
                      std::function<int (MyClass&, int)>(std::bind(&MyClass::f21, _1, _2, 20)))
            .function("f22",
                      std::function<int (MyClass&, int)>(
                          std::bind(std::bind(&MyClass::f22, _1, _2, _3, 30), _1, _2, 20)))
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
PONDER_AUTO_TYPE(FunctionTest::MyBase,  &FunctionTest::declare)
PONDER_AUTO_TYPE(FunctionTest::ClassA,  &FunctionTest::declare)
PONDER_AUTO_TYPE(FunctionTest::ClassB,  &FunctionTest::declare)

using namespace FunctionTest;

//-----------------------------------------------------------------------------
//                         Tests for ponder::Function
//-----------------------------------------------------------------------------

TEST_CASE("Ponder supports functions")
{
    const ponder::Function* functions[23];
    
    const ponder::Class& metaclass = ponder::classByType<MyClass>();
    for (int i = 1; i <= 22; ++i)
    {
        functions[i] = &metaclass.function("f" + std::to_string(i));
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
    }

    SECTION("functions have a number of arguments")
    {
        REQUIRE(functions[1]->argCount() ==  0);
        REQUIRE(functions[2]->argCount() ==  1);
        REQUIRE(functions[3]->argCount() ==  0);
        REQUIRE(functions[4]->argCount() ==  0);
        REQUIRE(functions[5]->argCount() ==  0);
        REQUIRE(functions[6]->argCount() ==  0);
        REQUIRE(functions[7]->argCount() ==  1);
        REQUIRE(functions[8]->argCount() ==  0);
        REQUIRE(functions[9]->argCount() ==  1);
        REQUIRE(functions[10]->argCount() == 2);
        REQUIRE(functions[11]->argCount() == 3);
        REQUIRE(functions[12]->argCount() == 4);
        REQUIRE(functions[13]->argCount() == 5);
        REQUIRE(functions[14]->argCount() == 0);
        REQUIRE(functions[15]->argCount() == 0);
        REQUIRE(functions[16]->argCount() == 0);
        REQUIRE(functions[17]->argCount() == 0);
        REQUIRE(functions[18]->argCount() == 0);
        REQUIRE(functions[19]->argCount() == 0);
        REQUIRE(functions[20]->argCount() == 1);
        REQUIRE(functions[21]->argCount() == 1);
        REQUIRE(functions[22]->argCount() == 1);
    }
    
    SECTION("function arguments have a type")
    {
        REQUIRE_THROWS_AS(functions[1]->argType(0),  ponder::OutOfRange);
        REQUIRE(functions[2]->argType(0) ==  ponder::ValueType::Integer);
        REQUIRE_THROWS_AS(functions[3]->argType(0),  ponder::OutOfRange);
        REQUIRE_THROWS_AS(functions[4]->argType(0),  ponder::OutOfRange);
        REQUIRE_THROWS_AS(functions[5]->argType(0),  ponder::OutOfRange);
        REQUIRE_THROWS_AS(functions[6]->argType(0),  ponder::OutOfRange);
        REQUIRE(functions[7]->argType(0) ==  ponder::ValueType::User);
        REQUIRE_THROWS_AS(functions[8]->argType(0),  ponder::OutOfRange);
        REQUIRE(functions[9]->argType(0) ==  ponder::ValueType::Boolean);
        REQUIRE(functions[10]->argType(0) == ponder::ValueType::Real);
        REQUIRE(functions[10]->argType(1) == ponder::ValueType::Real);
        REQUIRE(functions[11]->argType(0) == ponder::ValueType::Integer);
        REQUIRE(functions[11]->argType(1) == ponder::ValueType::Integer);
        REQUIRE(functions[11]->argType(2) == ponder::ValueType::Integer);
        REQUIRE(functions[12]->argType(0) == ponder::ValueType::String);
        REQUIRE(functions[12]->argType(1) == ponder::ValueType::String);
        REQUIRE(functions[12]->argType(2) == ponder::ValueType::String);
        REQUIRE(functions[12]->argType(3) == ponder::ValueType::String);
        REQUIRE(functions[13]->argType(0) == ponder::ValueType::Enum);
        REQUIRE(functions[13]->argType(1) == ponder::ValueType::Enum);
        REQUIRE(functions[13]->argType(2) == ponder::ValueType::Enum);
        REQUIRE(functions[13]->argType(3) == ponder::ValueType::Enum);
        REQUIRE(functions[13]->argType(4) == ponder::ValueType::Enum);
        REQUIRE_THROWS_AS(functions[14]->argType(0), ponder::OutOfRange);
        REQUIRE_THROWS_AS(functions[15]->argType(0), ponder::OutOfRange);
        REQUIRE_THROWS_AS(functions[16]->argType(0), ponder::OutOfRange);
        REQUIRE_THROWS_AS(functions[17]->argType(0), ponder::OutOfRange);
        REQUIRE_THROWS_AS(functions[18]->argType(0), ponder::OutOfRange);
        REQUIRE_THROWS_AS(functions[19]->argType(0), ponder::OutOfRange);
        REQUIRE(functions[20]->argType(0) == ponder::ValueType::Integer);
        REQUIRE(functions[21]->argType(0) == ponder::ValueType::Integer);
        REQUIRE(functions[22]->argType(0) == ponder::ValueType::Integer);
    }
    
    SECTION("Functions can be called")
    {
        MyClass object;
        
        IS_TRUE(functions[1]->call(object, ponder::Args()) == ponder::Value::nothing);
        IS_TRUE(functions[2]->call(object, ponder::Args(10)) == ponder::Value(12));
        IS_TRUE(functions[3]->call(object, ponder::Args()) == ponder::Value("3"));
        IS_TRUE(functions[4]->call(object, ponder::Args()).to<MyType>() == MyType(4));
        IS_TRUE(functions[5]->call(object, ponder::Args()).to<MyType>() == MyType(5));
        IS_TRUE(functions[6]->call(object, ponder::Args()) == ponder::Value::nothing);
        IS_TRUE(functions[7]->call(object, ponder::Args("hi")) == ponder::Value("hi"));
        IS_TRUE(functions[8]->call(object, ponder::Args()) == ponder::Value::nothing);
        IS_TRUE(functions[9]->call(object, ponder::Args(true)) == ponder::Value::nothing);
        IS_TRUE(functions[10]->call(object, ponder::Args(1., 2.f)) == ponder::Value::nothing);
        IS_TRUE(functions[11]->call(object, ponder::Args(1, 2, 3)) == ponder::Value::nothing);
        IS_TRUE(functions[12]->call(object, ponder::Args("1", "2", "3", "4"))
                 == ponder::Value::nothing);
        IS_TRUE(functions[13]->call(object, ponder::Args(Zero, One, Two, Zero, One))
                 == ponder::Value::nothing);
        IS_TRUE(functions[14]->call(object, ponder::Args()) == ponder::Value::nothing);
        IS_TRUE(functions[15]->call(object, ponder::Args()) == ponder::Value::nothing);
        IS_TRUE(functions[16]->call(object, ponder::Args()) == ponder::Value(16));
        IS_TRUE(functions[17]->call(object, ponder::Args(20)) == ponder::Value::nothing);
        IS_TRUE(functions[18]->call(object, ponder::Args()) == ponder::Value::nothing);
        IS_TRUE(functions[19]->call(object, ponder::Args()) == ponder::Value::nothing);
        IS_TRUE(functions[20]->call(object, ponder::Args(10)) == ponder::Value(10));
        IS_TRUE(functions[21]->call(object, ponder::Args(10)) == ponder::Value(30));
        IS_TRUE(functions[22]->call(object, ponder::Args(10)) == ponder::Value(60));
    }
    
    SECTION("calling null functions is an error")
    {
        REQUIRE_THROWS_AS(functions[1]->call(ponder::UserObject(),  ponder::Args()),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(functions[3]->call(ponder::UserObject(), ponder::Args()),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(functions[4]->call(ponder::UserObject(), ponder::Args()),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(functions[5]->call(ponder::UserObject(), ponder::Args()),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(functions[6]->call(ponder::UserObject(), ponder::Args()),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(functions[8]->call(ponder::UserObject(), ponder::Args()),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(functions[15]->call(ponder::UserObject(), ponder::Args()),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(functions[16]->call(ponder::UserObject(), ponder::Args()),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(functions[18]->call(ponder::UserObject(), ponder::Args()),
                          ponder::NullObject);
        REQUIRE_THROWS_AS(functions[19]->call(ponder::UserObject(), ponder::Args()),
                          ponder::NullObject);
    }
    
    SECTION("calling a function without enough arguments is an error")
    {
        MyClass object;
        
        REQUIRE_THROWS_AS(functions[2]->call(object, ponder::Args()),
                          ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(functions[7]->call(object, ponder::Args()),
                          ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(functions[9]->call(object, ponder::Args()),
                          ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(functions[10]->call(object, ponder::Args(1.)),
                          ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(functions[11]->call(object, ponder::Args(1, 2)),
                          ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(functions[12]->call(object, ponder::Args("1", "2", "3")),
                          ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(functions[13]->call(object, ponder::Args(Zero, One, Two, Zero)),
                          ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(functions[20]->call(object, ponder::Args()),
                          ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(functions[21]->call(object, ponder::Args()),
                          ponder::NotEnoughArguments);
        REQUIRE_THROWS_AS(functions[22]->call(object, ponder::Args()),
                          ponder::NotEnoughArguments);
    }

    SECTION("functions must be called with the correct arguments")
    {
        MyClass object;
        MyType arg(0);

        REQUIRE_THROWS_AS(functions[2]->call(object, ponder::Args(arg)),
                          ponder::BadArgument);
        REQUIRE_THROWS_AS(functions[10]->call(object, ponder::Args(arg, arg)),
                          ponder::BadArgument);
        REQUIRE_THROWS_AS(functions[11]->call(object, ponder::Args(arg, arg, arg)),
                          ponder::BadArgument);
        REQUIRE_THROWS_AS(functions[12]->call(object, ponder::Args(arg, arg, arg, arg)),
                          ponder::BadArgument);
        REQUIRE_THROWS_AS(functions[13]->call(object, ponder::Args(arg, arg, arg, arg, arg)),
                          ponder::BadArgument);
        REQUIRE_THROWS_AS(functions[20]->call(object, ponder::Args(arg)),
                          ponder::BadArgument);
        REQUIRE_THROWS_AS(functions[21]->call(object, ponder::Args(arg)),
                          ponder::BadArgument);
        REQUIRE_THROWS_AS(functions[22]->call(object, ponder::Args(arg)),
                          ponder::BadArgument);
    }
}

TEST_CASE("Functions can modify objects")
{
    // ModifyA() is called on an object of class A with the intend to modify that object:
    ClassA objectA;
    
    const ponder::Class& metaClassB = ponder::classByType<FunctionTest::ClassB>();
    ponder::UserObject wrapperB = metaClassB.construct();
    const ponder::Function& functionB = metaClassB.function("ModifyA");
    
    REQUIRE(objectA.TestMember == 0);
    functionB.call(wrapperB, &objectA);
    REQUIRE(objectA.TestMember == 5);
    
    metaClassB.destroy(wrapperB);
}

