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


#ifndef CAMP_INVALIDFUNCTION_HPP
#define CAMP_INVALIDFUNCTION_HPP


#include <camp/error.hpp>


namespace camp
{
class Class;

/**
 * \brief Error thrown when requesting a function which doesn't exist in a metaclass
 */
class CAMP_API InvalidFunction : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param function Name of the requested function
     * \param ownerClass Owner metaclass
     */
    InvalidFunction(const char* function, const Class& ownerClass) throw();

    /**
     * \brief Destructor
     */
    virtual ~InvalidFunction() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    virtual const char* what() const throw();

    /**
     * \brief Return the requested function name
     *
     * \return Name of the requested function
     */
    const char* function() const throw();

    /**
     * \brief Return the owner metaclass
     *
     * \return Reference to the owner metaclass
     */
    const Class& ownerClass() const throw();

private:

    char m_function[256]; ///< Name of the requested function
    const Class* m_ownerClass; ///< Owner metaclass
};

} // namespace camp


#endif // CAMP_INVALIDFUNCTION_HPP
