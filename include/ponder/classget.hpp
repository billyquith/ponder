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

//! \file classget.hpp

#pragma once
#ifndef PONDER_CLASSGET_HPP
#define PONDER_CLASSGET_HPP

#include <ponder/config.hpp>
#include <ponder/error.hpp>
#include <ponder/detail/typeid.hpp>
#include <ponder/detail/classmanager.hpp>
#include <ponder/detail/util.hpp>
#include <string>

namespace ponder {
    
/**
 * \brief Get the total number of existing metaclasses
 *
 * \relates Class
 *
 * \return Global metaclass count
 */
std::size_t classCount();
    
/**
 * \brief Get an iterator that can be used to iterate over all registered classes
 *
 * \relates Class
 * \snippet inspect.cpp classIterator
 *
 * \return Class iterator: `pair<String, Class*>`
 */
const detail::ClassManager& classIterator();

/**
 * \brief Get a metaclass from its name
 *
 * \note Automated registration does not occur when using this lookup call (since we don't
 *       have the object type). Use PONDER_TYPE() registration if you use this.
 *
 * \relates Class
 *
 * \param name Name of the metaclass to retrieve (case sensitive)
 *
 * \return Reference to the requested metaclass
 *
 * \throw ClassNotFound name is not a valid metaclass name
 */
const Class& classByName(IdRef name);

/**
 * \brief Get a metaclass from a C++ object
 *
 * \relates Class
 *
 * \param object object to get the metaclass of
 *
 * \return Reference to the metaclass bound to type T
 *
 * \throw ClassNotFound no metaclass has been declared for T or any of its bases
 */
template <typename T>
const Class& classByObject(const T& object);

/**
 * \brief Get a metaclass from its C++ type
 *
 * \relates Class
 *
 * \return Reference to the metaclass bound to type T
 *
 * \throw ClassNotFound no metaclass has been declared for T
 */
template <typename T>
const Class& classByType();

/**
 * \brief Get a metaclass from its C++ type
 *
 * \relates Class
 *
 * \return Pointer to the metaclass bound to type T, or null pointer if no metaclass has
 *         been declared
 */
template <typename T>
const Class* classByTypeSafe();

} // namespace ponder

#include <ponder/classget.inl>

#endif // PONDER_CLASSGET_HPP
