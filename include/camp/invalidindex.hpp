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


#ifndef CAMP_INVALIDINDEX_HPP
#define CAMP_INVALIDINDEX_HPP


#include <camp/error.hpp>


namespace camp
{
/**
 * \brief Error thrown when using an index which is out of bounds
 *
 * This error may be thrown by every function which takes an index in its parameters
 */
class CAMP_API InvalidIndex : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param index Invalid index
     * \param size Allowed size
     */
    InvalidIndex(std::size_t index, std::size_t size) throw();

    /**
     * \brief Destructor
     */
    virtual ~InvalidIndex() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    virtual const char* what() const throw();

    /**
     * \brief Return the invalid index which was passed
     *
     * \return Index
     */
    std::size_t index() const throw();

    /**
     * \brief Return the allowed size
     *
     * \return Valid range
     */
    std::size_t size() const throw();

private:

    std::size_t m_index; ///< Invalid index
    std::size_t m_size; ///< Valid size
};

} // namespace camp


#endif // CAMP_INVALIDINDEX_HPP
