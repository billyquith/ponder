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


#ifndef CAMP_TYPE_HPP
#define CAMP_TYPE_HPP


#include <string>
#include <cassert>


namespace camp
{
/**
 * \brief Special empty type associated to \c noType
 *
 * This type is mainly used when writing custom \c Value visitors
 */
struct NoType
{
};

/**
 * \brief Enumeration of abstract types supported by CAMP
 *
 * \sa ValueMapper
 */
enum Type
{
    noType,     ///< No type has been defined yet
    boolType,   ///< Boolean type (bool)
    intType,    ///< Integer types (unsigned/signed char short int long)
    realType,   ///< Real types (float, double)
    stringType, ///< String types (char*, std::string)
    enumType,   ///< Enumerated types
    arrayType,  ///< Array types (std::vector, std::list, T[])
    userType    ///< User-defined classes
};

} // namespace camp


#endif // CAMP_TYPE_HPP
