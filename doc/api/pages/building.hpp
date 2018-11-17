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

There are various options you can pass to cmake to turn on optional features:

- `USES_LUA:BOOL` turns Lua binding support on. See \ref ponder::lua.

E.g.

```
cmake -G "Unix Makefiles" -D USES_LUA=ON ..  # turn on Lua support
```


[proj]: https://github.com/billyquith/ponder

 */

