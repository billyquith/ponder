
Ponder
------

Linux & OSX: [![Build Status](https://travis-ci.org/billyquith/ponder.svg?branch=master)](https://travis-ci.org/billyquith/ponder) - 
Windows: [![Build status](https://ci.appveyor.com/api/projects/status/spskn9y93e8osve2/branch/master?svg=true)](https://ci.appveyor.com/project/billyquith/ponder/branch/master)

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

### Links

 - [Ponder website](http://billyquith.github.io/ponder/).
 - [Online documentation](http://billyquith.github.io/ponder/docs/).
 - [Project blog](http://billyquith.github.io/ponder/blog/).
 - [GitHub project](https://github.com/billyquith/ponder).

### Build

Ponder uses CMake to generate project files. E.g. Use Ninja to build Ponder:

```bash
git clone https://github.com/billyquith/ponder.git
cd ponder
mkdir build && cd build
cmake -G Ninja ..
ninja
```

### History

Ponder is a fork of [CAMP][2], which has been retired by the original authors. CAMP relies
on Boost, and is pre-C++11. In Ponder, the Boost dependency has been removed, and instead, 
C++11 features are used. CAMP was developed by Technogerma Systems France and then by [Tegesoft][1].


[1]: http://www.tegesoft.com
[2]: https://github.com/tegesoft/camp
