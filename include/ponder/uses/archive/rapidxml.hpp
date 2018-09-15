/****************************************************************************
**
** This file is part of the Ponder library.
**
** The MIT License (MIT)
**
** Copyright (C) 2015-2018 Nick Trout.
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
    using node_t = rapidxml::xml_node<ch_t> *;

    node_t addChild(node_t parent, const std::string& name)
    {
        const char* nodeName = parent->document()->allocate_string(name.c_str(), name.length()+1);
        node_t child = parent->document()->allocate_node(rapidxml::node_element, nodeName);
        parent->append_node(child);
        return child;
    }
    
    void setText(node_t node, const std::string& text)
    {
        node->value(node->document()->allocate_string(text.c_str(), text.length()+1));
    }

    node_t findFirstChild(node_t node, const std::string& name)
    {
        return node->first_node(name.c_str(), name.length());
    }
    
    node_t findNextSibling(node_t node, const std::string& name)
    {
        return node->next_sibling(name.c_str(), name.length());
    }
    
    detail::string_view getText(node_t node)
    {
        return detail::string_view(node->value(), node->value_size());
    }

    bool isValid(node_t node)
    {
        return node != nullptr;
    }
};

} // namespace archive
} // namespace ponder

#endif // PONDER_ARCHIVE_RAPIDXML_HPP
