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

//! [eg_shapes]

#include <ponder/classbuilder.hpp>
#include <ponder/uses/runtime.hpp>
#include <iostream>

static void declare();

//! [doc_enum_shape]
enum class ShapeType
{
    Circle,
    Rectangle
};
//! [doc_enum_shape]

//! [doc_class_vec2]
struct Vec2
{
    Vec2(float x_, float y_) : x(x_), y(y_) {}
    float x,y;
};
//! [doc_class_vec2]

//! [doc_class_Shape]
class Shape
{
public:
    virtual ~Shape() {}

    ShapeType getType() const { return m_type; }

protected:
    Shape(ShapeType shape) : m_type(shape) {}

private:
    ShapeType m_type;
};
//! [doc_class_Shape]

class Circle : public Shape
{
public:
    Circle(Vec2 pos, float radius)
    :   Shape(ShapeType::Circle)
    ,   m_pos(pos)
    ,   m_radius(radius)
    {}
private:
    Vec2 m_pos;
    float m_radius;
};

/** [doc_PONDER_TYPE] */
PONDER_AUTO_TYPE(ShapeType, declare);   // enum
PONDER_AUTO_TYPE(Vec2, declare);        // class
/** [doc_PONDER_TYPE] */
PONDER_AUTO_TYPE(Shape, declare);
PONDER_AUTO_TYPE(Circle, declare);

static void declare()
{
    ponder::Enum::declare<ShapeType>()
        ;

    ponder::Class::declare<Vec2>()
        .constructor<float,float>()
        .property("x", &Vec2::x)                // expose member variables
        .property("y", &Vec2::y)
        ;

    ponder::Class::declare<Shape>()
        .property("type", &Shape::getType)      // expose getter member function
        ;

    ponder::Class::declare<Circle>()
        .base<Shape>()
        //.constructor<Vec2,float>()
        .property("type", &Shape::getType)      // expose getter member function
        ;
}

//! [eg_shapes]

TEST_CASE("shapes")
{

}


