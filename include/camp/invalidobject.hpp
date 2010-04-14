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


#ifndef CAMP_INVALIDOBJECT_HPP
#define CAMP_INVALIDOBJECT_HPP


#include <camp/error.hpp>


namespace camp
{
class UserObject;

/**
 * \brief Error thrown when using an invalid user object
 *
 * This error may be thrown if user object is null, of if its type doesn't match the requested one
 */
class CAMP_API InvalidObject : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param object User object
     */
    InvalidObject(const UserObject& object) throw();

    /**
     * \brief Destructor
     */
    virtual ~InvalidObject() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    virtual const char* what() const throw();

    /**
     * \brief Return the user object
     *
     * \return Reference to the user object
     */
    const UserObject& object() const throw();

private:

    const UserObject* m_object; ///< User object
};

} // namespace camp


#endif // CAMP_INVALIDOBJECT_HPP
