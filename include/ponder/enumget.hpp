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


#ifndef PONDER_ENUMGET_HPP
#define PONDER_ENUMGET_HPP


#include <ponder/error.hpp>
#include <ponder/detail/typeid.hpp>
#include <ponder/detail/enummanager.hpp>
#include <string>


namespace ponder
{
/**
 * \relates Enum
 *
 * \brief Get the total number of existing metaenums
 *
 * \return Global metaenum count
 */
std::size_t enumCount();

/**
 * \relates Enum
 *
 * \brief Get a metaenum from its global index
 *
 * The purpose of this function is to provide a way to iterate through
 * all existing metaenums. \sa enumCount
 *
 * \param index Index of the metaenum to get
 *
 * \return Reference to the index-th metaenum
 *
 * \throw OutOfRange index is out of range
 */
const Enum& enumByIndex(std::size_t index);

/**
 * \relates Enum
 *
 * \brief Get a metaenum from its name
 *
 * \param name Name of the metaenum to retrieve (case sensitive)
 *
 * \return Reference to the requested metaenum
 *
 * \throw EnumNotFound name is not a valid metaenum name
 */
const Enum& enumByName(const std::string& name);

/**
 * \relates Enum
 *
 * \brief Get a metaenum from a C++ object
 *
 * It is equivalent to calling enumByType<T>(index).
 *
 * \param value Value to get the metaenum of
 *
 * \return Reference to the metaenum bound to type T
 *
 * \throw EnumNotFound no metaenum has been declared for T
 */
template <typename T>
const Enum& enumByObject(T value);

/**
 * \relates Enum
 *
 * \brief Get a metaenum from its C++ type
 *
 * \return Reference to the metaenum bound to type T
 *
 * \throw EnumNotFound no metaenum has been declared for T
 */
template <typename T>
const Enum& enumByType();

/**
 * \relates Enum
 *
 * \brief Get a metaenum from its C++ type
 *
 * \return Pointer to the metaenum bound to type T, or null pointer if no metaenum has been declared
 */
template <typename T>
const Enum* enumByTypeSafe();

} // namespace ponder

#include <ponder/enumget.inl>


#endif // PONDER_ENUMGET_HPP
