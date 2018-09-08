/**

\note These a notes for an older API _for reference_. The details may have changed and
therefore the links and code may not work. Make sure you check the latest docs.

\page blog_ponder_2 Ponder 2.x Release Notes

\tableofcontents

\section ponder_2_0 2.0 Release Notes

_date: 2016-09-18_

\note Ponder has undergone some significant changes in this release. V1 featured the removal of Boost.
Subsequent versions have added more features. V2 is moving towards a different design.

\subsection decoupling Decoupling

The biggest change is the decoupling of the runtime usage of the declared data. In CAMP/V1 the
declared `Function` contains both the data about the function and the call interface. Additionally
it contains controls for the calling, i.e. `callable()`.

```
class Function  // V1
{
public:

    const std::string& name() const;
    std::size_t argCount() const;
    Type returnType() const;
    Type argType(std::size_t index) const;

    bool callable(const UserObject& object) const;
    Value call(const UserObject& object, const Args& args = Args::empty) const;

    ...
};
```

In V2 we are moving towards Function being an immutable data object containing information about
the function, but no interface for calling. This has been moved to the "runtime" namespace.

```
class Function  // V2
{
public:

    IdRef name() const;
    FunctionKind kind() const;
    ValueKind returnType() const;
    policy::ReturnKind returnPolicy() const;
    std::size_t paramCount() const;
    ValueKind paramType(std::size_t index) const;

};
```

In `ponder::runtime` we now have an `ObjectCaller`. This retrieves the requested function and
allows you to call it. So the use of the `Function` data is decoupled from the data. This stops the
`Function` data from being polluted with a particular usage of the data.

```
class ObjectCaller
{
public:
    ObjectCaller(const Function &fn);

    template <typename... A>
    Value call(const UserObject &obj, A... args);
};
```

\subsection renaming_2 Renaming

You might notice from the above that several things have been renamed, e.g. arguments are now
parameters. In the declaration we declare *parameters*, i.e. what the function *receives*.
Arguments are what are *given* to the function when called.

There are also words which have been overloaded, which causes confusion. "Type" can mean a data
type, or a group's classification, e.g. `int` vs integer. Classifications are now "kinds", i.e. a
kind of integer. This might seem pedantic but in an API like this type is used extensively!

    enum class ValueKind
    {
        None,       // No type has been defined yet
        Boolean,    // Boolean type (bool)
        Integer,    // Integer types (unsigned/signed char short int long)
        Real,       // Real types (float, double)
        String,     // String types (char*, ponder::String)
        Enum,       // Enumerated types
        Array,      // Array types (std::vector, std::list, T[])
        User        // User-defined classes
    };

    enum class FunctionKind
    {
        None,               // not a function
        Function,           // a function
        MemberFunction,     // function in a class or struct
        MemberObject,       // object in a class or struct
        FunctionWrapper,    // `std::function<>`
        BindExpression,     // `std::bind()`
        Lambda              // lambda function `[](){}`
    };

Another example is renaming `PONDER_RTTI` to `PONDER_POLYMORPHIC` (issue #36). The macro only needs
to be added to a user API when we want to support polymorphism.

\subsection lua_support Lua binding generator

A new feature is the Lua binding generator (issue #40). Please note this is still in development at
the current time and could change significantly. This can expose a class declaration to Lua so that
the functions can be called and the properties can get and set values. More articles to come.

\subsection static_funcs Added support for binding to static functions (issue #46)

We can now add non-member functions to a class declaration. This can be a class static function, or
a non-class function.

\subsection call_return_policy Added function call return policies (issue #51)

Declared functions that return values can declare return policies. If a function returns, say, a
`const` reference this is ambiguous. It could either be a reference that will be copied to form a
new object, or a reference to an internal object that we want to inspect. We can now use
`ponder::policy::ReturnCopy` and `ponder::policy::ReturnInternalRef` to choose which one to return.
The API will bind the function accordingly.

\subsection parent_child_removed Removed UserObject parent-child. Simplification, unused.

`UserObject`s had a parent-child relationship. I am unsure of the use for this and in complicated
the `UserObject` implementation somewhat so it has been removed.

\section ponder_2_1 2.1 Release Notes

_date: 2016-10-12_

\note The emphasis was on Lua scripting in this release.

\subsection lua_binding_improved Lua: Improved function bindings

In the previous release, Ponder used the runtime metaclass function information at runtime to call
functions, i.e. runtime introspection. This can be a little inefficient though as the runtime
caller requires us to convert arguments to `ponder::Value`s for calling. For example a function
accepting a `const char*` parameter would generate a temporary `std::string` in a `ponder::Value`,
part of `ponder::Args`, which would be discarded after the call. Returns values were also converted
to `ponder::Value`s, which need to be converted back into user types.

This has been made more efficient, and more flexible, by adding a direct Lua binding. Now the Lua
API is used directly from the *compile-time* metadata, so no `ponder::Value` coercion is necessary.

\subsection lua_custom_type_conv Lua: Custom type conversion

This direct binding also means that the Lua function parameters can be customised, so we can
convert user types to Lua types, customised per type if necessary. `ponder_ext::LuaValueReader` is
used to read Lua values, and `ponder_ext::LuaValueWriter` to write back. The `ponder_ext` namespace
is used here to signify that user extensions are allowed.

\subsection lua_table_params Lua: Table parameters

Functions can accept Lua table parameters. `ponder_ext::LuaTable` can be declared as a function
parameter. The binding then expects table to be passed to the declared function and the Lua API can
be used to parse the table.

```
struct TableClass
{
    int a;
    std::string b;

    // Function accepting a Lua table. User writes parser.
    void init(ponder_ext::LuaTable lt)
    {
        assert(lua_istable(lt.L, -1)); // should be guaranteed by conversion

        lua_getfield(lt.L, -1, "a");
        a = luaL_checknumber(lt.L, -1);
        lua_pop(lt.L, 1);

        lua_getfield(lt.L, -1, "b");
        b = luaL_checkstring(lt.L, -1);
        lua_pop(lt.L, 1);
    }
};
```

Declare as normal:

```
ponder::Class::declare<Parsing>()
    .constructor()
    .function("init", &Parsing::init)
    .property("a", &Parsing::a)
    .property("b", &Parsing::b)
    ;
```

From Lua:

    p = Parsing()
    p:init{a=77, b='some text'}               -- table is passed to user function
    assert(p.a == 77 and p.b == 'some text')  -- check values set correctly

\subsection lua_multiple_return Lua: Multiple return values

Declared functions can return multiple values to Lua using tuples. A policy is used to denote this.

```
struct Vec
{
    float   x,y;

    Vec() : x(0), y(0) {}
    Vec(float x_, float y_) : x(x_), y(y_) {}

    std::tuple<float,float> get() const { return std::make_tuple(x,y); }
};
```

Declare:

```
ponder::Class::declare<Vec>()
    .constructor()
    .constructor<float, float>()
    .property("x", &Vec::x)
    .property("y", &Vec::y)
    .function("get", &Vec::get, policy::ReturnMultiple()) // tuple to multiple policy
    ;

ponder::lua::expose<Vec>(L, "Vec2");
```

From Lua:

    t = Vec2(11,22)       -- create vector
    x,y = t:get()         -- get scalars via multiple return values

\subsection lua_enums Lua: Enum support

The Lua state can be populate with registered enums now.

```
enum class Colour { Red, Green, Blue };
```

Declare:

```
ponder::Enum::declare<Colour>()
    .value("red", Colour::Red)
    .value("green", Colour::Green)
    .value("blue", Colour::Blue)
    ;

ponder::lua::expose<Colour>(L, "Colour");
```

From Lua:

    assert(Colour.red == 0)
    assert(Colour.green == 1)
    assert(Colour.blue == 2)

\section ponder_2_2 2.2 Release Notes

\subsection fmt_removed fmt library removed

In this release we removed the dependency on the [fmt][fmt] print formatter library. Although this
is an excellent library it causes problems if a Ponder client application is also using it. See
issue [#59][59]. We could work around these problems, but fmt was not used extensively so it is
easier to remove it and deal with any performance issue directly.

[fmt]: https://github.com/fmtlib/fmt
[59]: https://github.com/billyquith/ponder/issues/59

*/
