/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2018 Nick Trout.
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
** 
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
** 
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
**
****************************************************************************/

#pragma once
#ifndef PONDER_VALUEMAPPER_HPP
#define PONDER_VALUEMAPPER_HPP

#include <ponder/config.hpp>
#include <ponder/enum.hpp>
#include <ponder/enumobject.hpp>
#include <ponder/userobject.hpp>
#include <ponder/arraymapper.hpp>
#include <ponder/errors.hpp>
#include <ponder/detail/util.hpp>

/**
 * \namespace ponder_ext
 * \brief Ponder user extendable namespace.
 * 
 * C++ only allows specialisation within the same namespace as the type you 
 * are extending. To avoid clashes with the ponder namespace we use `ponder_ext` 
 * to provide a safe place to place custom specialisations.
 * \see ValueMapper.
 */

namespace ponder_ext
{
    template <typename T, typename C = void> struct ValueMapper;
}

namespace ponder {

/**
 * \brief Map a C++ type to a Ponder type
 *
 * This function simply returns the mapping defined by ValueMapper (i.e. \c ValueMapper<T>::type).
 *
 * \return Ponder type which T maps to
 */
template <typename T>
inline ValueKind mapType()
{
    return ponder_ext::ValueMapper<typename detail::RawType<T>::Type>::kind;
}

} // namespace ponder


namespace ponder_ext {
    
/**
 * \class ValueMapper
 *
 * \brief Template providing a mapping between C++ types/values and Ponder types/values
 *
 * ValueMapper<T> defines a mapping to and from type T. It defines three things in 
 * order to make T fully compliant with the system:
 *
 * \li The abstract Ponder type that T is mapped to
 * \li A function to convert from T *to* the mapped Ponder type
 * \li A function to convert *from* all supported Ponder types to T
 *
 * Pseudo-code:
 * \code
 * template <> struct ValueMapper<TypeSpecialised>
 * {
 *     static PonderValueKind to(ValueKind value)      { return convertToPonderType(value); }
 *     static TypeSpecialised from(PonderValueKind pv) { return convertPonderToType(pc); }
 * };
 * \endcode
 *
 * ValueMapper is specialized for every supported type, and can be specialized
 * for any of your own types in order to extend the system.
 *
 * Here is an example of mapping for a custom string class:
 *
 * \code
 * namespace ponder_ext
 * {
 *     template <>
 *     struct ValueMapper<MyStringClass>
 *     {
 *         // The corresponding Ponder type is "string"
 *         static const ponder::ValueKind kind = ponder::ValueKind::String;
 *  
 *         // Convert from MyStringClass to ponder::String
 *         static ponder::String to(const MyStringClass& source)
 *         {
 *             return source.to_std_string();
 *         }
 * 
 *         // Convert from any type to MyStringClass
 *         // Be smart, just reuse ValueMapper<ponder::String> :)
 *         template <typename T>
 *         static MyStringClass from(const T& source)
 *         {
 *             return MyStringClass(ValueMapper<ponder::String>::from(source));
 *         }
 *     };
 * }
 * \endcode
 *
 * Generic version of ValueMapper -- T doesn't match with any specialization
 * and is thus treated as a user object    
 */
    
/** \cond NoDocumentation */
    
template <typename T, typename C>
struct ValueMapper
{
    static const ponder::ValueKind kind = ponder::ValueKind::User;
    static ponder::UserObject to(const T& source) {return ponder::UserObject(source);}

