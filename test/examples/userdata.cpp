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
#include <ponder/classbuilder.hpp>
#include <iostream>

//! [userdata_example]

class Vec2D
{
public:
    Vec2D(float x_, float y_) : x(x_), y(y_) {}
    Vec2D operator+(const Vec2D& o) const { return Vec2D(x+o.x, y+o.y); }
    float x,y;
};

PONDER_TYPE(Vec2D)

static void declare()
{
    using namespace ponder;
    
    Class::declare<Vec2D>("Vec2D")
            ( UserData("help", "2D vector") )
        .constructor<float,float>()
        .property("x", &Vec2D::x)( UserData("default", 0.f) )
        .property("y", &Vec2D::y)( UserData("default", 0.f) )
        .function("add", &Vec2D::operator+)
            (
                UserData("help", "add vector to this and return result"),
                UserData("eg", "result = a + b")
            )
        ;
}

static void test()
{
    //! [get_user_data]    
    auto const& cls = ponder::classByType<Vec2D>();
    
    // print help about the class
    const ponder::Value* help = ponder::userDataStore()->getValue(cls, "help");
    std::cout << "Class " << cls.name() << ": ";
    std::cout << (help ? help->to<ponder::Id>() : "no help") << std::endl;
    
    // print functions and any help
    for (auto const& it : cls.functionIterator())
    {
        auto const& fn = it.second;
        std::cout << "\t" << fn->name() << ": ";
        help = ponder::userDataStore()->getValue(*fn, "help");
        if (help)
            std::cout << (help ? help->to<ponder::Id>() : "no help") << std::endl;
    }    
    //! [get_user_data]
}

//! [userdata_example]

TEST_CASE("test types")
{
    SECTION("intro")
    {
        declare();
        test();
    }
}


