// <insert copyright>

/*! \mainpage CAMP C++ library documentation
 *
 * \section overview Overview
 *
 * CAMP is a C++ library which provides runtime reflection for types.
 * It provides an abstraction for most of the high-level concepts of C++:
 *
 * \li Classes
 * \li Enumerations
 * \li Properties
 * \li Functions
 * \li Objects
 * \li Variables
 *
 * By wrapping all these concepts into abstract structures, CAMP provides an extra layer of
 * flexibility to programs, and allow them to fully expose their data structures at runtime.
 *
 * Many applications can take advantage of CAMP, in order to automate tasks which would
 * otherwise require a huge amount of work. For example, CAMP can be used to expose and edit
 * objects' attributes into a graphical user interface. It can also be used to do
 * automatic binding of C++ classes to script languages such as Python or Lua.
 * Another possible application would be the serialization of objects to XML, text or
 * binary formats. Or you can even combine all these examples to provide a powerful
 * and consistent interface for manipulating your objects outside C++ code.
 *
 * \section start Getting started
 *
 * To start using CAMP, please refer to the \ref tutorial.
 *
 * \section example Quick example
 * 
 * Here is a simple example of how to use CAMP:
 *
 * \code
 * #include <string>
 * #include <iostream>
 * #include <camp/class.hpp>
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
 *    {
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
 * // Make the Person type available to CAMP
 * CAMP_TYPE(Person);
 * 
 * 
 * int main()
 * {
 *     // Bind our Person class to CAMP
 *     camp::Class::declare<Person>("Person")
 *         .constructor1<std::string>()
 *         .property("name", &Person::name)
 *         .property("age", &Person::age, &Person::setAge)
 *         .function("speak", &Person::speak);
 * 
 *     // Retrieve it by its name
 *     const camp::Class& metaclass = camp::classByName("Person");
 * 
 *     // Construct a new person named John
 *     Person* john = metaclass.construct<Person>(camp::Args("John"));
 * 
 *     // Print its name
 *     std::string name = metaclass.property("name").get(john);
 *     std::cout << "John's name is: " << name << std::endl;
 * 
 *     // Set its age to 24
 *     metaclass.property("age").set(john, 24);
 * 
 *     // Make John say something
 *     metaclass.function("speak").call(john);
 * 
 *     // Kill John
 *     metaclass.destroy(john);
 * 
 *     return 0;
 * }
 * \endcode
 *
 */
