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
#ifndef PONDER_VALUE_HPP
#define PONDER_VALUE_HPP

#include <ponder/type.hpp>
#include <ponder/enumobject.hpp>
#include <ponder/userobject.hpp>
#include <ponder/valuemapper.hpp>
#include <ponder/detail/valueimpl.hpp>
#include <ponder/detail/variant.hpp>
#include <iosfwd>
#include <string>

namespace ponder
{

/**
 * \brief Variant class which is used to wrap values in the Ponder system
 *
 * The Value class can store any type of variable, and supports conversion
 * to any type compatible with the stored type.
 *
 * \code
 * ponder::Value v1 = true;
 * ponder::Value v2 = 10;
 * ponder::Value v3 = "24.5";
 * ponder::Value v4 = myObject;
 *
 * bool        b = v1; // b == true
 * String      s = v2; // s == "10"
 * float       f = v3; // f == 24.5
 * MyObject    o = v4; // o == myObject
 * \endcode
 *
 * It also supports unary and binary visitation for type-safe processing
 * depending on the stored type.
 *
 * \remark The set of supported types can be extended by specializing the
 * ponder_ext::ValueMapper template.
 *
 * \sa ValueVisitor, ponder_ext::ValueMapper
 */
class PONDER_API Value
{
public:

    /**
     * \brief Default constructor, constructs a null value
     */
    Value();

    /**
     * \brief Construct the value from a variable of type T
     *
     * \param val Value to store
     */
    template <typename T>
    Value(const T& val);

    /**
     * \brief Copy constructor
     *
     * \param other Value to copy
     */
    Value(const Value& other);

    /**
     * \brief Move constructor
     *
     * \param other Value to move
     */
    Value(Value&& other);

    /**
     * \brief Assignment operator
     *
     * \param other Value to assign to this
     */
    void operator = (const Value& other);
    
    /**
     * \brief Return the Ponder runtime kind of the value
     *
     * \return Type of the value
     */
    ValueKind kind() const;

    /**
     * \brief Convert the value to the type T
     *
     * Convert the value contained to the type provided. An exception is throw if the target
     * type is not compatible. The value returned will be a copy. See ref() and cref() for
     * referencing the internal data.
     *
     * \return Value converted to T
     *
     * \throw BadType the stored value is not convertible to T
     */
    template <typename T>
    T to() const;

    /**
     * \brief Get a reference to the value data contained
     *
     * Get a reference to the contained value of type T. The user is responsible for ensuring
     * that the type passed is correct. See cref() for a non-const reference, or to() to
     * convert the value.
     *
     * \return A non-const reference to the contained data.
     */
    template <typename T>
    T& ref();

    /**
     * \brief Get a const reference to the value data contained
     *
     * Get a const reference to the contained value of type T. The user is responsible for 
     * ensuring that the type passed is correct. See ref() for a const reference, or to() to
     * convert the value.
     *
     * \return A const reference to the contained data.
     */
    template <typename T>
    const T& cref() const;

    /**
     * \brief Check if the stored value can be converted to a type T
     *
     * If this function returns true, then calling to<T>() or operator T() will succeed.
     *
     * \return True if conversion is possible, false otherwise
     */
    template <typename T>
    bool isCompatible() const;

    /**
     * \brief Visit the value with a unary visitor
     *
     * Using this function allows to dispatch an operation depending on the stored type.
     *
     * \param visitor Visitor to apply (must inherit from ValueVisitor<type_to_return>)
     *
     * \return Value returned by the visitor
     */
    template <typename T>
    typename T::result_type visit(T visitor) const;

    /**
     * \brief Visit the value and another one with a binary visitor
     *
     * Using this function allows to dispatch a binary operation depending on the stored type
     * of both values.
     *
     * \param visitor Visitor to apply (must inherit from ValueVisitor<type_to_return>)
     * \param other Other value to visit
     *
     * \return Value returned by the visitor
     */
    template <typename T>
    typename T::result_type visit(T visitor, const Value& other) const;

    /**
     * \brief Operator == to compare equality between two values
     *
     * Two values are equal if their Ponder type and value are equal.
     * It uses the == operator of the stored type.
     *
     * \param other Value to compare with this
     *
     * \return True if both values are the same, false otherwise
     */
    bool operator == (const Value& other) const;

    /**
     * \brief Operator != to compare equality between two values
     *
     * \see operator==
     *
     * \return True if both values are not the same, false otherwise
     */
    bool operator != (const Value& other) const {return !(*this == other);}

    /**
     * \brief Operator < to compare two values
     *
     * \param other Value to compare with this
     *
     * \return True if this < other
     */
    bool operator < (const Value& other) const;

    /**
     * \brief Special Value instance representing an empty value
     */
    static const Value nothing;

private:

    typedef mapbox::util::variant<
                NoType, bool, long, double, ponder::String, EnumObject, UserObject
            > Variant;

    Variant m_value; // Stored value
    ValueKind m_type; // Ponder type of the value
};

/**
 * \brief Overload of operator >> to extract a ponder::Value from a standard stream
 *
 * \param stream Source input stream
 * \param value Value to fill
 *
 * \return Reference to the input stream
 */
PONDER_API std::istream& operator>>(std::istream& stream, Value& value);

/**
 * \brief Overload of operator << to print a ponder::Value into a standard stream
 *
 * \param stream Target output stream
 * \param value Value to print
 *
 * \return Reference to the output stream
 */
PONDER_API std::ostream& operator<<(std::ostream& stream, const Value& value);

} // namespace ponder

#include <ponder/value.inl>

#endif // PONDER_VALUE_HPP
