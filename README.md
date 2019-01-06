
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

 - [Homepage & documentation](http://billyquith.github.io/ponder/).
 - [GitHub project](https://github.com/billyquith/ponder).
 - [Release notes][rel].
 - [Maintainer blog](https://chinbilly.blogspot.com/search/label/ponder)

### Build

Compilers supported. Requires C++14 support.

- MSVC 2017+
- Xcode 8+
- GCC 4.9+
- Clang 3.4+

Ponder uses CMake to generate project files which are then used to build the project ([Build
docs][build]). E.g. Use `make` to build Ponder (but you can use any build system CMake supports):

```bash
git clone https://github.com/billyquith/ponder.git
cd ponder
mkdir build && cd build
cmake -G "Unix Makefiles" ..
make
```

### History

Ponder is a fork of [CAMP][camp], which has been retired by the original authors. CAMP relies
on Boost, and is pre-C++11. In Ponder, the Boost dependency has been removed, and instead,
C++11 features are used. CAMP was developed by Technogerma Systems France and then by [Tegesoft][tege].

See [CHANGELOG.md](CHANGELOG.md) and [release notes][rel] for more details on Ponder changes.

[rel]: https://billyquith.github.io/ponder/blog_ponder_3.html
[cl]: https://github.com/billyquith/ponder/CHANGELOG.md
[build]: https://billyquith.github.io/ponder/building_ponder.html
[tege]: http://www.tegesoft.com
[camp]: https://github.com/tegesoft/camp
