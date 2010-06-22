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

#ifndef CAMP_XML_STRING_HPP
#define CAMP_XML_STRING_HPP

#include <camp-xml/common.hpp>
#include <camp/class.hpp>
#include <string>
#include <list>

namespace camp
{
namespace xml
{
namespace detail
{
/**
 * \brief Proxy that adapts the camp::xml functions to raw std::strings
 */
struct String
{
    struct Node
    {
        std::string name;
        std::string text;
        std::list<Node> children;

        Node(const std::string& nodeName) : name(nodeName)
        {
        }

        std::string output(int indentation = 0) const
        {
            std::string prefix(indentation, ' ');

            if (text.empty() && children.empty())
            {
                // Node is empty (no children, no text)
                return prefix + "<" + name + " />\n";
            }
            else if (text.empty())
            {
                // Node contains children
                std::string result = prefix + "<" + name + ">\n";
                for (std::list<Node>::const_iterator it = children.begin(); it != children.end(); ++it)
                {
                    result += it->output(indentation + 2);
                }
                result += prefix + "</" + name + ">\n";
                return result;
            }
            else
            {
                // Node contains text
                return prefix + "<" + name + ">" + text + "</" + name + ">\n";
            }
        }
    };

    typedef Node* NodeType;

    static NodeType addChild(NodeType node, const std::string& name)
    {
        node->children.push_back(Node(name));
        return &node->children.back();
    }

    static void setText(NodeType node, const std::string& text)
    {
        node->text = text;
    }
};

} // namespace detail

/**
 * \brief Serialize a CAMP object into a raw string
 *
 * This function iterates over all the object's properties
 * and transforms them into valid XML nodes. Composed sub-objects
 * are serialized recursively.
 *
 * \a output is the variable to fill with the result of the
 * serialization process. The resulting text will be a valid,
 * human-readable set of XML tags.
 *
 * You have the possibility to exclude some properties from the
 * generated output with the last (optional) parameter, \a exclude.
 * If it is defined, any property containing this value as a tag
 * will be excluded from the serialization process. It is empty
 * by default, which means that no property will be excluded.
 *
 * \param object Object to serialize
 * \param output String that will be filled with the generated output
 * \param exclude Tag to exclude from the serialization process
 */
inline void serialize(const UserObject& object, std::string& output, const Value& exclude = Value::nothing)
{
    detail::String::Node node(object.getClass().name());
    detail::serialize<detail::String>(object, &node, exclude);
    output = node.output();
}
} // namespace xml

} // namespace camp

#endif // CAMP_XML_STRING_HPP
