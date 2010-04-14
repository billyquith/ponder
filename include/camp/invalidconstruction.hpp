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


#ifndef CAMP_INVALIDCONSTRUCTION_HPP
#define CAMP_INVALIDCONSTRUCTION_HPP


#include <camp/args.hpp>
#include <camp/error.hpp>


namespace camp
{
class Class;

/**
 * \brief Error thrown when an invalid call to Class::construct is made
 *
 * This exception may be thrown by Class::construct if no constructor matches
 * the list of arguments, or if the target type is unrelated to the metaclass.
 */
class CAMP_API InvalidConstruction : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param args List of arguments passed to the constructor
     * \param ownerClass Reference to the owner metaclass
     */
    InvalidConstruction(const Args& args, const Class& ownerClass) throw();

    /**
     * \brief Destructor
     */
    virtual ~InvalidConstruction() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    virtual const char* what() const throw();

    /**
     * \brief Get the list of arguments
     *
     * \return Arguments that were passed to the constructor
     */
    const Args& args() const throw();

    /**
     * \brief Return the owner metaclass
     *
     * \return Reference to the owner metaclass
     */
    const Class& ownerClass() const throw();

private:

    Args m_args; ///< Arguments that were passed to the constructor
    const Class* m_ownerClass; ///< Owner metaclass
};

} // namespace camp


#endif // CAMP_INVALIDCONSTRUCTION_HPP
