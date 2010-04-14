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


#ifndef CAMP_INVALIDENUM_HPP
#define CAMP_INVALIDENUM_HPP


#include <camp/error.hpp>


namespace camp
{
/**
 * \brief Error thrown when requesting a metaenum which doesn't exist
 */
class CAMP_API InvalidEnum : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param enumName Name of the requested metaenum
     */
    InvalidEnum(const char* enumName) throw();

    /**
     * \brief Destructor
     */
    virtual ~InvalidEnum() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    virtual const char* what() const throw();

    /**
     * \brief Return the requested metaenum name
     *
     * \return Name of the metaenum
     */
    const char* enumName() const throw();

private:

    char m_enumName[256]; ///< Name of the requested metaenum
};

} // namespace camp


#endif // CAMP_INVALIDENUM_HPP
