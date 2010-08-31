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


#ifndef CAMP_CONSTRUCTOR_HPP
#define CAMP_CONSTRUCTOR_HPP


#include <camp/config.hpp>
#include <camp/args.hpp>
#include <vector>


namespace camp
{
class Value;
class UserObject;

/**
 * \brief camp::Constructor represents a metaconstructor which is used to create objects instances from metaclasses
 *
 * This class is an interface which has to be derived to implement typed constructors.
 *
 * \sa Property, Function
 */
class Constructor
{
public:

    /**
     * \brief Destructor
     */
    virtual ~Constructor() {}

    /**
     * \brief Check if the constructor matches the given set of arguments
     *
     * \param args Set of arguments to check
     *
     * \return True if the constructor is compatible with the given arguments
     */
    virtual bool matches(const Args& args) const = 0;

    /**
     * \brief Use the constructor to create a new object
     *
     * \param args Set of arguments to pass to the constructor
     *
     * \return Pointer to the new object wrapped in a UserObject, or UserObject::nothing on failure
     */
    virtual UserObject create(const Args& args) const = 0;
};

} // namespace camp


#endif // CAMP_CONSTRUCTOR_HPP
