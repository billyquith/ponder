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


#ifndef PONDER_CLASSGET_HPP
#define PONDER_CLASSGET_HPP


#include <ponder/config.hpp>
#include <ponder/error.hpp>
#include <ponder/detail/typeid.hpp>
#include <ponder/detail/classmanager.hpp>
#include <ponder/detail/util.hpp>
#include <string>


namespace ponder
{
/**
 * \relates Class
 *
 * \brief Get the total number of existing metaclasses
 *
 * \return Global metaclass count
 */
std::size_t classCount();

/**
 * \relates Class
 *
 * \brief Get a metaclass from its global index
 *
 * The purpose of this function is to provide a way to iterate through
 * all existing metaclasses. \sa classCount
 *
 * \param index Index of the metaclass to get
 *
 * \return Reference to the index-th metaclass
 *
 * \throw OutOfRange index is out of range
 */
const Class& classByIndex(std::size_t index);

/**
 * \relates Class
 *
 * \brief Get a metaclass from its name
 *
 * \param name Name of the metaclass to retrieve (case sensitive)
 *
 * \return Reference to the requested metaclass
 *
 * \throw ClassNotFound name is not a valid metaclass name
 */
const Class& classByName(const std::string& name);

/**
 * \relates Class
 *
 * \brief Get a metaclass from a C++ object
 *
 * \param object object to get the metaclass of
 *
 * \return Reference to the etaclass bound to type T
 *
 * \throw ClassNotFound no metaclass has been declared for T or any of its bases
 */
template <typename T>
const Class& classByObject(const T& object);

/**
 * \relates Class
 *
 * \brief Get a metaclass from its C++ type
 *
 * \return Reference to the metaclass bound to type T
 *
 * \throw ClassNotFound no metaclass has been declared for T
 */
template <typename T>
const Class& classByType();

/**
 * \relates Class
 *
 * \brief Get a metaclass from its C++ type
 *
 * \return Pointer to the metaclass bound to type T, or null pointer if no metaclass has been declared
 */
template <typename T>
const Class* classByTypeSafe();

} // namespace ponder

#include <ponder/classget.inl>


#endif // PONDER_CLASSGET_HPP
