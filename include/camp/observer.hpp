/****************************************************************************
**
** Copyright (C) 2009 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** Commercial Usage
** Licensees holding valid CAMP Commercial licenses may use this file in
** accordance with the CAMP Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and TECHNOGERMA Systems France.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3.txt included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at sales@technogerma.fr.
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
