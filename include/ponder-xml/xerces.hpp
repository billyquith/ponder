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

#ifndef PONDER_XML_XERCES_HPP
#define PONDER_XML_XERCES_HPP

#include <ponder-xml/common.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>

namespace ponder
{
namespace xml
{
namespace detail
{
/**
 * \brief Proxy that adapts the ponder::xml functions to the Xerces library
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

    static NodeType findFirstChild(NodeType node, const std::string& name)
    {
        XMLCh buffer[256];
        xercesc::XMLString::transcode(name.c_str(), buffer, sizeof(buffer));

        xercesc::DOMNodeList* children = node->getElementsByTagName(buffer);
        if (children && (children->getLength() > 0))
            return static_cast<NodeType>(children->item(0));
        else
            return 0;
    }

    static NodeType findNextSibling(NodeType node, const std::string& name)
    {
        XMLCh buffer[256];
        xercesc::XMLString::transcode(name.c_str(), buffer, sizeof(buffer));

        xercesc::DOMNode* sibling = node->getNextSibling();
        while (sibling)
        {
            bool same = xercesc::XMLString::compareString(sibling->getNodeName(), buffer) == 0;
            if ((sibling->getNodeType() == xercesc::DOMNode::ELEMENT_NODE) && same)
                return static_cast<NodeType>(sibling);
            sibling = sibling->getNextSibling();
        }

        return 0;
    }

    static std::string getText(NodeType node)
    {
        const XMLCh* text = node->getTextContent();

        char buffer[256];
        xercesc::XMLString::transcode(text, buffer, sizeof(buffer) - 1);

        return buffer;
    }

    static bool isValid(NodeType node)
    {
        return node != 0;
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

/**
 * \brief Deserialize a CAMP object from a Xerces DOMElement
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
inline void deserialize(const UserObject& object, xercesc::DOMElement* node, const Value& exclude = Value::nothing)
{
    detail::deserialize<detail::Xerces>(object, node, exclude);
}


} // namespace xml

} // namespace ponder

#endif // PONDER_XML_TINYXML_HPP
