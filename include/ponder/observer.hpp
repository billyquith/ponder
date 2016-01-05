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


#ifndef PONDER_OBSERVER_HPP
#define PONDER_OBSERVER_HPP


#include <ponder/config.hpp>


namespace ponder
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
class PONDER_API Observer
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
PONDER_API void addObserver(Observer* observer);

/**
 * \brief Unregister an observer
 *
 * \param observer Pointer to the observer instance to unregister
 */
PONDER_API void removeObserver(Observer* observer);

} // namespace ponder


#endif // PONDER_OBSERVER_HPP
