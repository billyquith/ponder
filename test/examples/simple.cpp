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

//#include <ponder/uses/report.hpp>

//! [simple_example]

#include <ponder/classbuilder.hpp>
#include <ponder/uses/runtime.hpp>
#include <iostream>

// An example class for defining a person.
class Person
{
public:

    // Constructor
    Person(const std::string& name)
        : m_name(name)
        , m_age(0)
    {}
    
    // getter for name
    std::string name() const {return m_name;}
    
    // getter/setter for age
    unsigned int age() const {return m_age;}
    void setAge(unsigned int age) {m_age = age;}
    
    // dump info about the person
    void dump()
    {
        std::cout << "Name: " << m_name << ", age: " << m_age << " years old." << std::endl;
    }
    
private:
    std::string m_name;
    unsigned int m_age;
};

// Declare the type to Ponder
PONDER_TYPE(Person)

static void declare()
{
    // Declare the class members to Ponder
    ponder::Class::declare<Person>("Person")
        .constructor<std::string>()
        .property("name", &Person::name)
        .property("age", &Person::age, &Person::setAge)
        .function("dump", &Person::dump)
        ;
}

// An example of how you might use Ponder:
static void use()
{
    // Retrieve the metaclass (containing the member data)
    const ponder::Class& metaclass = ponder::classByType<Person>();
    
    // Use the metaclass to construct a new person named John
    ponder::runtime::ObjectFactory factory(metaclass);
    ponder::UserObject john = factory.create("John");
    
    // Set John's age
    john.set("age", 97);
    
    // Dump John's info
    ponder::runtime::call(metaclass.function("dump"), john);
    
    // Kill John (not really)
    factory.destroy(john);
}

//! [simple_example]

//static void reportAll()
//{
//    ponder::uses::reportAll();
//}

TEST_CASE("simple tests")
{
    SECTION("intro")
    {
        std::printf("------------------------------------------------------------\n");
        declare();
        use();
    }
    
//    SECTION("report")
//    {
//        std::printf("------------------------------------------------------------\n");
//        reportAll();
//    }
}


