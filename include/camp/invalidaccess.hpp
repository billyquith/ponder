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


#ifndef CAMP_INVALIDACCESS_HPP
#define CAMP_INVALIDACCESS_HPP


#include <camp/error.hpp>


namespace camp
{
/**
 * \brief Error thrown when doing an invalid access (read, write or call) to a property or a function
 *
 * It may happen when:
 *
 * \li Calling \c get on a non-readable property
 * \li Calling \c set on a non-writable property
 * \li Calling \c call on a non-callable function
 */
class CAMP_API InvalidAccess : public Error
{
public:

    /**
     * \brief Enumeration of action types
     */
    enum Action
    {
        Read,  ///< Read action (call to \c get)
        Write, ///< Write action (call to \c set)
        Call   ///< Call action (call to \c call)
    };

    /**
     * \brief Constructor
     *
     * \param attribute Name of the owner property/function
     * \param action Type of action which triggered the error
     */
    InvalidAccess(const char* attribute, Action action) throw();

    /**
     * \brief Destructor
     */
    virtual ~InvalidAccess() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    virtual const char* what() const throw();

    /**
     * \brief Get the owner property/function
     *
     * \return Name of the owner attribute
     */
    const char* attribute() const throw();

    /**
     * \brief Get the type of action
     *
     * \return Type of action which triggered the error
     */
    Action action() const throw();

private:

    char m_attribute[256]; ///< Name of the owner attribute (property or function)
    Action m_action; ///< Type of action which triggered the error
};

} // namespace camp


#endif // CAMP_INVALIDACCESS_HPP
