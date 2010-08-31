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

#ifndef CAMP_XML_RAPIDXML_HPP
#define CAMP_XML_RAPIDXML_HPP

#include <camp-xml/common.hpp>
#include <rapidxml.hpp>

namespace camp
{
namespace xml
{
namespace detail
{
/**
 * \brief Proxy that adapts the camp::xml functions to the rapidxml library
 */
struct RapidXml
{
    typedef rapidxml::xml_node<>* NodeType;

    static NodeType addChild(NodeType node, const std::string& name)
    {
        const char* allocatedName = node->document()->allocate_string(name.c_str());
        NodeType child = node->document()->allocate_node(rapidxml::node_element, allocatedName);
        node->append_node(child);
        return child;
    }

    static void setText(NodeType node, const std::string& text)
    {
        node->value(node->document()->allocate_string(text.c_str()));
    }

    static NodeType findFirstChild(NodeType node, const std::string& name)
    {
        return node->first_node(name.c_str());
    }

    static NodeType findNextSibling(NodeType node, const std::string& name)
    {
        return node->next_sibling(name.c_str());
    }

    static std::string getText(NodeType node)
    {
        return node->value();
    }

    static bool isValid(NodeType node)
    {
        return node != 0;
    }
};

} // namespace detail

/**
 * \brief Serialize a CAMP object into a rapidxml xml_node<>
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
inline void serialize(const UserObject& object, rapidxml::xml_node<>* node, const Value& exclude = Value::nothing)
{
    detail::serialize<detail::RapidXml>(object, node, exclude);
}

/**
 * \brief Deserialize a CAMP object from a TinyXml TiXmlElement
 *
 * This function iterates over all the object's properties
 * and read their value from XML nodes. Composed sub-objects
 * are deserialized recursively.
 *
 * You have the possibility to exclude some properties from
 * being read with the last (optional) parameter, \a exclude.
 * If it is defined, any property containing this value as a tag
 * will be excluded from the deserialization process. It is empty
 * by default, which means that no property will be excluded.
 *
 * \param object Object to fill with deserialized information
 * \param node XML node to parse
 * \param exclude Tag to exclude from the deserialization process
 */
inline void deserialize(const UserObject& object, rapidxml::xml_node<>* node, const Value& exclude = Value::nothing)
{
    detail::deserialize<detail::RapidXml>(object, node, exclude);
}

} // namespace xml

} // namespace camp

#endif // CAMP_XML_RAPIDXML_HPP
