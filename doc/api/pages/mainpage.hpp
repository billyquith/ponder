/**

\mainpage Ponder C++ library documentation

\section overview Overview

\note **3.0 Alpha Version**. API is still in development. Feel free to download and try. This is an
effect to evolve the API and fix past issues. It is a hobby project so please be patient!

\image html ponder1.jpg "Ponder"

Ponder is a C++ library which provides runtime reflection for types. It provides an abstraction for
most of the high-level concepts of C++ like classes, enumerations, properties, functions, and
objects. By wrapping all these concepts into abstract structures, Ponder provides an extra layer of
flexibility to programs, and allow them to fully expose their data structures at runtime.

Many applications can take advantage of Ponder, in order to automate tasks which would
otherwise require a huge amount of work. For example, Ponder can be used to expose and edit
objects' attributes into a graphical user interface. It can also be used to do
automatic binding of C++ classes to script languages such as Python or Lua.
Another possible application would be the serialization of objects to XML, text or
binary formats. Or you can even combine all these examples to provide a powerful
and consistent interface for manipulating your objects outside C++ code.

\section blog News

 - \ref blog_ponder_3
 - \ref blog_ponder_2
 - \ref blog_ponder_1
 - \ref blog_ponder1

\section links Useful links

 - <a href="https://github.com/billyquith/ponder">GitHub project</a>.
 - <a href="http://billyquith.github.io/ponder/docs/">Online documentation</a>.
 - <a href="https://chinbilly.blogspot.com/search/label/ponder">Maintainer blog</a>.

\section example Quick example

Here is a simple example of how to use Ponder:

\snippet simple.cpp simple_example

One way to retrieve the reflected API:

\snippet inspect.cpp inspect_example

\section license_sect License

Ponder is distributed under the terms of the \ref license_page "MIT license".

 */
