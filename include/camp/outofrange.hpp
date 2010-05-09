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


#ifndef CAMP_OUTOFRANGE_HPP
#define CAMP_OUTOFRANGE_HPP


#include <camp/error.hpp>


namespace camp
{
/**
 * \brief Error thrown when using an index which is out of bounds
 */
class CAMP_API OutOfRange : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param index Invalid index
     * \param size Allowed size
     */
    OutOfRange(std::size_t index, std::size_t size);

    /**
     * \brief Destructor
     */
    virtual ~OutOfRange() throw();
};

} // namespace camp


#endif // CAMP_OUTOFRANGE_HPP
