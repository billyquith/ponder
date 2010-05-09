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


#ifndef CAMP_BADTYPE_HPP
#define CAMP_BADTYPE_HPP


#include <camp/error.hpp>
#include <camp/type.hpp>


namespace camp
{
/**
 * \brief Error thrown when providing a metavalue whose type is incompatible with what's expected
 */
class CAMP_API BadType : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param provided Provided type
     * \param expected Expected type
     */
    BadType(Type provided, Type expected);

    /**
     * \brief Destructor
     */
    virtual ~BadType() throw();

protected:

    /**
     * \brief Constructor for derived classes
     *
     * \param message Description of the error
     */
    BadType(const std::string& message);

    /**
     * \brief Get the string name of a CAMP type
     *
     * \param type Type to convert
     *
     * \return Name of the provided type
     */
    static std::string typeName(Type type);
};

} // namespace camp


#endif // CAMP_BADTYPE_HPP