    static T from(bool)
        {PONDER_ERROR(ponder::BadType(ponder::ValueKind::Boolean,ponder::mapType<T>()));}
    static T from(long)
        {PONDER_ERROR(ponder::BadType(ponder::ValueKind::Integer,ponder::mapType<T>()));}
    static T from(double)
        {PONDER_ERROR(ponder::BadType(ponder::ValueKind::Real,   ponder::mapType<T>()));}
    static T from(const ponder::String&)
        {PONDER_ERROR(ponder::BadType(ponder::ValueKind::String, ponder::mapType<T>()));}
    static T from(const ponder::EnumObject&)
        {PONDER_ERROR(ponder::BadType(ponder::ValueKind::Enum,   ponder::mapType<T>()));}
    static T from(const ponder::UserObject& source)
        {return source.get<T>();}
};

/**
 * Specialization of ValueMapper for abstract types
 */
template <typename T>
struct ValueMapper<T, typename std::enable_if<std::is_abstract<T>::value >::type>
{
    static const ponder::ValueKind kind = ponder::ValueKind::User;
    static ponder::UserObject to(const T& source) {return ponder::UserObject(source);}
};

/**
 * Specialization of ValueMapper for booleans
 */
template <>
struct ValueMapper<bool>
{
    static const ponder::ValueKind kind = ponder::ValueKind::Boolean;
    static bool to(bool source) {return source;}

    static bool from(bool source)                  {return source;}
    static bool from(long source)                  {return source != 0;}
    static bool from(double source)                {return source != 0.;}
    static bool from(const ponder::String& source) {return ponder::detail::convert<bool>(source);}
    static bool from(const ponder::EnumObject& source) {return source.value() != 0;}
    static bool from(const ponder::UserObject& source) {return source.pointer() != nullptr;}
};

/**
 * Specialization of ValueMapper for integers
 */
template <typename T>
struct ValueMapper<T,
    typename std::enable_if<
                 std::is_integral<T>::value
                 && !std::is_const<T>::value     // to avoid conflict with ValueMapper<const T>
             >::type >
{
    static const ponder::ValueKind kind = ponder::ValueKind::Integer;
    static long to(T source) {return static_cast<long>(source);}

    static T from(bool source)                    {return static_cast<T>(source);}
    static T from(long source)                    {return static_cast<T>(source);}
    static T from(double source)                  {return static_cast<T>(source);}
    static T from(const ponder::String& source)   {return ponder::detail::convert<T>(source);}
    static T from(const ponder::EnumObject& source)
        {return static_cast<T>(source.value());}
    static T from(const ponder::UserObject&)
        {PONDER_ERROR(ponder::BadType(ponder::ValueKind::User, ponder::ValueKind::Integer));}
};

/*
 * Specialization of ValueMapper for reals
 */
template <typename T>
struct ValueMapper<T,
    typename std::enable_if<
                 std::is_floating_point<T>::value
                 && !std::is_const<T>::value // to avoid conflict with ValueMapper<const T>
             >::type >
{
    static const ponder::ValueKind kind = ponder::ValueKind::Real;
    static double to(T source) {return static_cast<double>(source);}

    static T from(bool source)                    {return static_cast<T>(source);}
    static T from(long source)                    {return static_cast<T>(source);}
    static T from(double source)                  {return static_cast<T>(source);}
    static T from(const ponder::String& source)   {return ponder::detail::convert<T>(source);}
    static T from(const ponder::EnumObject& source) {return static_cast<T>(source.value());}
    static T from(const ponder::UserObject&)
        {PONDER_ERROR(ponder::BadType(ponder::ValueKind::User, ponder::ValueKind::Real));}
};

/**
 * Specialization of ValueMapper for ponder::String
 */
template <>
struct ValueMapper<ponder::String>
{
    static const ponder::ValueKind kind = ponder::ValueKind::String;
    static const ponder::String& to(const ponder::String& source) {return source;}

    static ponder::String from(bool source)
        {return ponder::detail::convert<ponder::String>(source);}
    static ponder::String from(long source)
        {return ponder::detail::convert<ponder::String>(source);}
    static ponder::String from(double source)
        {return ponder::detail::convert<ponder::String>(source);}
    static ponder::String from(const ponder::String& source)
        {return source;}
    static ponder::String from(const ponder::EnumObject& source)
        {return ponder::String(source.name());}
    static ponder::String from(const ponder::UserObject&)
        {PONDER_ERROR(ponder::BadType(ponder::ValueKind::User, ponder::ValueKind::String));}
};

/**
 * Specialization of ValueMapper for const char*.
 * Conversions to const char* are disabled (can't return a pointer to a temporary)
 */
template <>
struct ValueMapper<const char*>
{
    static const ponder::ValueKind kind = ponder::ValueKind::String;
    static ponder::String to(const char* source) {return ponder::String(source);}
    
