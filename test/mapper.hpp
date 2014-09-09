/****************************************************************************
**
** This file is part of the CAMP library.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
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

#ifndef CAMPTEST_MAPPER_HPP
#define CAMPTEST_MAPPER_HPP

#include <camp/camptype.hpp>
#include <camp/class.hpp>
#include <camp/simpleproperty.hpp>
#include <camp/function.hpp>
#include <map>
#include <string>

namespace MapperTest
{
    struct MyClass
    {
        enum
        {
            propertyCount = 5,
            functionCount = 3
        };

        static std::string property(std::size_t index)
        {
            const char* names[] = {"prop0", "prop1", "prop2", "prop3", "prop4"};
            return names[index];
        }

        static std::string function(std::size_t index)
        {
            const char* names[] = {"func0", "func1", "func2"};
            return names[index];
        }

        MyClass()
        {
            m_props[property(0)] = 0;
            m_props[property(1)] = 10;
            m_props[property(2)] = 20;
            m_props[property(3)] = 30;
            m_props[property(4)] = 40;
        }

        int& prop(const std::string& name)
        {
            return m_props[name];
        }

        std::string func(const std::string& name)
        {
            return name + "_called";
        }

        std::map<std::string, int> m_props;
    };

    template <typename T>
    struct MyMapper
    {
        std::size_t propertyCount()
        {
            return T::propertyCount;
        }

        camp::Property* property(std::size_t index)
        {
            return new MyProperty(T::property(index));
        }

        std::size_t functionCount()
        {
            return T::functionCount;
        }

        camp::Function* function(std::size_t index)
        {
            return new MyFunction(T::function(index));
        }

        struct MyProperty : public camp::SimpleProperty
        {
        public:

            MyProperty(const std::string& name)
                : camp::SimpleProperty(name, camp::intType)
            {
            }

            virtual camp::Value getValue(const camp::UserObject& object) const
            {
                T& t = object.get<T>();
                return t.prop(name());
            }

            virtual void setValue(const camp::UserObject& object, const camp::Value& value) const
            {
                T& t = object.get<T>();
                t.prop(name()) = value.to<int>();
            }
        };

        class MyFunction : public camp::Function
        {
        public:

            MyFunction(const std::string& name)
                : camp::Function(name, camp::stringType)
            {
            }

            virtual camp::Value execute(const camp::UserObject& object, const camp::Args&) const
            {
                T& t = object.get<T>();
                return t.func(name());
            }
        };
    };

    void declare()
    {
        camp::Class::declare<MyClass>("MapperTest::MyClass")
            .external<MyMapper>();
    }
}

CAMP_AUTO_TYPE(MapperTest::MyClass, &MapperTest::declare)

#endif // CAMPTEST_MAPPER_HPP
