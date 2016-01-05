/****************************************************************************
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
** 
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
** 
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
**
****************************************************************************/


#ifndef PONDER_DETAIL_OBSERVERNOTIFIER_HPP
#define PONDER_DETAIL_OBSERVERNOTIFIER_HPP


#include <ponder/config.hpp>
#include <set>


namespace ponder
{
class Observer;
class Class;
class Enum;

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

    /**
     * \brief Default constructor
     */
    ObserverNotifier();

    /**
     * \brief Notify all the registered observers of a class creation
     *
     * \param theClass Class that have been added
     */
    void notifyClassAdded(const Class& theClass);

    /**
     * \brief Notify all the registered observers of a class removal
     *
     * \param theClass Class that have been removed
     */
    void notifyClassRemoved(const Class& theClass);

    /**
     * \brief Notify all the registered observers of an enum creation
     *
     * \param theEnum Enum that have been added
     */
    void notifyEnumAdded(const Enum& theEnum);

    /**
     * \brief Notify all the registered observers of an enum removal
     *
     * \param theEnum Enum that have been removed
     */
    void notifyEnumRemoved(const Enum& theEnum);

private:

    typedef std::set<Observer*> ObserverSet;

    ObserverSet m_observers; ///< Sequence of registered observers
};

} // namespace detail

} // namespace ponder


#endif // PONDER_DETAIL_OBSERVERNOTIFIER_HPP
