
Ponder
------

Linux & OSX: [![Build Status](https://travis-ci.org/billyquith/ponder.svg?branch=master)](https://travis-ci.org/billyquith/ponder) -
Windows: [![Build status](https://ci.appveyor.com/api/projects/status/spskn9y93e8osve2/branch/master?svg=true)](https://ci.appveyor.com/project/billyquith/ponder/branch/master)

### New: Version 3

V1 replaced Boost with C++11. V2 added Lua bindings. V3 refactored to remove some warts,
ease future development, and re-add serialisation. API is evolving.

### About

Ponder is a C++ multi-purpose reflection library. It provides an abstraction for most
of the high-level concepts of C++: classes, enumerations, functions, properties.

C++1x supplies better support for compile-time reflection. Runtime reflection involves
baking compile-time reflection into the executable so this can be used at runtime. Ponder
presents a simple API for baking your objects so that they can be used at runtime. The baking
can also, optionally, userdata and support for Lua scripting.

Features:

- API to expose C++ objects.
- Runtime API to create objects, call functions, and read and modify properties.
- Automatic Lua binding generation.

### Links

 - [GitHub project](https://github.com/billyquith/ponder).
 - [Online documentation](http://billyquith.github.io/ponder/docs/).
 - [Maintainer blog](https://chinbilly.blogspot.com/search/label/ponder)

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
