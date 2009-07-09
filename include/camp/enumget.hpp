/****************************************************************************
**
** Copyright (C) 2009 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** Commercial Usage
** Licensees holding valid CAMP Commercial licenses may use this file in
** accordance with the CAMP Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and TECHNOGERMA Systems France.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3.txt included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at sales@technogerma.fr.
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
 * \brief Get the number of metaenums bound to a C++ type
 *
 * \return Number of metaenums bound to T
 */
template <typename T>
std::size_t enumCount();

/**
 * \relates Enum
 *
 * \brief Get the number of metaenums bound to an object
 *
 * This is equivalent to calling enumCount<T>().
 *
 * \param value Value to get the metaenum count from
 *
 * \return Number of metaenums bound to T
 */
template <typename T>
std::size_t enumCount(T value);

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
 * \throw InvalidIndex index is out of range
 */
const Enum& enumByIndex(std::size_t index);

/**
 * \relates Enum
 *
 * \brief Get a metaenum from its C++ type
 *
 * If more than one metaenum is bound to T, then the
 * index parameter is used to choose which metaenum to retrieve.
 * Otherwise it is ignored.
 *
 * \param index Index of the metaenum to retrieve, if multiple metaenums are bound to T (0 by default)
 *
 * \return Reference to the index-th metaenum bound to type T
 *
 * \throw InvalidEnum no metaenum has been declared for T
 * \throw InvalidIndex index is out of range
 */
template <typename T>
const Enum& enumByType(std::size_t index = 0);

/**
 * \relates Enum
 *
 * \brief Get a metaenum from a C++ object
 *
 * It is equivalent to calling enumByType<T>(index).
 *
 * \param value Value to get the metaenum of
 * \param index Index of the metaenum to retrieve, if multiple metaenums are bound to T (0 by default)
 *
 * \return Reference to the index-th metaenum bound to type T
 *
 * \throw InvalidEnum no metaenum has been declared for T
 * \throw InvalidIndex index is out of range
 */
template <typename T>
const Enum& enumByObject(T value, std::size_t index = 0);

/**
 * \relates Enum
 *
 * \brief Get a metaenum from its name
 *
 * \param name Name of the metaenum to retrieve (case sensitive)
 *
 * \return Reference to the requested metaenum
 *
 * \throw InvalidEnum name is not a valid metaenum name
 */
const Enum& enumByName(const std::string& name);

/**
 * \relates Enum
 *
 * \brief Get a metaenum from its C++ type
 *
 * This function, unlike enumByType, returns a null pointer on failure. It's meant to
 * be used in contexts where T may not have a valid metaenum declared.
 *
 * \param index Index of the metaenum to retrieve, if multiple metaenums are bound to T (0 by default)
 *
 * \return Pointer to the index-th metaenum bound to type T, or 0 if no metaenum has been declared
 */
template <typename T>
const Enum* enumByTypeSafe(std::size_t index = 0);

} // namespace camp

#include <camp/enumget.inl>


#endif // CAMP_ENUMGET_HPP
