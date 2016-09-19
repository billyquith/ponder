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

#include <ponder/simpleproperty.hpp>
#include <ponder/arrayproperty.hpp>
#include <ponder/enumproperty.hpp>
#include <ponder/userproperty.hpp>

#include <ponder/type.hpp>

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
        
    public:
        virtual ~TextReporter() {}
        
        void open(string_view name) override
        {
            m_what.push(name);
            print("{} [\n", name);
        }
        
        void close() override
        {
            print("]\n");
            m_what.pop();
        }
        
        void info(string_view key, string_view value) override
        {
            int ind = m_what.size();
            while (ind-- > 0)
                print("  ");
                
            print("{} = {}\n", key, value);
        }
    };
    

    class ReportVisitor : public ponder::ClassVisitor
    {
        Reporter &m_rp;
        
    public:
        ReportVisitor(Reporter &rp) : m_rp(rp) {}
    
        virtual ~ReportVisitor() {}

        virtual void visit(const Property& property) override
        {
            m_rp.open("property");
            m_rp.info("name", property.name());
            m_rp.info("kind", enumByType<ValueKind>().name(property.kind()));            
            m_rp.close();
        }

        virtual void visit(const SimpleProperty& property) override
        {
            m_rp.open("simple_property");
            m_rp.info("name", property.name());
            m_rp.info("kind", enumByType<ValueKind>().name(property.kind()));
            m_rp.close();
        }

        virtual void visit(const ArrayProperty& property) override
        {
            m_rp.open("array_property");
            m_rp.info("name", property.name());
            m_rp.info("kind", enumByType<ValueKind>().name(property.kind()));
            m_rp.close();
        }

        virtual void visit(const EnumProperty& property) override
        {
            m_rp.open("enum_property");
            m_rp.info("name", property.name());
            m_rp.info("kind", enumByType<ValueKind>().name(property.kind()));
            m_rp.close();
        }

        virtual void visit(const UserProperty& property) override
        {
            m_rp.open("user_property");
            m_rp.info("name", property.name());
            m_rp.info("kind", enumByType<ValueKind>().name(property.kind()));
            m_rp.close();
        }

        virtual void visit(const Function& function) override
        {
            m_rp.open("function");
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

void ponder::uses::reportAll()
{
    declarePonder();
    
    auto const& clmgr = detail::ClassManager::instance();
    auto nbClasses = clmgr.count();
    
    TextReporter tr;
    ReportVisitor repVis(tr);
    
    for (auto ci = 0; ci < nbClasses; ++ci)
    {
        auto const& cls = clmgr.getByIndex(ci);
        cls.visit(repVis);
    }
}
