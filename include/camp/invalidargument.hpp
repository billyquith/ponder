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


#ifndef CAMP_INVALIDARGUMENT_HPP
#define CAMP_INVALIDARGUMENT_HPP


#include <camp/invalidvalue.hpp>


namespace camp
{
/**
 * \brief Error thrown when passing an invalid argument to a function call
 */
class CAMP_API InvalidArgument : public InvalidValue
{
public:

    /**
     * \brief Constructor
     *
     * \param sourceType Type of the source value
     * \param requestedType Type the value is being converted to
     * \param function Name of the owner function
     * \param index Index of the argument
     */
    InvalidArgument(Type sourceType, Type requestedType, const char* function, std::size_t index) throw();

    /**
     * \brief Destructor
     */
    virtual ~InvalidArgument() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    virtual const char* what() const throw();

    /**
     * \brief Return the name of the owner function
     *
     * \return Name of the function
     */
    const char* function() const throw();

    /**
     * \brief Return the index of the argument
     *
     * \return Index of the argument within the function call
     */
    std::size_t index() const throw();

private:

    char m_function[256]; ///< Name of the owner function
    std::size_t m_index; ///< Index of the argument in the function call
};

} // namespace camp


#endif // CAMP_INVALIDARGUMENT_HPP
