/**

\mainpage Ponder C++ library documentation

\image html ponder1.jpg "ponder: to reflect on"

 - **TL;DR** - Expose C++ classes and objects so they can be more easily used as data. Serialisation
    and Lua scripting supported.

\section overview Overview


Ponder is a C++ library which provides runtime reflection for types. It provides an abstraction for
most of the high-level concepts of C++ like classes, enumerations, properties, functions, and
objects. By wrapping all these concepts into abstract structures, Ponder provides an extra layer of
flexibility to programs, and allow them to expose and manipulate their data structures at runtime.

Many applications can take advantage of Ponder, in order to automate tasks which would
otherwise require a huge amount of work. For example, Ponder can be used to expose and edit
objects' attributes into a graphical user interface. It can also be used to do
automatic binding of C++ classes to script languages such as Python or Lua.
Another possible application would be the serialization of objects to XML, text or
binary formats. Or you can even combine all these examples to provide a powerful
and consistent interface for manipulating your objects outside C++ code.

Features:

 - Expose C++ classes so they can be found by name or type.
 - Create and destroy instance of classes at runtime.
 - Get and set properties on member variables.
 - Call member and static functions.
 - Save class instance data as JSON or XML.
 - Automatic Lua binding generation.

\section intro_example Quick example

Here is a simple example of how to use Ponder. First we need a class to expose:

\snippet simple.cpp eg_simple_class

Then we declare what we want to expose to Ponder:

\snippet simple.cpp eg_simple_declare

We can then create and manipulate instances of the class using the API:

\snippet simple.cpp eg_simple_use

\section examples Examples

 - \subpage eg_page_shapes

\section links Useful links

 - <a href="https://github.com/billyquith/ponder">GitHub project</a>.
 - <a href="http://billyquith.github.io/ponder/docs/">Online documentation</a>.
 - <a href="https://chinbilly.blogspot.com/search/label/ponder">Blog articles</a>.

\section blog Articles

 - \subpage blog_ponder_1
 - \subpage blog_ponder_2
 - \subpage blog_ponder_3

\section license License

Ponder is distributed under the terms of the \ref license_page "MIT license".

\ref license_page

 */
