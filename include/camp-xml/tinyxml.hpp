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

#ifndef CAMP_XML_TINYXML_HPP
#define CAMP_XML_TINYXML_HPP

#include <camp-xml/common.hpp>
#include <tinyxml.h>

namespace camp
{
namespace xml
{
namespace detail
{
/**
 * \brief Proxy that adapts the camp::xml functions to the TinyXml library
 */
struct TinyXml
{
    typedef TiXmlElement* NodeType;

    static NodeType addChild(NodeType node, const std::string& name)
    {
        return static_cast<NodeType>(node->InsertEndChild(TiXmlElement(name.c_str())));
    }

    static void setText(NodeType node, const std::string& text)
    {
        node->InsertEndChild(TiXmlText(text.c_str()));
    }
};

} // namespace detail

/**
 * \brief Serialize a CAMP object into a TinyXml TiXmlElement
 *
 * This function iterates over all the object's properties
 * and transforms them into valid XML nodes. Composed sub-objects
 * are serialized recursively.
 *
 * \a node is the parent node to which the generated child nodes
 * will be appended.
 *
 * You have the possibility to exclude some properties from the
 * generated output with the last (optional) parameter, \a exclude.
 * If it is defined, any property containing this value as a tag
 * will be excluded from the serialization process. It is empty
 * by default, which means that no property will be excluded.
 *
 * \param object Object to serialize
 * \param node Parent for the generated XML nodes
 * \param exclude Tag to exclude from the serialization process
 */
inline void serialize(const UserObject& object, TiXmlElement* node, const Value& exclude = Value::nothing)
{
    detail::serialize<detail::TinyXml>(object, node, exclude);
}
} // namespace xml

} // namespace camp

#endif // CAMP_XML_TINYXML_HPP
