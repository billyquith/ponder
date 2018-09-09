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
#include <ponder/simpleproperty.hpp>
#include <ponder/function.hpp>
#include <ponder/classbuilder.hpp>
#include "test.hpp"
#include <map>
#include <string>

// TODO: Fix this

//namespace MapperTest
//{
//    struct MyClass
//    {
//        enum
//        {
//            propertyCount = 5,
//            functionCount = 3
//        };
//
//        static ponder::String property(std::size_t index)
//        {
//            const char* names[] = {"prop0", "prop1", "prop2", "prop3", "prop4"};
//            return names[index];
//        }
//
//        static ponder::String function(std::size_t index)
//        {
//            const char* names[] = {"func0", "func1", "func2"};
//            return names[index];
//        }
//
//        MyClass()
//        {
//            m_props[property(0)] = 0;
//            m_props[property(1)] = 10;
//            m_props[property(2)] = 20;
//            m_props[property(3)] = 30;
//            m_props[property(4)] = 40;
//        }
//
//        int& prop(const ponder::String& name)
//        {
//            return m_props[name];
//        }
//
//        ponder::String func(const ponder::String& name)
//        {
//            return name + "_called";
//        }
//
//        std::map<ponder::String, int> m_props;
//    };
//
//    template <typename T>
//    struct MyMapper
//    {
//        std::size_t propertyCount()
//        {
//            return T::propertyCount;
//        }
//
//        ponder::Property* property(std::size_t index)
//        {
//            return new MyProperty(T::property(index));
//        }
//
//        std::size_t functionCount()
//        {
//            return T::functionCount;
//        }
//
//        ponder::Function* function(std::size_t index)
//        {
//            return new MyFunction(T::function(index));
//        }
//
//        struct MyProperty final : public ponder::SimpleProperty
//        {
//        public:
//
//            MyProperty(const ponder::String& name)
//            : ponder::SimpleProperty(name, ponder::ValueKind::Integer)
//            {
//            }
//
//            ponder::Value getValue(const ponder::UserObject& object) const final
//            {
//                T& t = object.get<T>();
//                return t.prop(name());
//            }
//
//            void setValue(const ponder::UserObject& object,
//                          const ponder::Value& value) const final
//            {
//                T& t = object.get<T>();
//                t.prop(name()) = value.to<int>();
//            }
//        };
//
////        class MyFunction : public ponder::Function
////        {
////        public:
////
////            MyFunction(const ponder::String& name)
////            : ponder::Function(name, ponder::ValueKind::String)
////            {
////            }
////
////            virtual ponder::Value execute(const ponder::UserObject& object,
////                                          const ponder::Args&) const
////            {
////                T& t = object.get<T>();
////                return t.func(name());
////            }
////        };
//    };
//
//    void declare()
//    {
//        ponder::Class::declare<MyClass>("MapperTest::MyClass")
//            .external<MyMapper>();
//    }
//}
//
//PONDER_AUTO_TYPE(MapperTest::MyClass, &MapperTest::declare)
//
//using namespace MapperTest;

//-----------------------------------------------------------------------------
//                         Tests for mappers
//-----------------------------------------------------------------------------

//TEST_CASE("Mapper test data checks")
//{
//    const ponder::Class* metaclass = &ponder::classByType<MyClass>();
//
//    REQUIRE(metaclass != nullptr);
//
//    SECTION("count")
//    {
//        REQUIRE(metaclass->propertyCount() == static_cast<std::size_t>(MyClass::propertyCount));
//        REQUIRE(metaclass->functionCount() == static_cast<std::size_t>(MyClass::functionCount));
//    }
//
//    SECTION("check types")
//    {
//        IS_TRUE(metaclass->property(0).kind() == ponder::ValueKind::Integer);
//        IS_TRUE(metaclass->function(0).returnType() == ponder::ValueKind::String);
//    }
//
//    SECTION("paramCount")
//    {
//        REQUIRE(metaclass->function(0).paramCount() == 0);
//    }
//
//    SECTION("propertyGet")
//    {
//        MyClass object;
//
//        REQUIRE(metaclass->property("prop0").get(object) == ponder::Value(object.prop("prop0")));
//        REQUIRE(metaclass->property("prop1").get(object) == ponder::Value(object.prop("prop1")));
//        REQUIRE(metaclass->property("prop2").get(object) == ponder::Value(object.prop("prop2")));
//        REQUIRE(metaclass->property("prop3").get(object) == ponder::Value(object.prop("prop3")));
//        REQUIRE(metaclass->property("prop4").get(object) == ponder::Value(object.prop("prop4")));
//    }
//
//    SECTION("propertySet")
//    {
//        MyClass object;
//
//        metaclass->property("prop0").set(object, 0);
//        metaclass->property("prop1").set(object, 100);
//        metaclass->property("prop2").set(object, 200);
//        metaclass->property("prop3").set(object, 300);
//        metaclass->property("prop4").set(object, 400);
//
//        REQUIRE(object.prop("prop0") == 0);
//        REQUIRE(object.prop("prop1") == 100);
//        REQUIRE(object.prop("prop2") == 200);
//        REQUIRE(object.prop("prop3") == 300);
//        REQUIRE(object.prop("prop4") == 400);
//    }
//
////    SECTION("functionCall")
////    {
////        MyClass object;
////
////        REQUIRE(metaclass->function("func0").call(object) == ponder::Value(object.func("func0")));
////        REQUIRE(metaclass->function("func1").call(object) == ponder::Value(object.func("func1")));
////        REQUIRE(metaclass->function("func2").call(object) == ponder::Value(object.func("func2")));
////    }
//}



