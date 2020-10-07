/**

\page blog_ponder_3 Ponder 3.x Release Notes

\tableofcontents

\section ponder_3_0 3.0 Release Notes

_date: 2018-09-08_

\subsection refactor3 Extensive refactoring of the API.

The API has be refactored internally to address some hacks from V1 when Boost was removed. Some
of the API has been simplified and removed.

\subsection userobj_ref_semantics Changed UserObject reference semantics.

When we construct a UserObject we can use a reference or a pointer. A reference will _copy_ the
object, and a pointer will make reference to the object. This is important because if you modify
the copied object, the change will be lost when the UserObject is destroyed. This may not be the
behaviour that you want.

\subsection common_ancestor_type All types have common ancestor: Type.

See ponder::Type.

\subsection userdata_added Added UserData per Type. (#98)

See ponder::UserData.

\subsection serialisation_refactor Refactored serialisation. Removed ponder-xml. (#43).

- Added RapidJSON for use by JSON serialiser (#43).
- Added RapidXML for use by XML serialiser (#43).

Note, in Ponder 3.0 we only have XML serialisation support.

\subsection  Remove dynamic read/write attribute from properties. (#91)

This was a simplification of the API. Similar functionality might be implemented by wrapping a
property, perhaps with a lambda function

\subsection tagholder_removed Remove TagHolder. (#92)

Tagholder removed. This could now be implemented with Userdata.

\subsection classbyindex_removed Removed ponder::classByIndex().

Use ponder::classIterator() instead.

\subsection cmake_project_rename Cmake project name changed to "ponder" to match namespace (#95).

Following convention.

\subsection composed_prop_getter_removed Removed composed property getter

Use lambdas instead.

\section ponder_3_1 3.1 Release Notes

_date: 2018-01-22_

\note Ponder now requires full C++14 support. We'd like to use the `constexpr` extensions.
    See \ref drop_msvc2015

\subsection obj_reg_by_typeid Change internal class registration to by type instead of by name

Objects are now registered interally using their
[`typeid`](https://en.cppreference.com/w/cpp/language/typeid). They were previously registered via
a string name, however, if we only have a type we could not always get the metaclass information if
the metaclass had been renamed. This was a problem for the object factory. This is now fixed. There
is also a mapping of string name to `typeid` so that you can rename metaclasses, however, you must
always use the new name. This fixes issue #108.

\subsection drop_msvc2015 Drop support for MSVC 2015.

Ponder requires full C++14 support. MSVC 2015 lacks this as it does not support
[N3652](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3652.html): "Relaxing constraints
on constexpr functions constexpr member functions and implicit const". We could work around this
but time is limited and it is just a complication when the latest compilers all support it.

\section ponder_3_2 3.2 Release Notes

_date: 2020-10-10_

\note Ponder now requires full C++17 support.

*/
