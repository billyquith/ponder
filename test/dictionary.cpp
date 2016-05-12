/****************************************************************************
 **
 ** This file is part of the Ponder library, formerly CAMP.
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

#include <ponder/detail/dictionary.hpp>
#include "catch.hpp"
#include <string.h>


//-----------------------------------------------------------------------------
//                         Tests for Ponder detail::dictionary
//-----------------------------------------------------------------------------

// Sanity check: make sure compiler supports features we need.
TEST_CASE("Ponder has a dictionary")
{
    struct Cmp {
        bool operator () (const std::string& a, const std::string& b) const {
            return a < b;
        }
    };
    
    typedef ponder::detail::Dictionary<std::string, int, Cmp> Dict;
    std::unique_ptr<Dict> dict(new Dict());
    
    REQUIRE(dict != nullptr);
    
    SECTION("functions")
    {
    }
}

