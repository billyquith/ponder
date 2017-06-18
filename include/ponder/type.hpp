/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2017 Nick Trout.
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


#ifndef PONDER_TYPE_HPP
#define PONDER_TYPE_HPP

/**
 * \namespace ponder
 * \brief Root namespace that encapsulates all of Ponder.
 *
 * \namespace ponder::detail
 * \brief Ponder hidden implementation details.
 *
 * \namespace ponder::policy
 * \brief Ponder policy options.
 */

namespace ponder {
    
/**
 * \brief Special empty type associated to \c noType
 *
 * This type is mainly used when writing custom \c Value visitors
 */
struct NoType
{
};

/**
 * \brief Enumeration of abstract value types supported by Ponder Values
 *
 * \sa Value ValueMapper
 */
enum class ValueKind
{
    None,       ///< No type has been defined yet
    Boolean,    ///< Boolean type (bool)
    Integer,    ///< Integer types (unsigned/signed char short int long)
    Real,       ///< Real types (float, double)
    String,     ///< String types (char*, ponder::String)
    Enum,       ///< Enumerated types
    Array,      ///< Array types (std::vector, std::list, T[])
    User        ///< User-defined classes
};

/**
 * \brief Enumeration of kinds of object
 *
 * \sa Value ValueMapper
 */
enum class ObjectKind
{
    None,               // not an object
    Object,             // a raw object, e.g. int
    Pointer,            // pointer to an object, e.g. T*
    Reference,          // reference to an object, e.g. T&
    SmartPointer,       // smart pointer reference
    BuiltinArray,       // builtin array, e.g. T[N]
};
    
/**
 * \brief Enumeration of the kinds of function recognised
 *
 * \sa Function
 */
enum class FunctionKind
{
    None,               ///< not a function
    Function,           ///< a function
    MemberFunction,     ///< function in a class or struct
    MemberObject,       ///< object in a class or struct
    FunctionWrapper,    ///< `std::function<>`
    BindExpression,     ///< `std::bind()`
    Lambda              ///< lambda function `[](){}`
};

    
namespace policy {

/**
 * \brief Enumeration of the kinds of return policy
 */
enum class ReturnKind
{
    NoReturn,       // void, returns nothing
    Copy,
    InternalRef,
    Multiple
};
    
/**
 * \brief Call return copy policy
 *
 * When added to a function declaration this sets the call return policy to copy. Any
 * values returned by a function are copied. This is the default behaviour.
 */
struct ReturnCopy
{
    static constexpr ReturnKind kind = ReturnKind::Copy; ///< The policy enum kind.
};

/**
 * \brief Call return internal reference policy
 *
 * When added to a function declaration this sets the call return policy to internal
 * reference. References returned by the function are returned as references values. This
 * is useful for declaring things like singletons, which return values that we do not
 * want to copy.
 */
struct ReturnInternalRef
{
    static constexpr ReturnKind kind = ReturnKind::InternalRef;  ///< The policy enum kind.
};

/**
 * \brief Return multiple values
 *
 * When added to a function declaration this sets the call return policy to support multiple
 * value returning. The function should return a `std::tuple<...>` containing the multiple
 * values. Note that the tuple needs to be declared as a type.
 */
struct ReturnMultiple
{
    static constexpr ReturnKind kind = ReturnKind::Multiple; ///< The policy enum kind.
};
    
} // namespace policy
    
} // namespace ponder


#endif // PONDER_TYPE_HPP