    template <typename T>
    static const char* from(const T&)
    {
        // If you get this error, it means you're trying to cast
        // a ponder::Value to a const char*, which is not allowed
        return T::CONVERSION_TO_CONST_CHAR_PTR_IS_NOT_ALLOWED();
    }
};

/**
 * Specialization of ValueMapper for arrays.
 * No conversion allowed, only type mapping is provided.
 *
 * Warning: special case for char[] and const char[], they are strings not arrays
 */
template <typename T>
struct ValueMapper<T,
    typename std::enable_if<
            ponder_ext::ArrayMapper<T>::isArray
            && !std::is_same<typename ponder_ext::ArrayMapper<T>::ElementType, char>::value
            && !std::is_same<typename ponder_ext::ArrayMapper<T>::ElementType, const char>::value
        >::type >
{
    static const ponder::ValueKind kind = ponder::ValueKind::Array;
};

/**
 * Specializations of ValueMapper for char arrays.
 * Conversion to char[N] is disabled (can't return an array).
 */
template <std::size_t N>
struct ValueMapper<char[N]>
{
    static const ponder::ValueKind kind = ponder::ValueKind::String;
    static ponder::String to(const char (&source)[N]) {return ponder::String(source);}
};
template <std::size_t N>
struct ValueMapper<const char[N]>
{
    static const ponder::ValueKind kind = ponder::ValueKind::String;
    static ponder::String to(const char (&source)[N]) {return ponder::String(source);}
};

template <>
struct ValueMapper<ponder::detail::string_view>
{
    static const ponder::ValueKind kind = ponder::ValueKind::String;
    static ponder::String to(const ponder::detail::string_view& sv)
        {return ponder::String(sv.data(), sv.length());}
};

/**
 * Specialization of ValueMapper for enum types
 */
template <typename T>
struct ValueMapper<T, typename std::enable_if<std::is_enum<T>::value>::type>
{
    static const ponder::ValueKind kind = ponder::ValueKind::Enum;
    static ponder::EnumObject to(T source) {return ponder::EnumObject(source);}

    static T from(bool source)      {return static_cast<T>(static_cast<long>(source));}
    static T from(long source)      {return static_cast<T>(source);}
    static T from(double source)    {return static_cast<T>(static_cast<long>(source));}
    static T from(const ponder::EnumObject& source)
        {return static_cast<T>(source.value());}
    static T from(const ponder::UserObject&)
        {PONDER_ERROR(ponder::BadType(ponder::ValueKind::User, ponder::ValueKind::Enum));}

    // The string -> enum conversion involves a little more work:
    // we try two different conversions (as a name and as a value)
    static T from(const ponder::String& source)
    {
        // Get the metaenum of T, if any
        const ponder::Enum* metaenum = ponder::enumByTypeSafe<T>();

        // First try as a name
        if (metaenum && metaenum->hasName(source))
            return static_cast<T>(metaenum->value(source));

        // Then try as a number
        long value = ponder::detail::convert<long>(source);
        if (!metaenum || metaenum->hasValue(value))
            return static_cast<T>(value);

        // Not a valid enum name or number: throw an error
        PONDER_ERROR(ponder::BadType(ponder::ValueKind::String, ponder::ValueKind::Enum));
    }
};

/**
 * Specialization of ValueMapper for EnumObject
 */
template <>
struct ValueMapper<ponder::EnumObject>
{
    static const ponder::ValueKind kind = ponder::ValueKind::Enum;
    static const ponder::EnumObject& to(const ponder::EnumObject& source) {return source;}
    static const ponder::EnumObject& from(const ponder::EnumObject& source) {return source;}

