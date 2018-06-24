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

#include <ponder/observer.hpp>
#include <ponder/detail/classmanager.hpp>
#include <ponder/detail/enummanager.hpp>

namespace ponder {
    
Observer::~Observer()
{
    // Nothing to do
}

void Observer::classAdded(const Class&)
{
    // Default implementation does nothing
}

void Observer::classRemoved(const Class&)
{
    // Default implementation does nothing
}

void Observer::enumAdded(const Enum&)
{
    // Default implementation does nothing
}

void Observer::enumRemoved(const Enum&)
{
    // Default implementation does nothing
}

Observer::Observer()
{
    // Nothing to do
}

PONDER_API void addObserver(Observer* observer)
{
    detail::ClassManager::instance().addObserver(observer);
    detail::EnumManager::instance().addObserver(observer);
}

PONDER_API void removeObserver(Observer* observer)
{
    detail::ClassManager::instance().removeObserver(observer);
    detail::EnumManager::instance().removeObserver(observer);
}

} // namespace ponder
