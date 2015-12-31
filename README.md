
Ponder
------

[![Build Status](https://travis-ci.org/billyquith/camp.svg)](https://travis-ci.org/billyquith/camp)

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

### History

Ponder is a multi-purpose reflection library which is a fork of [CAMP][2], which has been
retired by the original authors. CAMP relies on Boost, and is pre-C++11. In Ponder,
the Boost dependency has been removed (apart from testing) and instead C++11 features
are used.

CAMP was developed by Technogerma Systems France and then by [Tegesoft][1]
(where the dev team has moved) .


[1]: http://www.tegesoft.com
[2]: https://github.com/tegesoft/camp

