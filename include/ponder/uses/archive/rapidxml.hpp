/****************************************************************************
**
** This file is part of the Ponder library.
**
** The MIT License (MIT)
**
** Copyright (C) 2015-2020 Nick Trout.
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

#pragma once
#ifndef PONDER_ARCHIVE_RAPIDXML_HPP
#define PONDER_ARCHIVE_RAPIDXML_HPP

#include <rapidxml/rapidxml.hpp>
#include <ponder/detail/string_view.hpp>

namespace ponder {
namespace archive {

/**
 * \brief Archive used to read/write using XML.
 *
 * The [RapidXML](http://rapidxml.sourceforge.net) library is used to parse and
 * create and XML DOM.
 */
template <typename CH = char>
class RapidXmlArchive
{
public:

    using ch_t = CH;
    using Node = rapidxml::xml_node<ch_t>*;

    //! Facilitate iteration over XML elements/arrays.
    class ArrayIterator
    {
        Node m_node{};
        detail::string_view m_name;

    public:

        ArrayIterator(Node arrayNode, detail::string_view name)
            : m_name(name)
        {
            m_node = arrayNode->first_node(name.data(), name.length());
        }

        bool isEnd() const { return m_node == nullptr; }
        void next()
        {
            m_node = m_node->next_sibling(m_name.data(), m_name.length());
        }
        Node getItem() { return m_node; }
    };

    // Write

    Node beginChild(Node parent, const std::string& name)
    {
        const char* nodeName = parent->document()->allocate_string(name.c_str(), name.length()+1);
        Node child = parent->document()->allocate_node(rapidxml::node_element, nodeName);
        parent->append_node(child);
        return child;
    }

    void endChild(Node /*parent*/, Node /*child*/) {}

    void setProperty(Node parent, const std::string& name, const std::string& text)
    {
        const char* nodeName = parent->document()->allocate_string(name.c_str(), name.length() + 1);
        Node child = parent->document()->allocate_node(rapidxml::node_element, nodeName);
        parent->append_node(child);
        child->value(child->document()->allocate_string(text.c_str(), text.length() + 1));
    }

    Node beginArray(Node parent, const std::string& name)
    {
        return beginChild(parent, name);
    }

    void endArray(Node /*parent*/, Node /*child*/) {}

    // Read

    Node findProperty(Node node, const std::string& name)
    {
        return node->first_node(name.c_str(), name.length());
    }

    ArrayIterator createArrayIterator(Node node, const std::string& name)
    {
        return ArrayIterator(node, detail::string_view(name.c_str(), name.length()));
    }
    
    detail::string_view getValue(Node node)
    {
        return detail::string_view(node->value(), node->value_size());
    }

    bool isValid(Node node)
    {
        return node != nullptr;
    }
};

} // namespace archive
} // namespace ponder

#endif // PONDER_ARCHIVE_RAPIDXML_HPP
