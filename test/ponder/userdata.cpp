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

#include <ponder/classbuilder.hpp>
#include "test.hpp"

namespace UserDataTest
{
    class HasUserData
    {
    public:
        void noData(int) {}
        void data1(float) {}
        void data2(double) {}
    };
    
    void declare()
    {
        ponder::Class::declare<HasUserData>()
            .function("none", &HasUserData::noData)()
            .function("data1", &HasUserData::data1)(
                ponder::UserData("value", 3)
            )
            .function("data2", &HasUserData::data2)(
                ponder::UserData("value", 2.5f),
                ponder::UserData("value2", "xyz")
            )
            ;
    }
}

PONDER_AUTO_TYPE(UserDataTest::HasUserData, &UserDataTest::declare)

using namespace UserDataTest;

//-----------------------------------------------------------------------------
//                         Tests for ponder::user
//-----------------------------------------------------------------------------

TEST_CASE("User data is stored in a store")
{
    SECTION("")
    {
        auto const& cls = ponder::classByType<HasUserData>();
        
        ponder::userDataStore()->setValue(cls, "test1", ponder::Value(2345));
        const ponder::Value* uv = ponder::userDataStore()->getValue(cls, "test1");
    }
}

TEST_CASE("Class members can have user data")
{
    const ponder::Class& cls = ponder::classByType<HasUserData>();

    SECTION("Empty user data")
    {
        REQUIRE(ponder::userDataStore()->getValue(cls.function("none"), "name") == nullptr);
    }

    SECTION("Per member user data")
    {
        auto uds = ponder::userDataStore();
        REQUIRE(cls.hasFunction("data1"));
        const ponder::Value *uv = uds->getValue(cls.function("data1"), "value");
        REQUIRE(uv != nullptr);
        REQUIRE(*uv == ponder::Value(3));

        REQUIRE(cls.hasFunction("data2"));
        REQUIRE(*uds->getValue(cls.function("data2"), "value") == ponder::Value(2.5f));
        REQUIRE(*uds->getValue(cls.function("data2"), "value2") == ponder::Value("xyz"));
    }
}

