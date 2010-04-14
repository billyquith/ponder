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


#ifndef CAMP_OBSERVER_HPP
#define CAMP_OBSERVER_HPP


#include <camp/config.hpp>


namespace camp
{
class Class;
class Enum;

/**
 * \brief Receives notification about creation / destruction of metaclasses and metaenums
 *
 * This class is a base class which must be derived in order to create custom observers.
 * None of the virtual functions is pure, so you can only override the one you're interested in.
 *
 * \sa Class, Enum
 */
class CAMP_API Observer
{
public:

    /**
     * \brief Destructor
     */
    virtual ~Observer();

    /**
     * \brief Functon called when a new metaclass is created
     *
     * \param added Metaclass that have been added
     */
    virtual void classAdded(const Class& added);

    /**
     * \brief Functon called when an existing metaclass is destroyed
     *
     * \param removed Metaclass that have been destroyed
     */
    virtual void classRemoved(const Class& removed);

    /**
     * \brief Functon called when a new metaenum is created
     *
     * \param added Metaenum that have been added
     */
    virtual void enumAdded(const Enum& added);

    /**
     * \brief Functon called when an existing metaenum is destroyed
     *
     * \param removed Metaenum that have been destroyed
     */
    virtual void enumRemoved(const Enum& removed);

protected:

    /**
     * \brief Default constructor
     */
    Observer();
};

/**
 * \brief Register an observer
 *
 * \param observer Pointer to the observer instance to register
 */
CAMP_API void addObserver(Observer* observer);

/**
 * \brief Unregister an observer
 *
 * \param observer Pointer to the observer instance to unregister
 */
CAMP_API void removeObserver(Observer* observer);

} // namespace camp


#endif // CAMP_OBSERVER_HPP
