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
#include <iostream>
#include <ponder/detail/string_view.hpp>

using ponder::detail::string_view;

// About string types here:
//
// ponder::String can be redefined to a user type. For now we'll pair ponder::string_view
// with std::string. It may be the case that the user string type has its own string_view.

TEST_CASE("String views make std::string more efficient")
{
    SECTION("comparison")
    {
        std::string str("test2");
        string_view view(str);
        REQUIRE(view.compare(string_view(str)) == 0);
        REQUIRE(view == string_view(str));
        REQUIRE(view == str);
        
        str = "changed";  // view now out of date
        REQUIRE(view.compare(string_view(str)) != 0);
        REQUIRE(view != str);
        REQUIRE(str == "changed");
        
        view = str;  // updated
        REQUIRE(str == "changed");
        REQUIRE(view == "changed");
        REQUIRE(view == str);
    }

    SECTION("substring")
    {
        const string_view view("test");
        std::string str("test2");
        const string_view view2(str);
        
        REQUIRE(view.substr(1) == "est");
        REQUIRE(view.substr(1, 1) == "e");
        REQUIRE(view2.substr(1) == "est2");
        
        REQUIRE(view.compare(view2) < 0);
        REQUIRE(view2.compare(view) > 0);
        REQUIRE(view.compare(view2.substr(0, 4)) == 0);
        
        REQUIRE(view2.substr(1, 3) == "est");
    }
    
    SECTION("find")
    {
        std::string s("This is a string");
        string_view v(s);
        
        REQUIRE(s.find("is") == v.find("is"));
        REQUIRE(s.find("is", 4) == v.find("is", 4));
        REQUIRE(s.find("s", 4) == v.find("s", 4));
        REQUIRE(s.find("s") == v.find("s"));
        REQUIRE(s.find('q') == v.find('q'));
        REQUIRE(s.find('q') == v.find('q'));
        
        IS_TRUE(v.find('q') == string_view::npos);
    }
    
    SECTION("reverse find")
    {
        std::string s("This is a string");
        string_view v(s);
        
        
        REQUIRE(s.rfind("is") == v.rfind("is"));
        REQUIRE(s.rfind("is", 4) == v.rfind("is", 4));
        REQUIRE(s.rfind("s", 4) == v.rfind("s", 4));
        REQUIRE(s.rfind("s") == v.rfind("s"));
        REQUIRE(s.rfind('q') == v.rfind('q'));
        REQUIRE(s.rfind('q') == v.rfind('q'));
        
        REQUIRE(s.rfind("is") == 5);
        
        REQUIRE(s.rfind("is", 4) == 2);
        
        REQUIRE(s.rfind("s") == 10);
        
        IS_TRUE(s.rfind('q') == string_view::npos);
    }
    
    SECTION("first of")
    {
        std::string s("This is a string");
        string_view v(s);
        
        REQUIRE(s.find_first_of("is") == v.find_first_of("is"));
        REQUIRE(s.find_first_of("is", 4) == v.find_first_of("is", 4));
        REQUIRE(s.find_first_of("s", 4) == v.find_first_of("s", 4));
        REQUIRE(s.find_first_of("s") == v.find_first_of("s"));
        REQUIRE(s.find_first_of('q') == v.find_first_of('q'));
        REQUIRE(s.find_first_of('q') == v.find_first_of('q'));
        
        IS_TRUE(v.find_first_of('q') == string_view::npos);
    }
    
    SECTION("last of")
    {
        std::string s("This is a string");
        string_view v(s);
        
        REQUIRE(s.find_last_of("is") == v.find_last_of("is"));
        REQUIRE(s.find_last_of("is", 4) == v.find_last_of("is", 4));
        REQUIRE(s.find_last_of("s", 4) == v.find_last_of("s", 4));
        REQUIRE(s.find_last_of("s") == v.find_last_of("s"));
        REQUIRE(s.find_last_of('q') == v.find_last_of('q'));
        REQUIRE(s.find_last_of('q') == v.find_last_of('q'));
        
        IS_TRUE(v.find_last_of('q') == string_view::npos);
    }
    
    SECTION("first not of")
    {
        std::string s("This is a string");
        string_view v(s);
        
        REQUIRE(s.find_first_not_of("is") == v.find_first_not_of("is"));
        REQUIRE(s.find_first_not_of("is", 4) == v.find_first_not_of("is", 4));
        REQUIRE(s.find_first_not_of("s", 4) == v.find_first_not_of("s", 4));
        REQUIRE(s.find_first_not_of("s") == v.find_first_not_of("s"));
        REQUIRE(s.find_first_not_of('q') == v.find_first_not_of('q'));
        REQUIRE(s.find_first_not_of('q') == v.find_first_not_of('q'));
        
        REQUIRE(v.find_first_not_of('q') == 0);
    }
    
    SECTION("last not of")
    {
        std::string s("This is a string");
        string_view v(s);
        REQUIRE(s.find_last_not_of("is") == v.find_last_not_of("is"));
        REQUIRE(s.find_last_not_of("is", 4) == v.find_last_not_of("is", 4));
        REQUIRE(s.find_last_not_of("s", 4) == v.find_last_not_of("s", 4));
        REQUIRE(s.find_last_not_of("s") == v.find_last_not_of("s"));
        REQUIRE(s.find_last_not_of('q') == v.find_last_not_of('q'));
        REQUIRE(s.find_last_not_of('q') == v.find_last_not_of('q'));
        
        REQUIRE(v.find_last_not_of('q') == v.size()-1);
    }

    SECTION("conversion")
    {
        string_view sv1("a string view");
        std::string s1(sv1);
        std::string s2 = sv1;   // this is not allowed if *explicit* operator std::string()
    }
}



