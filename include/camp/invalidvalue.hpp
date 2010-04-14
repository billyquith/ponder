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


#ifndef CAMP_INVALIDVALUE_HPP
#define CAMP_INVALIDVALUE_HPP


#include <camp/error.hpp>
#include <camp/type.hpp>


namespace camp
{
/**
 * \brief Error thrown when using an invalid value
 *
 * This error may be thrown when a value can't be converted to the requested type
 */
class CAMP_API InvalidValue : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param sourceType Type of the source value
     * \param requestedType Type the value is being converted to
     */
    InvalidValue(Type sourceType, Type requestedType) throw();

    /**
     * \brief Destructor
     */
    virtual ~InvalidValue() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    virtual const char* what() const throw();

    /**
     * \brief Return the type of the source value
     *
     * \return Source type
     */
    Type sourceType() const throw();

    /**
     * \brief Return the type the value is being converted to
     *
     * \return Requested type
     */
    Type requestedType() const throw();

private:

    Type m_sourceType; ///< Type of the source value
    Type m_requestedType; ///< Type the value is being converted to
};

} // namespace camp


#endif // CAMP_INVALIDVALUE_HPP
