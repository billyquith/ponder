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


#ifndef CAMP_ENUMGET_HPP
#define CAMP_ENUMGET_HPP


#include <camp/config.hpp>
#include <camp/error.hpp>
#include <camp/detail/typeid.hpp>
#include <camp/detail/enummanager.hpp>
#include <string>


namespace camp
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
 * \return Pointer to the metaenum bound to type T, or 0 if no metaenum has been declared
 */
template <typename T>
const Enum* enumByTypeSafe();

} // namespace camp

#include <camp/enumget.inl>


#endif // CAMP_ENUMGET_HPP
