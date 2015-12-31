
Ponder
------

[![Build Status](https://travis-ci.org/billyquith/ponder.svg)](https://travis-ci.org/billyquith/ponder)

### About

Ponder is a C++ multi-purpose reflection library. It provides an abstraction for most
of the high-level concepts of C++:

- Classes
- Enumerations
- Functions
- Objects
- Properties
- Variables

By wrapping all these concepts into abstract structures, Ponder provides an extra layer of
flexibility to programs, and allow them to fully expose their data structures at runtime.

Many applications can take advantage of Ponder, in order to automate tasks which would
otherwise require a huge amount of work. For example, Ponder can be used to expose and edit
objects' attributes into a graphical user interface. It can also be used to do
automatic binding of C++ classes to script languages such as Python or Lua.
Another possible application would be the serialization of objects to XML, text or
binary formats. Or you can even combine all these examples to provide a powerful
and consistent interface for manipulating your objects outside C++ code.

### Example

The following example exposes our chosen view of a C++ class:

```cpp
class Person  // A C++ class
{
public:
    Person(const std::string& name) : m_name(name), m_age(0) {}

    std::string name() const {return m_name;}

    unsigned int age() const {return m_age;}
    void setAge(unsigned int age) {m_age = age;}

    void speak() {
        std::cout << "Hi! My name is " << m_name << " and I'm " << m_age << " years old." << std::endl;
    }
    
private:
    std::string m_name;
    unsigned int m_age;
};

PONDER_TYPE(Person) // declare type

void declare() // register details (once)
{
    ponder::Class::declare<Person>("Person")
        .constructor<std::string>()
        .property("name", &Person::name)
        .property("age", &Person::age, &Person::setAge)
        .function("speak", &Person::speak)
        ;
}

void use()
{
    // Retrieve the metaclass by its name
    const ponder::Class& metaclass = ponder::classByName("Person");
    
    // Use the metaclass to construct a new person named John
    ponder::UserObject john = metaclass.construct(ponder::Args("John"));
    
    // Print its name
    std::cout << "John's name is: " << john.get("name") << std::endl;
    
    // Set its age to 24
    john.set("age", 24);
    
    // Make John say something
    john.call("speak");
    
    // Kill John
    metaclass.destroy(john);
}
```

### History

Ponder is a fork of [CAMP][2], which has been retired by the original authors. CAMP relies
on Boost, and is pre-C++11. In Ponder, the Boost dependency has been removed
(apart from testing) and, instead, C++11 features are used. CAMP was developed by
Technogerma Systems France and then by [Tegesoft][1] (where the dev team has moved).


[1]: http://www.tegesoft.com
[2]: https://github.com/tegesoft/camp

