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


#ifndef CAMP_DETAIL_OBSERVERNOTIFIER_HPP
#define CAMP_DETAIL_OBSERVERNOTIFIER_HPP


#include <camp/config.hpp>
#include <set>


namespace camp
{
class Observer;

namespace detail
{
/**
 * \brief Base class for classes that can notify global observers
 */
class ObserverNotifier
{
public:

    /**
     * \brief Register a new observer
     *
     * The new observer will be notified of metaclasses and metaenums creation/removal.
     * This function doesn't take the ownership of the given object,
     * don't forget to destroy it on your side
     *
     * \param observer New observer to register
     */
    void addObserver(Observer* observer);

    /**
     * \brief Unregister an existing observer
     *
     * This function doesn't destroy the object, it just removes it
     * from the list of registered observers.
     *
     * \param observer Observer to unregister
     */
    void removeObserver(Observer* observer);

protected:

    typedef std::set<Observer*> ObserverSet;
    typedef ObserverSet::const_iterator ObserverIterator;

    /**
     * \brief Default constructor
     */
    ObserverNotifier();

    /**
     * \brief Return an iterator on the beginning of the observers sequence
     *
     * \return Iterator on the observers' beginning
     */
    ObserverIterator observersBegin() const;

    /**
     * \brief Return an iterator on the end of the observers sequence
     *
     * \return Iterator on the observers' end
     */
    ObserverIterator observersEnd() const;

private:

    ObserverSet m_observers; ///< Sequence of registered observers
};

} // namespace detail

} // namespace camp


#endif // CAMP_DETAIL_OBSERVERNOTIFIER_HPP
