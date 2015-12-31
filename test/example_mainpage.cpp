/****************************************************************************
 **
 ** This file is part of the CAMP library.
 **
 ** The MIT License (MIT)
 **
 ** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
 ** Contact: Tegesoft Information (contact@tegesoft.com)
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

#include <ponder/pondertype.hpp>
#include <ponder/classbuilder.hpp>
#include <string>
#include <iostream>

#include <boost/test/unit_test.hpp>

// Let's define a class for handling persons
class Person
{
public:
    // Construct a person from its name
    Person(const std::string& name) : m_name(name), m_age(0), m_spoke(false)
    {
    }
    // Retrieve the name of a person
    std::string name() const
    {
        return m_name;
    }
    // Retrieve the age of a person
    unsigned int age() const
    {
        return m_age;
    }
    // Set the age of a person
    void setAge(unsigned int age)
    {
        m_age = age;
    }
    // Make a person speak (tell about its name and age)
    void speak()
    {
        //std::cout << "Hi! My name is " << m_name << " and I'm " << m_age << " years old." << std::endl;
        m_spoke = true;
    }
    bool spoke() const
    {
        return m_spoke;
    }
    
private:
    std::string m_name;
    unsigned int m_age;
    bool m_spoke;
};
// Make the Person type available to CAMP
PONDER_TYPE(Person)

BOOST_AUTO_TEST_SUITE(EXAMPLE)

BOOST_AUTO_TEST_CASE(mainpage)
{
    // Bind our Person class to CAMP
    ponder::Class::declare<Person>("Person")
    .constructor<std::string>()
    .property("name", &Person::name)
    .property("age", &Person::age, &Person::setAge)
    .function("speak", &Person::speak)
    ;
    
    // Retrieve the metaclass by its name
    const ponder::Class& metaclass = ponder::classByName("Person");
    
    // Use the metaclass to construct a new person named John
    ponder::UserObject john = metaclass.construct(ponder::Args("John"));
    
    // Print its name
    //std::cout << "John's name is: " << john.get("name") << std::endl;
    BOOST_CHECK(john.get("name") == std::string("John"));
    
    // Set its age to 24
    john.set("age", 24);
    
    // Make John say something
    john.call("speak");
    BOOST_CHECK_EQUAL(john.get<Person>().spoke(), true);
    
    // Kill John
    metaclass.destroy(john);
}

BOOST_AUTO_TEST_SUITE_END()
