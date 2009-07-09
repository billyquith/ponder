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
