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

CAMP_AUTO_TYPE(MapperTest::MyClass, &MapperTest::declare);

#endif // CAMPTEST_MAPPER_HPP
