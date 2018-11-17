/**

\page building_ponder Building Ponder

You can clone or download Ponder via the [Ponder project page][proj]. E.g.

```
git clone https://github.com/billyquith/ponder.git
```

Ponder uses `cmake` to generate project files. As you would normally do, create a build directory
and generate your project files:

```
cd ponder
mkdir build && cd build
cmake -G "Unix Makefiles" ..
make
```

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


[proj]: https://github.com/billyquith/ponder

 */

