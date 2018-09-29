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

*/
