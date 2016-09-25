/****************************************************************************
**
** This file is part of the Ponder library.
**
** The MIT License (MIT)
**
** Copyright (C) 2016 Bill Quith.
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

#include <ponder/uses/report.hpp>
#include <ponder/classvisitor.hpp>
#include <ponder/detail/format.hpp>
#include <stack>
#include <iostream>

#include <ponder/simpleproperty.hpp>
#include <ponder/arrayproperty.hpp>
#include <ponder/enumproperty.hpp>
#include <ponder/userproperty.hpp>

#include <ponder/type.hpp>

#include "picojson.h"

using namespace ponder;
using ponder::detail::string_view;

namespace {
    
    using namespace ponder::detail::fmt;

    class Reporter
    {
    public:        
        virtual ~Reporter() {}
        virtual void open(string_view name) {}
        virtual void close() {}
        virtual void info(string_view key, string_view value) {}
    };
    
    
    class TextReporter : public Reporter
    {
        std::stack<string_view> m_what;

        void indent()
        {
            int ind = m_what.size();
            while (ind-- > 0)
                print("  ");
        }

    public:
        
        virtual ~TextReporter() {}
        
        void open(string_view name) override
        {
            indent();
            print("{}:\n", name);
            m_what.push(name);
        }
        
        void close() override
        {
            m_what.pop();
        }
        
        void info(string_view key, string_view value) override
        {
            indent();
            print("{} = {}\n", key, value);
        }
    };

//    class JsonReporter : public Reporter
//    {
//        std::stack<json::object_t> m_what;
//        json::array_t m_root;
//        
//    public:
//        
//        JsonReporter() {}
//        virtual ~JsonReporter() {}
//        
//        std::string toJson()
//        {
////            json::object_t ret;
////            ret["classes"] = m_root;
////            return ret;
//            return "";
//        }
//        
//        void open(string_view name) override
//        {
//            json::object_t node;
//            m_root.push_back(node);
//            m_what.push(node);
//        }
//        
//        void close() override
//        {
//            m_what.pop();
//        }
//        
//        void info(string_view key, string_view value) override
//        {
//            m_what.top()[key.to_string()] = value.to_string();
//        }
//    };

    class ReportVisitor : public ponder::ClassVisitor
    {
        Reporter &m_rp;
        
    public:
        
        ReportVisitor(Reporter &rp) : m_rp(rp) {}
    
        virtual ~ReportVisitor() {}

        virtual void visit(const Property& property) override
        {
            m_rp.open("Property");
            m_rp.info("name", property.name());
            m_rp.info("kind", enumByType<ValueKind>().name(property.kind()));            
            m_rp.close();
        }

        virtual void visit(const SimpleProperty& property) override
        {
            m_rp.open("SimpleProperty");
            m_rp.info("name", property.name());
            m_rp.info("kind", enumByType<ValueKind>().name(property.kind()));
            m_rp.close();
        }

        virtual void visit(const ArrayProperty& property) override
        {
            m_rp.open("ArrayProperty");
            m_rp.info("name", property.name());
            m_rp.info("kind", enumByType<ValueKind>().name(property.kind()));
            m_rp.close();
        }

        virtual void visit(const EnumProperty& property) override
        {
            m_rp.open("EnumProperty");
            m_rp.info("name", property.name());
            m_rp.info("kind", enumByType<ValueKind>().name(property.kind()));
            m_rp.close();
        }

        virtual void visit(const UserProperty& property) override
        {
            m_rp.open("UserProperty");
            m_rp.info("name", property.name());
            m_rp.info("kind", enumByType<ValueKind>().name(property.kind()));
            m_rp.close();
        }

        virtual void visit(const Function& function) override
        {
            m_rp.open("Function");
            m_rp.info("name", function.name());
            m_rp.info("kind", enumByType<FunctionKind>().name(function.kind()));
            m_rp.close();
        }
    };

} // anonymous namespace

static void declarePonder()
{
    Enum::declare<ponder::ValueKind>()
        .value("None",              ValueKind::None)   
        .value("Boolean",           ValueKind::Boolean)
        .value("Integer",           ValueKind::Integer)
        .value("Real",              ValueKind::Real)   
        .value("String",            ValueKind::String) 
        .value("Enum",              ValueKind::Enum)   
        .value("Array",             ValueKind::Array)  
        ;

    Enum::declare<ponder::FunctionKind>()
        .value("None",              FunctionKind::None)
        .value("Function",          FunctionKind::Function)
        .value("MemberFunction",    FunctionKind::MemberFunction)
        .value("MemberObject",      FunctionKind::MemberObject)
        .value("FunctionWrapper",   FunctionKind::FunctionWrapper)
        .value("BindExpression",    FunctionKind::BindExpression)
        .value("Lambda",            FunctionKind::Lambda)
        ;
}

PONDER_TYPE(ValueKind);
PONDER_TYPE(FunctionKind);

static void reportTo(Reporter &rep)
{
    auto const& clmgr = detail::ClassManager::instance();
    auto nbClasses = clmgr.count();
    
    ReportVisitor repVis(rep);
    for (auto ci = 0; ci < nbClasses; ++ci)
    {
        auto const& cls = clmgr.getByIndex(ci);
        rep.open("Class");
        rep.info("name", cls.name());
        cls.visit(repVis);
        rep.close();
    }
}

void ponder::uses::reportAll()
{
    declarePonder();
    
    {
        TextReporter rep;
        reportTo(rep);
    }
    
//    {
//        JsonReporter rep;
//        reportTo(rep);
//        std::cout << rep.toJson() << std::endl;
//    }
}
