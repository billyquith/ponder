
/****************************************************************************
 **
 ** This file is part of the Ponder library, formerly CAMP.
 **
 ** The MIT License (MIT)
 **
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

#include <iostream>

#include "test.hpp"
#include <ponder/uses/archive/rapidxml.hpp>
#include <ponder/uses/serialise.hpp>
#include <ponder/classbuilder.hpp>
#include <rapidxml/rapidxml_print.hpp>
#include <iostream>
#include <sstream>

namespace SerialiseTest
{
    class Simple
    {
    public:
        Simple() : m_i(0), m_f(0.f) {}
        
        Simple(int i, const std::string& s, float f)
        :   m_i(i)
        ,   m_s(s)
        ,   m_f(f)
        {}
        
        float getF() const { return m_f; }
        void setF(float f) { m_f = f; }
        
        int m_i;
        std::string m_s;
        std::vector<int> m_v;
        
    private:
        float m_f;
    };
    
    class Ref
    {
    public:
        Ref() : m_ref(nullptr) {}
        ~Ref() {}
        
        Simple m_instance;
        Simple *m_ref;
    };
    
    static void declare()
    {
        ponder::Class::declare<Simple>()
            .property("int", &Simple::m_i)
            .property("float", &Simple::getF, &Simple::setF)
            .property("string", &Simple::m_s)
            .property("vector", &Simple::m_v)
            ;
        
        ponder::Class::declare<Ref>()
            .property("instance", &Ref::m_instance)
//            .property("ref", &Ref::m_ref)
            ;
    }
}

PONDER_AUTO_TYPE(SerialiseTest::Simple, &SerialiseTest::declare)
PONDER_AUTO_TYPE(SerialiseTest::Ref, &SerialiseTest::declare)

using namespace SerialiseTest;

//-----------------------------------------------------------------------------
//                         Tests for serialisation
//-----------------------------------------------------------------------------

TEST_CASE("Can serialise using RapidXML")
{
    SECTION("Member values")
    {
        std::string serialised;
        
        {
            std::unique_ptr<Simple> s = ponder::detail::make_unique<Simple>(78, std::string("yadda"), 99.25f);
            REQUIRE(s != nullptr);
            s->m_v = {3,6,9};
            
            rapidxml::xml_document<> doc;
            auto rootNode = doc.allocate_node(rapidxml::node_element, "simple");
            REQUIRE(rootNode != nullptr);
            doc.append_node(rootNode);
            
            ponder::archive::RapidXmlArchive<> archive;
            ponder::archive::ArchiveWriter<ponder::archive::RapidXmlArchive<>> writer(archive);
            writer.write(rootNode, ponder::UserObject::makeRef(*s));
            
            std::cout << doc;
            
            std::ostringstream ostrm;
            ostrm << doc;
            serialised = ostrm.str();
            doc.clear();
        }
        
        {
            std::unique_ptr<Simple> s2 = ponder::detail::make_unique<Simple>(0, "", 0.f);
            REQUIRE(s2 != nullptr);
            
            rapidxml::xml_document<> doc;
            doc.parse<rapidxml::parse_non_destructive>(const_cast<char*>(serialised.data()));
            auto rootNode = doc.first_node();
            REQUIRE(rootNode != nullptr);
            
            ponder::archive::RapidXmlArchive<> archive;
            ponder::archive::ArchiveReader<ponder::archive::RapidXmlArchive<>> reader(archive);
            reader.read(rootNode, ponder::UserObject::makeRef(*s2));
            
            CHECK(s2->m_i == 78);
            CHECK(s2->getF() == 99.25f);
            CHECK(s2->m_s == std::string("yadda"));
            CHECK(s2->m_v == std::vector<int>({3,6,9}));
        }
    }
    
    // Simple nested object
    SECTION("Nested object")
    {
        std::string serialised;
        
        {
            std::unique_ptr<Ref> r{ new Ref };
            REQUIRE(r != nullptr);
            
            r->m_instance.m_i = 89;
            r->m_instance.setF(0.75f);
            r->m_instance.m_s = "stringy";

            rapidxml::xml_document<> doc;
            auto rootNode = doc.allocate_node(rapidxml::node_element, "ref");
            REQUIRE(rootNode != nullptr);
            doc.append_node(rootNode);
            
            ponder::archive::RapidXmlArchive<> archive;
            ponder::archive::ArchiveWriter<ponder::archive::RapidXmlArchive<>> writer(archive);
            writer.write(rootNode, ponder::UserObject::makeRef(*r));
            
            std::cout << doc;
            
            std::ostringstream ostrm;
            ostrm << doc;
            serialised = ostrm.str();
            doc.clear();
        }
        
        {
            std::unique_ptr<Ref> r = ponder::detail::make_unique<Ref>();
            REQUIRE(r != nullptr);

            rapidxml::xml_document<> doc;
            doc.parse<rapidxml::parse_non_destructive>(const_cast<char*>(serialised.data()));
            auto rootNode = doc.first_node();
            REQUIRE(rootNode != nullptr);

            ponder::archive::RapidXmlArchive<> archive;
            ponder::archive::ArchiveReader<ponder::archive::RapidXmlArchive<>> reader(archive);
            reader.read(rootNode, ponder::UserObject::makeRef(*r));

            CHECK(r->m_instance.m_i == 89);
            CHECK(r->m_instance.getF() == 0.75f);
            CHECK(r->m_instance.m_s == std::string("stringy"));
        }
    }

}




