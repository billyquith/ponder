
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

#include "test.hpp"
#include <ponder/uses/serialise.hpp>
#include <ponder/classbuilder.hpp>

namespace SerialiseTest
{
    class Simple
    {
    public:
        Simple(int i, const std::string& s, float f)
        :   m_i(i)
        ,   m_s(s)
        ,   m_f(f)
        {}
        
        float getF() const { return m_f; }
        void setF(float f) { m_f = f; }
        
        int m_i;
        std::string m_s;

    private:
        float m_f;
    };
    
    static void declare()
    {
        ponder::Class::declare<Simple>()
            .property("int", &Simple::m_i)
            .property("float", &Simple::getF, &Simple::setF)
            .property("string", &Simple::m_s)
            ;
    }
}

PONDER_AUTO_TYPE(SerialiseTest::Simple, &SerialiseTest::declare)

using namespace SerialiseTest;

//-----------------------------------------------------------------------------
//                         Tests for serialisation
//-----------------------------------------------------------------------------

TEST_CASE("Serialise saves an object state")
{
    SECTION("simple")
    {
//        auto const& mc = ponder::classByType<Simple>();
//        auto& s = ponder::runtime;
        auto s = new Simple(78, std::string("yadda"), 99.25f);
        REQUIRE(s != nullptr);
        
        ponder::archive::ObjectWriter<ponder::archive::Archive<ponder::archive::TextWriter>>> archive;
        
        serialise(archive, ponder::UserObject::makeRef(*s));
    }
}


//TEST_CASE("Serialise saves an object state")
//{
//    SECTION("simple")
//    {
//    }
//}


