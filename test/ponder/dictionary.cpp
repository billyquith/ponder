/****************************************************************************
 **
 ** This file is part of the Ponder library, formerly CAMP.
 **
 ** The MIT License (MIT)
 **
 ** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
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

#include <ponder/config.hpp>
#include <ponder/detail/dictionary.hpp>
#include "test.hpp"
#include <string.h>

using namespace ponder;

//-----------------------------------------------------------------------------
//                         Tests for Ponder detail::dictionary
//-----------------------------------------------------------------------------

TEST_CASE("Ponder has a dictionary")
{
    typedef ponder::detail::Dictionary<Id, IdRef, int> Dict;
    std::unique_ptr<Dict> dict(new Dict());
    
    REQUIRE((dict != nullptr));
    REQUIRE(dict->size() == 0);

    dict->insert("bravo", 1);
    dict->insert("alpha", 2);
    dict->insert("zebra", 3);
    dict->insert("foxtrot", 4);
    dict->insert("echo", 5);
    
    SECTION("contains items")
    {
        REQUIRE(dict->size() == 5);
        
        REQUIRE(dict->containsKey("bravo") == true);
        REQUIRE(dict->containsKey("zebra") == true);
        REQUIRE(dict->containsKey("foxtrot") == true);
        REQUIRE(dict->containsKey("bravo") == true);
        REQUIRE(dict->containsKey("echo") == true);
    }

    SECTION("test for missing keys")
    {
        REQUIRE(dict->containsKey("") == false);
        REQUIRE(dict->containsKey("monkey") == false);
    }
    
    SECTION("can find keys")
    {
        REQUIRE(dict->findKey("echo") != dict->end());
        REQUIRE(dict->findKey("xyz") == dict->end());
        REQUIRE(dict->findKey("") == dict->end());
    }

    SECTION("has values")
    {
        REQUIRE(dict->containsValue(1) == true);
        REQUIRE(dict->containsValue(2) == true);
        REQUIRE(dict->containsValue(3) == true);
        REQUIRE(dict->containsValue(4) == true);
        REQUIRE(dict->containsValue(5) == true);
    }

    SECTION("test for missing values")
    {
        REQUIRE(dict->containsValue(71237) == false);
        REQUIRE(dict->containsValue(9) == false);
    }

    SECTION("keys are sorted")
    {
        REQUIRE(dict->at(0)->first == Id("alpha"));
        REQUIRE(dict->at(1)->first == Id("bravo"));
        REQUIRE(dict->at(2)->first == Id("echo"));
        REQUIRE(dict->at(3)->first == Id("foxtrot"));
        REQUIRE(dict->at(4)->first == Id("zebra"));
    }
    
    SECTION("can iterate over key,values")
    {
        int count = 0;
        for (auto&& it : dict->getIterator())
        {
            REQUIRE(dict->at(count)->name() == it.name());
            REQUIRE(dict->at(count)->value() == it.value());
            ++count;
        }
        REQUIRE(count == 5);
    }

    SECTION("keys can be removed")
    {
        REQUIRE(dict->size() == 5);
        REQUIRE(dict->at(0)->first == Id("alpha"));
        REQUIRE(dict->at(1)->first == Id("bravo"));
        REQUIRE(dict->at(2)->first == Id("echo"));
        REQUIRE(dict->at(3)->first == Id("foxtrot"));
        REQUIRE(dict->at(4)->first == Id("zebra"));
        
        dict->erase("alpha");
        REQUIRE(dict->size() == 4);
        REQUIRE(dict->at(0)->first == Id("bravo"));
        REQUIRE(dict->at(1)->first == Id("echo"));
        REQUIRE(dict->at(2)->first == Id("foxtrot"));
        REQUIRE(dict->at(3)->first == Id("zebra"));

        dict->erase("zebra");
        REQUIRE(dict->size() == 3);
        REQUIRE(dict->at(0)->first == Id("bravo"));
        REQUIRE(dict->at(1)->first == Id("echo"));
        REQUIRE(dict->at(2)->first == Id("foxtrot"));
        
        dict->erase("echo");
        REQUIRE(dict->size() == 2);
        REQUIRE(dict->at(0)->first == Id("bravo"));
        REQUIRE(dict->at(1)->first == Id("foxtrot"));
        
        dict->erase("bravo");
        dict->erase("foxtrot");
        REQUIRE(dict->size() == 0);
    }
}

TEST_CASE("Dictionary can have customised sorter")
{
    struct ReverseKeyCmp {
        bool operator () (IdRef a, IdRef b) const {
            return a > b;
        }
    };
    
    typedef ponder::detail::Dictionary<Id, IdRef, int, ReverseKeyCmp> Dict;
    std::unique_ptr<Dict> dict(new Dict());
    
    REQUIRE((dict != nullptr));
    REQUIRE(dict->size() == 0);
    
    dict->insert("bravo", 1);
    dict->insert("alpha", 2);
    dict->insert("zebra", 3);
    dict->insert("foxtrot", 4);
    dict->insert("echo", 5);
    
    SECTION("contains items")
    {
        REQUIRE(dict->size() == 5);
        
        REQUIRE(dict->containsKey("bravo") == true);
        REQUIRE(dict->containsKey("zebra") == true);
        REQUIRE(dict->containsKey("foxtrot") == true);
        REQUIRE(dict->containsKey("bravo") == true);
        REQUIRE(dict->containsKey("echo") == true);
        
        REQUIRE(dict->containsKey("monkey") == false);
    }
    
    SECTION("have values")
    {
        REQUIRE(dict->containsValue(1) == true);
        REQUIRE(dict->containsValue(2) == true);
        REQUIRE(dict->containsValue(3) == true);
        REQUIRE(dict->containsValue(4) == true);
        REQUIRE(dict->containsValue(5) == true);
        
        REQUIRE(dict->containsValue(9) == false);
    }
    
    SECTION("keys are reverse sorted")
    {
        REQUIRE(dict->at(0)->first == Id("zebra"));
        REQUIRE(dict->at(1)->first == Id("foxtrot"));
        REQUIRE(dict->at(2)->first == Id("echo"));
        REQUIRE(dict->at(3)->first == Id("bravo"));
        REQUIRE(dict->at(4)->first == Id("alpha"));
    }
}

