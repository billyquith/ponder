/****************************************************************************
**
** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** CAMP is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** 
** CAMP is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public License
** along with CAMP.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/


#ifndef CAMP_ENUM_HPP
#define CAMP_ENUM_HPP


#include <camp/config.hpp>
#include <camp/enumbuilder.hpp>
#include <camp/detail/enummanager.hpp>
#include <camp/detail/typeid.hpp>
#include <boost/noncopyable.hpp>
#include <string>
#include <vector>


namespace camp
{
/**
 * \brief camp::Enum represents a metaenum composed of <name, value> pairs
 *
 * Enums are declared, bound to a C++ type and filled with the \c declare
 * template function.
 *
 * \code
 * enum MyEnum {one = 1, two = 2, ten = 10};
 *
 * camp::Enum::declare<MyEnum>("MyEnum")
 *     .value("one", one)
 *     .value("two", two)
 *     .value("ten", ten);
 * \endcode
 *
 * It then provides a set of accessors to retrieve names, values and pairs contained in it.
 *
 * \code
 * const camp::Enum& metaenum = camp::enumByType<MyEnum>();
 *
 * bool b1 = metaenum.hasName("one");     // b1 == true
 * bool b2 = metaenum.hasValue(5);        // b2 == false
 *
 * std::string s = metaenum.name(10);     // s == "ten"
 * long l = metaenum.value("two");        // l == 2
 *
 * camp::Enum::Pair p = metaenum.pair(0); // p == {"one", one}
 * \endcode
 *
 * \remark All values and names are unique within the metaenum.
 *
 * \sa Class, EnumBuilder
 */
class CAMP_API Enum : boost::noncopyable
{
public:

    /**
     * \brief Structure defining the <name, value> pairs stored in metaenums
     */
    struct Pair
    {
        std::string name; ///< Name of the pair
        long value; ///< Value of the pair
    };

public:

    /**
     * \brief Declare a new metaenum
     *
     * This is the function to call to create a new metaenum. The template
     * parameter T is the C++ enum type that will be bound to the metaclass.
     *
     * \param name Name of the metaenum in CAMP. This name identifies
     *             the metaenum and thus has to be unique
     *
     * \return A EnumBuilder object that will provide functions
     *         to fill the new metaenum with values.
     */
    template <typename T>
    static EnumBuilder declare(const std::string& name);

    /**
     * \brief Return the name of the metaenum
     *
     * \return String containing the name of the metaenum
     */
    const std::string& name() const;

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
    const Pair& pair(std::size_t index) const;

    /**
     * \brief Check if the enum contains a name
     *
     * \param name Name to check
     *
     * \return True if the metaenum contains a pair whose name is \a name
     */
    bool hasName(const std::string& name) const;

    /**
     * \brief Check if the enum contains a value
     *
     * \param value Value to check
     *
     * \return True if the metaenum contains a pair whose value is \a value
     */
    bool hasValue(long value) const;

    /**
     * \brief Return the name corresponding to given a value
     *
     * \param value Value to get
     *
     * \return Name of the requested value
     *
     * \throw InvalidEnumValue value doesn't exist in the metaenum
     */
    const std::string& name(long value) const;

    /**
     * \brief Return the value corresponding to given a name
     *
     * \param name Name to get
     *
     * \return Value of the requested name
     *
     * \throw InvalidEnumName name doesn't exist in the metaenum
     */
    long value(const std::string& name) const;

    /**
     * \brief Operator == to check equality between two metaenums
     *
     * Two metaenums are equal if their name is the same.
     *
     * \param other Metaenum to compare with this
     *
     * \return True if both metaenums are the same, false otherwise
     */
    bool operator==(const Enum& other) const;

    /**
     * \brief Operator != to check inequality between two metaenums
     *
     * \param other Metaenum to compare with this
     *
     * \return True if metaenums are different, false if they are equal
     */
    bool operator!=(const Enum& other) const;

private:

    friend class EnumBuilder;
    friend class detail::EnumManager;

    /**
     * \brief Construct the metaenum from its name
     *
     * \param name Name of the metaenum
     */
    Enum(const std::string& name);

    typedef std::vector<Pair> PairArray;

    std::string m_name; ///< Name of the metaenum
    PairArray m_pairs; ///< List of pairs stored in the metaenum
};

} // namespace camp

#include <camp/enum.inl>


#endif // CAMP_ENUM_HPP
