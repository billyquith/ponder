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

#ifndef CAMP_XML_XERCES_HPP
#define CAMP_XML_XERCES_HPP

#include <camp/xml/common.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>

namespace camp
{
namespace xml
{
namespace detail
{
/**
 * \brief Proxy that adapts the camp::xml functions to the Xerces library
 */
struct Xerces
{
    typedef xercesc::DOMElement* NodeType;

    static NodeType addChild(NodeType node, const std::string& name)
    {
        XMLCh buffer[256];
        xercesc::XMLString::transcode(name.c_str(), buffer, sizeof(buffer));

        NodeType child = node->getOwnerDocument()->createElement(buffer);
        node->appendChild(child);

        return child;
    }

    static void setText(NodeType node, const std::string& text)
    {
        XMLCh buffer[256];
        xercesc::XMLString::transcode(text.c_str(), buffer, sizeof(buffer));

        node->setTextContent(buffer);
    }
};

} // namespace detail

/**
 * \brief Serialize a CAMP object into a Xerces DOMElement
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
inline void serialize(const UserObject& object, xercesc::DOMElement* node, const Value& exclude = Value::nothing)
{
    detail::serialize<detail::Xerces>(object, node, exclude);
}
} // namespace xml

} // namespace camp

#endif // CAMP_XML_TINYXML_HPP
