/****************************************************************************
**
** This file is part of the Ponder library.
**
** The MIT License (MIT)
**
** Copyright (C) 2015-2020 Nick Trout.
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
#ifndef PONDER_ARCHIVE_RAPIDJSON_HPP
#define PONDER_ARCHIVE_RAPIDJSON_HPP

#include <ponder/class.hpp>
#define RAPIDJSON_HAS_STDSTRING 1
#include <rapidjson/rapidjson.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>

namespace ponder {
namespace archive {

/**
 * \brief An archive that uses JSON format as storage.
 *
 * The [RapidJSON](http://rapidjson.org/) library is used for JSON parsing and formatting.
 */
template <typename ARCHIVE>
class RapidJsonArchiveWriter
{
    ARCHIVE& m_archive;
    int m_arrayLevel{ 0 };

public:
    
    struct JsonNode {};    
    using Node = JsonNode*;

    RapidJsonArchiveWriter(ARCHIVE& archive) : m_archive(archive) {}
    
    Node beginChild(Node parent, const std::string& name)
    {
        m_archive.Key(name);
        m_archive.StartObject();
        return Node();
    }

    void endChild(Node parent, Node child)
    {
        m_archive.EndObject();
    }

    void setProperty(Node node, const std::string& name, const std::string& text)
    {
        if (m_arrayLevel == 0)
            m_archive.Key(name);
        m_archive.String(text);
    }

    Node beginArray(Node parent, const std::string& name)
    {
        m_archive.Key(name);
        m_archive.StartArray();
        ++m_arrayLevel;
        return parent;
    }

    void endArray(Node /*parent*/, Node /*child*/)
    {
        m_archive.EndArray();
        --m_arrayLevel;
    }

    detail::string_view getValue(Node node)
    {
        return detail::string_view();
    }

    bool isValid(Node node)
    {
        return node != nullptr;
    }
};

class RapidJsonArchiveReader
{
    rapidjson::Document& m_archive;

public:

    struct Node {
        const rapidjson::Value& m_value;
        Node(const rapidjson::Value& value) : m_value(value) {}
    };

    struct ArrayIterator
    {
        const rapidjson::Value& m_value;
        rapidjson::Value::ConstValueIterator m_iter;

        bool isEnd() const { return m_iter == m_value.End(); }
        void next() { ++m_iter; }
        Node getItem() { return Node(*m_iter); }
    };

    RapidJsonArchiveReader(rapidjson::Document& archive) : m_archive(archive) {}

    Node findProperty(Node node, const std::string& name)
    {
        const rapidjson::Value& val{ node.m_value[name] };
        return Node(val);
    }

    ArrayIterator createArrayIterator(Node node, const std::string& name)
    {
        return ArrayIterator({ node.m_value, node.m_value.Begin() });
    }

    detail::string_view getValue(Node node)
    {
        return detail::string_view(node.m_value.GetString(), node.m_value.GetStringLength());
    }

    bool isValid(Node node)
    {
        return true;
    }
};

} // namespace archive
} // namespace ponder

#endif // PONDER_ARCHIVE_RAPIDJSON_HPP
