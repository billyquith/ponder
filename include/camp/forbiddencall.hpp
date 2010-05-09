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


#ifndef CAMP_FORBIDDENCALL_HPP
#define CAMP_FORBIDDENCALL_HPP


#include <camp/error.hpp>


namespace camp
{
/**
 * \brief Error thrown when calling a function that is not callable
 */
class CAMP_API ForbiddenCall : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param functionName Name of the function
     */
    ForbiddenCall(const std::string& functionName);

    /**
     * \brief Destructor
     */
    virtual ~ForbiddenCall() throw();
};

} // namespace camp


#endif // CAMP_FORBIDDENCALL_HPP
