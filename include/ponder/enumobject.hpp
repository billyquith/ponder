/****************************************************************************
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
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


#ifndef PONDER_ENUMOBJECT_HPP
#define PONDER_ENUMOBJECT_HPP


#include <ponder/enumget.hpp>
#include <string>


namespace ponder
{
/**
 * \brief Wrapper to manipulate enumerated values in the Ponder system
 *
 * ponder::EnumObject is an abstract representation of enum values, and supports
 * conversions from strings and integers.
 *
 * \sa UserObject
 */
class PONDER_API EnumObject
{
public:

    /**
     * \brief Construct the enum object from an enumerated value
     *
     * \param value Value to store in the enum object
     */
    template <typename T>
    EnumObject(T value, typename std::enable_if<std::is_enum<T>::value >::type* = 0);

    /**
     * \brief Get the value of the enum object
     *
     * \return Integer value of the enum object
     */
    long value() const;
    
    /**
     * \brief Get the value of the enum class object
     *
     * \return Enum typed value of the enum class object
     */
    template <typename E>
    E value() const;
    
    /**
     * \brief Get the name of the enum object
     *
     * \return String containing the name of the enum object
     */
    const std::string& name() const;

    /**
     * \brief Retrieve the metaenum of the stored enum object
     *
     * \return Reference to the object's metaenum
     */
    const Enum& getEnum() const;

    /**
     * \brief Operator == to compare equality between two enum objects
     *
     * Two enum objects are equal if their metaenums and values are both equal
     *
     * \param other Enum object to compare with this
     *
     * \return True if both enum objects are the same, false otherwise
     */
    bool operator == (const EnumObject& other) const;

    /**
     * \brief Operator < to compare two enum objects
     *
     * \param other Enum object to compare with this
     *
     * \return True if this < other
     */
    bool operator < (const EnumObject& other) const;

private:

    long m_value; ///< Value
    const Enum* m_enum; ///< Metaenum associated to the value
};

} // namespace ponder

#include <ponder/enumobject.inl>


#endif // PONDER_ENUMOBJECT_HPP
