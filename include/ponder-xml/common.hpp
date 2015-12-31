/****************************************************************************
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
**
** This file is part of the CAMP library.
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

#ifndef PONDER_XML_COMMON_HPP
#define PONDER_XML_COMMON_HPP

#include <ponder/userobject.hpp>
#include <ponder/value.hpp>
#include <ponder/arrayproperty.hpp>
#include <string>

namespace ponder
{
namespace xml
{
namespace detail
{
/**
 * \brief Serialize a CAMP object into XML elements
 *
 * This function uses a proxy class that defines a unified
 * interface for creating and populating XML nodes. To
 * provide support for a particular XML library, you just
 * have to provide a specialized Proxy which maps the
 * unified interface to the library's API, and call this
 * function.
 *
 * \param object Object to serialize
 * \param node Parent for the generated XML nodes
 * \param exclude Tag to exclude from the serialization process
 */
template <typename Proxy>
void serialize(const UserObject& object, typename Proxy::NodeType node, const Value& exclude);

/**
 * \brief Deserialize a CAMP object from XML elements
 *
 * This function uses a proxy class that defines a unified
 * interface for reading and iterating over XML nodes. To
 * provide support for a particular XML library, you just
 * have to provide a specialized Proxy which maps the
 * unified interface to the library's API, and call this
 * function.
 *
 * \param object Object to serialize
 * \param node XML node to parse
 * \param exclude Tag to exclude from the deserialization process
 */
template <typename Proxy>
void deserialize(const UserObject& object, typename Proxy::NodeType node, const Value& exclude);

} // namespace detail

} // namespace xml

} // namespace ponder

#include <ponder-xml/common.inl>

#endif // PONDER_XML_COMMON_HPP
