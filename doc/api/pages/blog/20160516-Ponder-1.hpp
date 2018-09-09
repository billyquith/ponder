/**

\note These a notes for an older API _for reference_. The details may have changed and
therefore the links and code may not work. Make sure you check the latest docs.

\page blog_ponder_1 Ponder 1.x Release Notes

\tableofcontents

\section ponder_1_0 1.0 Release Notes

_date: 2016-01-01 20:46:12_

\par *ponder*: to reflect on

Ponder is a fork of [CAMP][camp], which reached the end of its life. Ponder 1.0 keeps the same API
as CAMP, but makes some changes, like removing the Boost dependency, instead using C++11 features.
Boost is great library, but more recent C++ changes supply a lot of this functionality in the
language and standard libraries.

CAMP was originally released, by [Tegesoft][tegesoft], under the LGPL licence, but this changed to
the MIT licence in version 0.8. Ponder retains the same licence.

    Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
    Contact: Tegesoft Information (contact@tegesoft.com)

    This file is part of the CAMP library.

    The MIT License (MIT)

    Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.


\section ponder_1_1 1.1 Release Notes

_date: 2016-05-16_

\subsection placement_new Placement new creation of objects added

You can now use placement new to specify the location of a class instance. This is useful
if you are managing your own memory.

```
const ponder::Class* metaclass = &ponder::classByType<MyClass>();
const std::size_t sz = metaclass->sizeOf();

char* p = malloc(sizeof(sz))

// note extra argument here:
ponder::UserObject object = metaclass->construct(ponder::Args(), p);

MyClass* instance = object.get<MyClass*>();

assert(instance == reinterpret_cast<MyClass*>(p));

// Be sure to destruct the object as destroy calls delete.
metaclass->destruct(object); // not destroy()
free(p);
```

\note There is now a `Class::destruct()` to pair with placement new.

\subsection undeclare Ability to undeclare Classes and Enums

Classes and Enums can be undeclared. This is useful if you need to unload classes which
you will no longer need. E.g. when unloading dynamic libraries which contain Ponder
registered objects.

Be careful not to auto register the class declarations or they will be added straight back
again!

```
void declare_types()  // call manually to declare
{
    ponder::Class::declare<MyClass>()
        // ... members
        ;
}

void undeclare_types()  // call manually to undeclare
{
    ponder::Class::undeclare(ponder::classByType<MyClass>());
}

PONDER_TYPE(MyClass);  // not PONDER_AUTO_TYPE()
```

\subsection member_iterators Class function and property iterators added

Properties and functions are easier to iterate over:

```
for (auto const& prop : metaclass.propertyIterator()) {}

for (auto const& func : metaclass.functionIterator()) {}
```

\subsection access_by_index UserObject function and property access by index added

In addition to by name and functions and properties in UserObjects can be accessed
by index:

```
MyClass object(3);
ponder::UserObject userObject(object);
userObject.get(0);
userObject.set(1, 7);
```

\subsection internal_dict Dictionaries

Dictionaries are now ordered (sorted alphabetically) internally. The insertion cost will
be more, but this will only happen on declaration. Searching for keys will become a binary
search rather than linear.

\section ponder_1_2 1.2 Release Notes

_date: 2016-07-01_

\subsection prop_chaining Properties support method chaining

Ponder now recognises if properties return the class they are declared in. This allows [method
chaining][mch]. This is also called the "named parameter idiom", see [issue #28][28]. E.g.

```
class MyClass
{
public:
    int Getter() { return i_; };
    MyClass& Setter(int i) { i_ = i; return *this; };  // returns self
private:
    int i_;
};

PONDER_TYPE(MyClass)

void declare_MyClass()
{
    ponder::Class::declare<MyClass>("MyClass")
        .constructor()
        .property("Value", &MyClass::Getter, &MyClass::Setter);
}
```

\subsection userobj_pointers UserObjects can be created from user pointers

UserObjects may now be created from opaque user data pointers. This was previous not possible
because there was no way of casting a user pointer to a Ponder metaclass pointer. See
[issue #30][30]. This can now be done using the metaclass method:

```
UserObject ponder::Class::getUserObjectFromPointer(void *ptr);
```

User data can be stored more efficiently like the following, only creating UserObjects when the
user wishes to interact with the object.

```
struct Entry {
    const ponder::Class& metaClass;
    void* p;
}
```

\subsection remove_composed Remove composed function bindings

Ponder/CAMP previously had composed function bindings. This allowed class declarations to bind to
access functions with a level of indirection. This adds a layer of complexity that can be solved in
more flexible ways, for example, using lambda functions.

Example of a class with internal data that we would like to expose:

```
struct MyClass
{
    struct Inner
    {
        void f14() {}
        void f15() const {}
        int f16() {return 16;}
        void f17(int) const {}
        void f18() {}
        void f19() {}
    };

    Inner inner;
    const Inner& getInner() const {return inner;}

    Inner* innerPtr;
    const Inner* getInnerPtr() const {return innerPtr;}

    std::shared_ptr<Inner> innerSmartPtr;
    const std::shared_ptr<Inner> getInnerSmartPtr() {return innerSmartPtr;}
};
```

Previously:

```
// declare ...
.function("f14", &MyClass::Inner::f14, &MyClass::inner)   // object
.function("f15", &MyClass::Inner::f15, &MyClass::getInner) // getter returning an object
.function("f16", &MyClass::Inner::f16, &MyClass::innerPtr) // raw pointer
.function("f17", &MyClass::Inner::f17, &MyClass::getInnerPtr) // getter returning a raw pointer
.function("f18", &MyClass::Inner::f18, &MyClass::innerSmartPtr)    // smart pointer
.function("f19", &MyClass::Inner::f19, &MyClass::getInnerSmartPtr) // getter returning a smart pointer
;
```

This can now be done using lambda functions. This allows more flexibility, and extra logic may now
be inserted.

```
// object instance
.function("f14",
          std::function<void(MyClass&)>([](MyClass& self){ self.inner.f14(); }))
// reference
.function("f15",
          std::function<void(MyClass&)>([](MyClass& self){ self.getInner().f15(); }))
// raw pointer
.function("f16",
          std::function<void(MyClass&)>([](MyClass& self){ self.innerPtr->f16(); }))
// getter returning a raw pointer
.function("f17",
          std::function<void(MyClass const&)>([](MyClass const& self){
              self.getInnerPtr()->f17(99);
          }))
// smart pointer
.function("f18", std::function<void(MyClass&)>([](MyClass& self){
              self.innerSmartPtr.get()->f18();
          }))
// getter returning a smart pointer
.function("f19", std::function<void(MyClass&)>([](MyClass& self){
              self.getInnerSmartPtr().get()->f19();
          }))
```

See unit test function.cpp for examples.

\subsection enum_type ponder::Type now an enum class

The Ponder value type enumerator was changed to a C++11 enum class. This was done for readability
and to modernise the API. See [issue #26][26].

```
enum class ValueType
{
    None,       // No type has been defined yet
    Boolean,    // Boolean type (bool)
    Integer,    // Integer types (unsigned/signed char short int long)
    Real,       // Real types (float, double)
    String,     // String types (char*, std::string)
    Enum,       // Enumerated types
    Array,      // Array types (std::vector, std::list, T[])
    User        // User-defined classes
};
```

\subsection unittests_readded Unit tests

Some unit tests were re-added that were overlooked in the Ponder refactoring, i.e. function unit
tests.


\section ponder_1_3 1.3 Release Notes

_date: 2016-07-06_

\subsection id_traits Ids/Names configurable using traits

Ponder is a library which is embedded in a library and/or application. The client may have its own
string type, besides `std::string`, so here we allow the customisation of the identifiers
(registered names), and string types.

Pre-Ponder 1.3, `const std::string&` was used as the parameter type for ids. The equivalent trait
is:

```
namespace ponder { namespace detail {

    struct IdTraits
    {
        typedef std::string         string_t;     // std::string
        typedef string_t            id_value_t;   // std::string
        typedef const id_value_t&   id_ref_t;     // const std::string&
    };

}}
```

This, potentially, results in many needless string copies (see [issue #11][11]). This has been replaced with a `string_view` version (see below).

```
namespace ponder { namespace detail {

    struct IdTraits
    {
        typedef std::string string_t;     // std::string
        typedef string_t    id_value_t;   // std::string
        typedef string_view id_ref_t;     // ponder::detail::string_view
    };

}}
```

\subsection string_view_added string_view class added

[`string_view`][strview] is reference to a character range. It is implemented as a couple of
pointers, which are more efficient to pass these around. It can be converted to a string on demand.

This feature is currently experimental and may not be present in all C++ standard libraries hence
an internal version is provided.

Note, like iterators, or pointers into containers, you should not rely on the data always being there, as the string may change or be deleted.

\subsection lambda_added Lambda functions recognised

Previously lambda functions had to be declared via `std::function<>` wrappers. They are now disambiguated and are recognised as separate.

This means function declarations are a little easier to declare, e.g. :-

```
struct MyClass
{
    struct Inner
    {
        void f14() {}
        void f15() const {}
        int f16() {return 16;}
        void f17(int) const {}
        void f18() {}
        void f19() {}
    };

    Inner inner;
    const Inner& getInner() const {return inner;}

    Inner* innerPtr;
    const Inner* getInnerPtr() const {return innerPtr;}

    std::shared_ptr<Inner> innerSmartPtr;
    const std::shared_ptr<Inner> getInnerSmartPtr() {return innerSmartPtr;}
};
```

Previously, an example of how to wrap indirection:

```
// object instance
.function("f14",
          std::function<void(MyClass&)>([](MyClass& self){ self.inner.f14(); }))
// reference
.function("f15",
          std::function<void(MyClass&)>([](MyClass& self){ self.getInner().f15(); }))
// raw pointer
.function("f16",
          std::function<void(MyClass&)>([](MyClass& self){ self.innerPtr->f16(); }))
// getter returning a raw pointer
.function("f17",
          std::function<void(MyClass const&)>([](MyClass const& self){
              self.getInnerPtr()->f17(99);
          }))
// smart pointer
.function("f18", std::function<void(MyClass&)>([](MyClass& self){
              self.innerSmartPtr.get()->f18();
          }))
// getter returning a smart pointer
.function("f19", std::function<void(MyClass&)>([](MyClass& self){
              self.getInnerSmartPtr().get()->f19();
          }))
```

Now:

```
// object instance
.function("f14", [](MyClass& self){ self.inner.f14(); })
// getter returning an object
.function("f15", [](MyClass& self){ self.getInner().f15(); })
// raw pointer
.function("f16", [](MyClass& self){ return self.innerPtr->f16(); })
// getter returning a raw pointer
.function("f17", [](MyClass const& self){ self.getInnerPtr()->f17(99); })
// smart pointer
.function("f18", [](MyClass& self){ self.innerSmartPtr.get()->f18(); })
// getter returning a smart pointer
.function("f19", [](MyClass& self){ self.getInnerSmartPtr().get()->f19(); })
```

\subsection auto_ptr_removed std::auto_ptr removed

`std::auto_ptr<>` is [deprecated in C++11][auto] and will be fully removed in C++17. Support is removed as it generates a lot of warning spam in some compilers, GCC.


[camp]: https://github.com/tegesoft/camp
[tegesoft]: http://www.tegesoft.com/
[28]: https://github.com/billyquith/ponder/issues/28
[mch]: https://en.wikipedia.org/wiki/Method_chaining
[30]: https://github.com/billyquith/ponder/issues/30
[26]: https://github.com/billyquith/ponder/issues/26
[11]: https://github.com/billyquith/ponder/issues/11
[strview]: http://en.cppreference.com/w/cpp/experimental/basic_string_view
[auto]: http://en.cppreference.com/w/cpp/memory/auto_ptr

*/
