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
#ifndef PONDER_ENUM_HPP
#define PONDER_ENUM_HPP

#include <ponder/config.hpp>
#include <ponder/enumbuilder.hpp>
#include <ponder/enumget.hpp>
#include <ponder/pondertype.hpp>
#include <ponder/detail/typeid.hpp>
#include <ponder/detail/dictionary.hpp>
#include <string>

namespace ponder {
    
/**
 * \brief ponder::Enum represents a metaenum composed of <name, value> pairs
 *
 * Enums are declared, bound to a C++ type and filled with the \c declare
 * template function.
 *
 * \code
 * enum MyEnum {one = 1, two = 2, ten = 10};
 *
 * ponder::Enum::declare<MyEnum>("MyEnum")
 *     .value("one", one)
 *     .value("two", two)
 *     .value("ten", ten);
 * \endcode
 *
 * It then provides a set of accessors to retrieve names, values and pairs contained in it.
 *
 * \code
 * const ponder::Enum& metaenum = ponder::enumByType<MyEnum>();
 *
 * bool b1 = metaenum.hasName("one");     // b1 == true
 * bool b2 = metaenum.hasValue(5);        // b2 == false
 *
 * Id s = metaenum.name(10);     // s == "ten"
 * EnumValue l = metaenum.value("two");   // l == 2
 *
 * ponder::Enum::Pair p = metaenum.pair(0); // p == {"one", one}
 * \endcode
 *
 * \remark All values and names are unique within the metaenum.
 *
 * \sa Class, EnumBuilder
 */
class PONDER_API Enum : public Type
{
    PONDER__NON_COPYABLE(Enum);
public:
    typedef long EnumValue;  //!< Type used to hold the enum value.

    /**
     * \brief Structure defining the <name, value> pairs stored in metaenums
     */
    struct Pair {
        Id name;            //!< Enum name
        EnumValue value;    //!< Enum value
        
        /**
         * \brief Constructor.
         * 
         * \param name_ Name of the enum item.
         * \param value_ Value of the enum item.
         */
        Pair(IdRef name_, EnumValue value_) : name(name_), value(value_) {}
        
        /**
         * \brief Helper to return value as require enum class type.
         *
         * E.g. `pair.valueAs<MyEnum>()`.
         *
         * \return Value as requested type
         */
        template <typename E>
        E valueAs() const {return static_cast<E>(value);}
    };
    
    /**
     * \brief Declare a new metaenum
     *
     * This is the function to call to create a new metaenum. The template
     * parameter T is the C++ enum type that will be bound to the metaclass.
     *
     * \param name Name of the metaenum in Ponder. This name identifies
     *             the metaenum and thus has to be unique
     *
     * \return A EnumBuilder object that will provide functions
     *         to fill the new metaenum with values.
     */
    template <typename T>
    static EnumBuilder declare(IdRef name = Id());

    /**
     * \brief Undeclare an existing metaenum
     *
     * Call this to undeclare an Enum that you no longer require.
     *
     * \param name Name of the existing metaenum in Ponder.
     *
     * \note See notes for Class::undeclare about registration.
     *
     * \see Class::undeclare
     */
    template <typename T>
    static void undeclare(IdRef name = Id());
    
public:

    /**
     * \brief Return the name of the metaenum
     *
     * \return String containing the name of the metaenum
     */
    IdReturn name() const;
        
    /**
     * \brief Return the size of the metaenum
     *
     * \return Total number of values contained in the metaenum
     */
    std::size_t size() const;

    /**
     * \brief Get a pair by its index
     *
     * \param index Index of the pair to get
     *
     * \return index-th pair
     *
     * \throw OutOfRange index is out of range
     */
    Pair pair(std::size_t index) const;

    /**
     * \brief Check if the enum contains a name
     *
     * \param name Name to check
     *
     * \return True if the metaenum contains a pair whose name is \a name
     */
    bool hasName(IdRef name) const;

    /**
     * \brief Check if the enum contains a value
     *
     * \param value Value to check
     *
     * \return True if the metaenum contains a pair whose value is \a value
     */
    bool hasValue(EnumValue value) const;

    /**
     * \brief Check if the enum contains a value
     *
     * This generic version of hasValue() allows enum classes to be queried.
     *
     * \param value Value to check
     *
     * \return True if the metaenum contains a pair whose value is \a value
     */
    template <typename E>
    bool hasValue(E value) const
    {
        return hasValue(static_cast<EnumValue>(value));
    }

    /**
     * \brief Return the name corresponding to given a value
     *
     * \param value Value to get
     *
     * \return Name of the requested value
     *
     * \throw InvalidEnumValue value doesn't exist in the metaenum
     */
    IdReturn name(EnumValue value) const;

    /**
     * \brief Return the name corresponding to given a value for enum class
     * 
     * \param value Value to get
     *
     * \return Name of the requested value
     *
     * \throw InvalidEnumValue value doesn't exist in the metaenum
     */
    template <typename E>
    IdReturn name(E value) const {return name(static_cast<EnumValue>(value));}

    /**
     * \brief Return the value corresponding to given a name
     *
     * \param name Name to get
     *
     * \return Value of the requested name
     *
     * \throw InvalidEnumName name doesn't exist in the metaenum
     */
    EnumValue value(IdRef name) const;

    /**
     * \brief Return the value corresponding to given a name for enum class
     *
     * Enum classes are strongly typed so the return type needs to be specified,
     * e.g. `MyEnum a = enum.value<MyEnum>("one");`
     *
     * \param name Name to get
     *
     * \return Value of the requested name as requested type
     *
     * \throw InvalidEnumName name doesn't exist in the metaenum
     */
    template <typename E>
    E value(IdRef name) const {return static_cast<E>(value(name));}

    /**
     * \brief Operator == to check equality between two metaenums
     *
     * Two metaenums are equal if their name is the same.
     *
     * \param other Metaenum to compare with this
     *
     * \return True if both metaenums are the same, false otherwise
     */
    bool operator == (const Enum& other) const;

    /**
     * \brief Operator != to check inequality between two metaenums
     *
     * \param other Metaenum to compare with this
     *
     * \return True if metaenums are different, false if they are equal
     */
    bool operator != (const Enum& other) const;

private:

    friend class EnumBuilder;
    friend class detail::EnumManager;

    /**
     * \brief Construct the metaenum from its name
     *
     * \param name Name of the metaenum
     */
    Enum(IdRef name);
    
    typedef detail::Dictionary<Id, IdRef, EnumValue> EnumTable;
    
    Id m_name;     ///< Name of the metaenum
    EnumTable m_enums;      ///< Table of enums
};

} // namespace ponder

#include <ponder/enum.inl>

#endif // PONDER_ENUM_HPP
