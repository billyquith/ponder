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


#ifndef CAMP_INVALIDENUMNAME_HPP
#define CAMP_INVALIDENUMNAME_HPP


#include <camp/error.hpp>


namespace camp
{
class Enum;

/**
 * \brief Error thrown when requesting the name of a pair which doesn't exist in a metaenum
 */
class CAMP_API InvalidEnumName : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param name Name of the requested pair
     * \param ownerEnum Owner metaenum
     */
    InvalidEnumName(const char* name, const Enum& ownerEnum) throw();

    /**
     * \brief Destructor
     */
    virtual ~InvalidEnumName() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    virtual const char* what() const throw();

    /**
     * \brief Return the requested pair name
     *
     * \return Name of the requested pair
     */
    const char* name() const throw();

    /**
     * \brief Return the owner metaenum
     *
     * \return Reference to the owner metaenum
     */
    const Enum& ownerEnum() const throw();

private:

    char m_name[256]; ///< Name of the requested pair
    const Enum* m_ownerEnum; ///< Owner metaenum
};

} // namespace camp


#endif // CAMP_INVALIDENUMNAME_HPP
