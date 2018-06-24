/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2018 Nick Trout.
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

#include <ponder/detail/observernotifier.hpp>
#include <ponder/observer.hpp>
#include <cassert>

namespace ponder {
namespace detail {
    
void ObserverNotifier::addObserver(Observer* observer)
{
    assert(observer != nullptr);

    m_observers.insert(observer);
}

void ObserverNotifier::removeObserver(Observer* observer)
{
    m_observers.erase(observer);
}

ObserverNotifier::ObserverNotifier()
{
}

void ObserverNotifier::notifyClassAdded(const Class& theClass)
{
    for (ObserverSet::iterator it = m_observers.begin(); it != m_observers.end(); ++it)
    {
        (*it)->classAdded(theClass);
    }
}

void ObserverNotifier::notifyClassRemoved(const Class& theClass)
{
    for (ObserverSet::iterator it = m_observers.begin(); it != m_observers.end(); ++it)
    {
        (*it)->classRemoved(theClass);
    }
}

void ObserverNotifier::notifyEnumAdded(const Enum& theEnum)
{
    for (ObserverSet::iterator it = m_observers.begin(); it != m_observers.end(); ++it)
    {
        (*it)->enumAdded(theEnum);
    }
}

void ObserverNotifier::notifyEnumRemoved(const Enum& theEnum)
{
    for (ObserverSet::iterator it = m_observers.begin(); it != m_observers.end(); ++it)
    {
        (*it)->enumRemoved(theEnum);
    }
}

} // namespace details
} // namespace ponder
