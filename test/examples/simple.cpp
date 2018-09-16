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

//! [eg_simple]

#include <ponder/classbuilder.hpp>
#include <ponder/uses/runtime.hpp>
#include <iostream>

//! [eg_simple_class]
class Person
{
public:
     // constructor
    Person(const std::string& name)
        : m_name(name)
    {}

    // accessors for private members
    std::string name() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }

    // public members
    float height;
    unsigned int shoeSize;

    // member function
    bool hasBigFeet() const { return shoeSize > 10; } // U.K.!

private:
    std::string m_name;
};
//! [eg_simple_class]

//! [eg_simple_declare]
PONDER_TYPE(Person)     // declare the type to Ponder

static void declare()   // declare the class members to Ponder
{
    ponder::Class::declare<Person>("Person")
        .constructor<std::string>()
        .property("name", &Person::name, &Person::setName)
        .property("height", &Person::height)
        .property("shoeSize", &Person::shoeSize)
        .function("hasBigFeet", &Person::hasBigFeet)
        ;
}
//! [eg_simple_declare]

//! [eg_simple_use]
// An example of how you might use Ponder:
static void use()
{
    //! [eg_simple_metaclass]
    // retrieve the metaclass (containing the member data)
    const ponder::Class& metaclass = ponder::classByType<Person>();
    //! [eg_simple_metaclass]

    //! [eg_simple_create]
    // construct a new person
    ponder::UserObject person = ponder::runtime::create(metaclass, "Bozo");
    //! [eg_simple_create]

    // set attributes
    person.set("height", 1.62f);
    person.set("shoeSize", 28);

    // retrieve a function we would like to call
    const auto& func = metaclass.function("hasBigFeet");
    
    // call the function and get the result
    const bool bigFeet = ponder::runtime::call(func, person).to<bool>();

    // nasty
    //! [eg_simple_destroy]
    ponder::runtime::destroy(person);
    //! [eg_simple_destroy]
}
//! [eg_simple_use]

//! [eg_simple]

TEST_CASE("simple tests")
{
    SECTION("intro")
    {
        std::printf("------------------------------------------------------------\n");
        declare();
        use();
    }
}