    static ponder::UserObject from(bool)
        {PONDER_ERROR(ponder::BadType(ponder::ValueKind::Boolean, ponder::ValueKind::Enum));}
    static ponder::UserObject from(long)
        {PONDER_ERROR(ponder::BadType(ponder::ValueKind::Integer, ponder::ValueKind::Enum));}
    static ponder::UserObject from(double)
        {PONDER_ERROR(ponder::BadType(ponder::ValueKind::Real,   ponder::ValueKind::Enum));}
    static ponder::UserObject from(const ponder::String&)
        {PONDER_ERROR(ponder::BadType(ponder::ValueKind::String, ponder::ValueKind::Enum));}
    static ponder::UserObject from(const ponder::UserObject&)
        {PONDER_ERROR(ponder::BadType(ponder::ValueKind::Enum,   ponder::ValueKind::Enum));}
};

/**
 * Specialization of ValueMapper for ponder::ValueKind.
 */
template <>
struct ValueMapper<ponder::ValueKind>
{
    static const ponder::ValueKind kind = ponder::ValueKind::String;
    static ponder::String to(ponder::ValueKind source)
        {return ponder::String(ponder::detail::valueTypeAsString(source));}
};

/**
 * Specialization of ValueMapper for UserObject
 */
template <>
struct ValueMapper<ponder::UserObject>
{
    static const ponder::ValueKind kind = ponder::ValueKind::User;
    static const ponder::UserObject& to(const ponder::UserObject& source) {return source;}
    static const ponder::UserObject& from(const ponder::UserObject& source) {return source;}

    static ponder::UserObject from(bool)
        {PONDER_ERROR(ponder::BadType(ponder::ValueKind::Boolean, ponder::ValueKind::User));}
    static ponder::UserObject from(long)
        {PONDER_ERROR(ponder::BadType(ponder::ValueKind::Integer, ponder::ValueKind::User));}
    static ponder::UserObject from(double)
        {PONDER_ERROR(ponder::BadType(ponder::ValueKind::Real,   ponder::ValueKind::User));}
    static ponder::UserObject from(const ponder::String&)
        {PONDER_ERROR(ponder::BadType(ponder::ValueKind::String, ponder::ValueKind::User));}
    static ponder::UserObject from(const ponder::EnumObject&)
        {PONDER_ERROR(ponder::BadType(ponder::ValueKind::Enum,   ponder::ValueKind::User));}
};

/**
 * Specialization of ValueMapper for void.
 * Conversion to void should never happen, the only aim of this
 * specialization is to define the proper type mapping.
 */
template <>
struct ValueMapper<void>
{
    static const ponder::ValueKind kind = ponder::ValueKind::None;
};

/**
 * Specialization of ValueMapper for NoType.
 * Conversion to NoType should never happen, the only aim of this
 * specialization is to define the proper mapped type.
 */
template <>
struct ValueMapper<ponder::NoType>
{
    static const ponder::ValueKind kind = ponder::ValueKind::None;
};
    
/**
 * Specialization of ValueMapper for const T -- just forward to ValueMapper<T>
 */
template <typename T>
struct ValueMapper<const T> : public ValueMapper<T> {};

/**
 * Show error for references. Not allowed.
 */
template <typename T>
struct ValueMapper<const T&>
{
    typedef int ReferencesNotAllowed[-(int)sizeof(T)];
};

/**
 * Show error for references using smart pointers.
 */
template <template <typename> class T, typename U>
struct ValueMapper<T<U>,
    typename std::enable_if< ponder::detail::IsSmartPointer<T<U>,U>::value>::type>
{
    typedef int ReferencesNotAllowed[-(int)sizeof(U)];
};

/** \endcond NoDocumentation */

} // namespace ponder_ext

#endif // PONDER_VALUEMAPPER_HPP
