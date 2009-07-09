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
