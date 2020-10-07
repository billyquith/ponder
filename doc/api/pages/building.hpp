/**

\page building_ponder Building Ponder

These are some notes on how to build Ponder. It is assumed that you have some knowledge of
[CMake][cmake]. If not, perhaps look at the [tutorial][tutorial], or search for tutorial resources,
of which there are many.

## Get Ponder

You can clone or download Ponder via the [Ponder project page][proj]. E.g.

```
git clone https://github.com/billyquith/ponder.git
```

## Compiler Notes

Ponder requires a C++ compiler that handles a minimum of **C++17**. These are some notes
on compilers used in testing:

- Compiler [C++ compatibility][compat] notes.
- Clang has full support for C++17 since version 4.
- GCC has good C++17 support from version 7 onwards.
- Apple Xcode:
    - Apple Clang is a modified version of Clang with different version numbers.
    - [Version information](https://trac.macports.org/wiki/XcodeVersionInfo)
    - Versions since Xcode 9.2 should work.
- Microsoft MSVC 2017 & 2019 32/64 bit - supported
    - MSVC started to have full C++17 support in 2019. Please prefer this version.
    - Best to use the most recent patched version.

## Generate Build Files

Ponder uses `cmake` to generate project files. As you would normally do, create a build directory
and generate your project files:

```
cd ponder
mkdir build && cd build
cmake -G "Unix Makefiles" ..
make
```

### Build Options

There are various options you can pass to cmake to turn on optional features. These are processed
in `cmake/Config.cmake`.

Features:

- `USES_LUA:BOOL` - turns Lua binding support on. See \ref ponder::lua.

Tests:

- `BUILD_TEST` - includes the unit tests. You might use this for reference.
- `BUILD_TEST_LUA:BOOL` - turns Lua testing on. You might want to build this to ensure your project
  is set up correctly if `USES_LUA` is on.
- `BUILD_TEST_EXAMPLES:BOOL` - includes the examples used in the documentation.

Options:

- `BUILD_SHARED_LIBS:BOOL` - builds Ponder as a shared library.
    - Only really applicable to Windows for making DLLs.
- `CMAKE_BUILD_TYPE:STRING` - sets the build configuration. This defaults to "RelWithDebInfo".

E.g.

```
cmake .. \
    -G "Unix Makefiles" \
    -D BUILD_TEST=TRUE      # include unit tests \
    -D USES_LUA=TRUE        # turn on Lua support \
    -D BUILD_TEST_LUA=TRUE  # include Lua tests
```

## Using Ponder

You can include and link to Ponder as you would a normal library. Please note if you install Ponder
there is a `FindPonder.cmake` module in the `cmake` directory which allows you to find and
configure Ponder from your client project.

[cmake]: https://cmake.org/
[tutorial]: https://cmake.org/cmake-tutorial/
[proj]: https://github.com/billyquith/ponder
[compat]: https://en.cppreference.com/w/cpp/compiler_support#cpp14

 */

  https://cmake.org/cmake-tutorial/
