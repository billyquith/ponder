---
layout: page
title: About
permalink: /about/
---

Ponder is a C++11 reflection library. It allows you to describe your C++ API
and then inspect this at runtime. This is useful for data-driven applications,
script binding, automatic editing, etc.

### Simple example

A class to expose:

```cpp
class Adder
{
public: 
    Adder() : total_(0)     {}
    void add(int value)     { total_ += value; }
    int getTotal() const    { return total_; }
private:
    int total_;    
};
```

How we would like to expose the object:

```cpp
ponder::Class::declare<Adder>("Adder")
    .constructor<>()
    .function("add",   &Adder::add)
    .property("total", &Adder::getTotal);
```

Now we can make *runtime* instances of the object and manipulate them:

```cpp
auto& metaclass = ponder::classByName("Person"); // find the class

auto adder = metaclass.construct(); // create an instance

adder.call("add", 7); // call a function
adder.call("add", 5);

int total = adder.get("total"); // get a property -> 12 
```
