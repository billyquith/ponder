/*! \mainpage Ponder C++ library documentation
 *
 * \htmlonly
 * <div class="logo">
 * \endhtmlonly
 * \image html logo_camp.png
 * \htmlonly
 * </div>
 * \endhtmlonly
 *
 * \section overview Overview
 *
 * Ponder is a C++ library which provides runtime reflection for types.
 * It provides an abstraction for most of the high-level concepts of C++:
 *
 * \li Classes
 * \li Enumerations
 * \li Properties
 * \li Functions
 * \li Objects
 * \li Variables
 *
 * By wrapping all these concepts into abstract structures, Ponder provides an extra layer of
 * flexibility to programs, and allow them to fully expose their data structures at runtime.
 *
 * Many applications can take advantage of Ponder, in order to automate tasks which would
 * otherwise require a huge amount of work. For example, Ponder can be used to expose and edit
 * objects' attributes into a graphical user interface. It can also be used to do
 * automatic binding of C++ classes to script languages such as Python or Lua.
 * Another possible application would be the serialization of objects to XML, text or
 * binary formats. Or you can even combine all these examples to provide a powerful
 * and consistent interface for manipulating your objects outside C++ code.
 *
 * \section example Quick example
 * 
 * Here is a simple example of how to use Ponder:
 *
 * \code
 * #include <ponder/pondertype.hpp>
 * #include <ponder/classbuilder.hpp>
 * #include <string>
 * #include <iostream>
 * 
 * // Let's define a class for handling persons
 * class Person
 * {
 * public:
 * 
 *     // Construct a person from its name
 *     Person(const std::string& name) : m_name(name), m_age(0)
 *     {
 *     }
 * 
 *     // Retrieve the name of a person
 *     std::string name() const
 *     {
 *         return m_name;
 *     }
 * 
 *     // Retrieve the age of a person
 *     unsigned int age() const
 *     {
 *         return m_age;
 *     }
 * 
 *     // Set the age of a person
 *     void setAge(unsigned int age)
 *     {
 *         m_age = age;
 *     }
 * 
 *     // Make a person speak (tell about its name and age)
 *     void speak()
 *     {
 *         std::cout << "Hi! My name is " << m_name << " and I'm " << m_age << " years old." << std::endl;
 *     }
 * 
 * private:
 * 
 *     std::string m_name;
 *     unsigned int m_age;
 * };
 * 
 * // Make the Person type available to Ponder
 * PONDER_TYPE(Person)
 * 
 * 
 * int main()
 * {
 *     // Bind our Person class to Ponder
 *     ponder::Class::declare<Person>("Person")
 *         .constructor<std::string>()
 *         .property("name", &Person::name)
 *         .property("age", &Person::age, &Person::setAge)
 *         .function("speak", &Person::speak);
 * 
 *     // Retrieve the metaclass by its name
 *     const ponder::Class& metaclass = ponder::classByName("Person");
 * 
 *     // Use the metaclass to construct a new person named John
 *     ponder::UserObject john = metaclass.construct(ponder::Args("John"));
 *
 *     // Print its name
 *     std::cout << "John's name is: " << john.get("name") << std::endl;
 *
 *     // Set its age to 24
 *     john.set("age", 24);
 *
 *     // Make John say something
 *     john.call("speak");
 * 
 *     // Kill John
 *     metaclass.destroy(john);
 * 
 *     return 0;
 * }
 * \endcode
 *
 * \section license License
 *
 * Ponder is distributed under the terms of the MIT license.
 *
 * \verbatim
 * The MIT License (MIT)
 *
 * Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
 * Contact: Tegesoft Information (contact@tegesoft.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * \endverbatim
 *
 */
