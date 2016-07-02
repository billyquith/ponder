
#include "test.hpp"
#include <iostream>
#include <ponder/string_view.hpp>

TEST_CASE("String views make std::string more efficient")
{
//    SECTION("substring")
//    {
//        std::string str("test2");
//        std_backport::string_view view(str);
//
//        REQUIRE(view == string_view(str));
//    }

    SECTION("substring")
    {
        const std_backport::string_view view("test");
        std::string str("test2");
        const std_backport::string_view view2(str);
        
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
        std_backport::string_view v(s);
        
        REQUIRE(s.find("is") == v.find("is"));
        REQUIRE(s.find("is", 4) == v.find("is", 4));
        REQUIRE(s.find("s", 4) == v.find("s", 4));
        REQUIRE(s.find("s") == v.find("s"));
        REQUIRE(s.find('q') == v.find('q'));
        REQUIRE(s.find('q') == v.find('q'));
        
        IS_TRUE(v.find('q') == std_backport::string_view::npos);
    }
    
    SECTION("reverse find")
    {
        std::string s("This is a string");
        std_backport::string_view v(s);
        
        
        REQUIRE(s.rfind("is") == v.rfind("is"));
        REQUIRE(s.rfind("is", 4) == v.rfind("is", 4));
        REQUIRE(s.rfind("s", 4) == v.rfind("s", 4));
        REQUIRE(s.rfind("s") == v.rfind("s"));
        REQUIRE(s.rfind('q') == v.rfind('q'));
        REQUIRE(s.rfind('q') == v.rfind('q'));
        
        REQUIRE(s.rfind("is") == 5);
        
        REQUIRE(s.rfind("is", 4) == 2);
        
        REQUIRE(s.rfind("s") == 10);
        
        IS_TRUE(s.rfind('q') == std_backport::string_view::npos);
    }
    
    SECTION("first of")
    {
        std::string s("This is a string");
        std_backport::string_view v(s);
        
        REQUIRE(s.find_first_of("is") == v.find_first_of("is"));
        REQUIRE(s.find_first_of("is", 4) == v.find_first_of("is", 4));
        REQUIRE(s.find_first_of("s", 4) == v.find_first_of("s", 4));
        REQUIRE(s.find_first_of("s") == v.find_first_of("s"));
        REQUIRE(s.find_first_of('q') == v.find_first_of('q'));
        REQUIRE(s.find_first_of('q') == v.find_first_of('q'));
        
        IS_TRUE(v.find_first_of('q') == std_backport::string_view::npos);
    }
    
    SECTION("last of")
    {
        std::string s("This is a string");
        std_backport::string_view v(s);
        
        REQUIRE(s.find_last_of("is") == v.find_last_of("is"));
        REQUIRE(s.find_last_of("is", 4) == v.find_last_of("is", 4));
        REQUIRE(s.find_last_of("s", 4) == v.find_last_of("s", 4));
        REQUIRE(s.find_last_of("s") == v.find_last_of("s"));
        REQUIRE(s.find_last_of('q') == v.find_last_of('q'));
        REQUIRE(s.find_last_of('q') == v.find_last_of('q'));
        
        IS_TRUE(v.find_last_of('q') == std_backport::string_view::npos);
    }
    
    SECTION("first not of")
    {
        std::string s("This is a string");
        std_backport::string_view v(s);
        
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
        std_backport::string_view v(s);
        REQUIRE(s.find_last_not_of("is") == v.find_last_not_of("is"));
        REQUIRE(s.find_last_not_of("is", 4) == v.find_last_not_of("is", 4));
        REQUIRE(s.find_last_not_of("s", 4) == v.find_last_not_of("s", 4));
        REQUIRE(s.find_last_not_of("s") == v.find_last_not_of("s"));
        REQUIRE(s.find_last_not_of('q') == v.find_last_not_of('q'));
        REQUIRE(s.find_last_not_of('q') == v.find_last_not_of('q'));
        
        REQUIRE(v.find_last_not_of('q') == v.size()-1);
    }
}



