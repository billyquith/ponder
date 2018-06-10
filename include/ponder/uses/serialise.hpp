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

#ifndef PONDER_USES_SERIALISE_HPP
#define PONDER_USES_SERIALISE_HPP

#include <ponder/class.hpp>
#include <ponder/arrayproperty.hpp>

namespace ponder {
namespace archive {
    
    namespace detail {
    } // namespace detail
    
    /*!
     For writing archive requires the following concepts:
     
        class Archive
        {
        public:
            node_t* addChild(node_t* parent, const std::string& name);
            void setText(node_t* node, const std::string& text);
            static bool isValid(node_t* node);
        };
     
     */
    template <class ARCHIVE>
    class ArchiveWriter
    {
    public:
        
        using archive_t = ARCHIVE;
        using node_t = typename archive_t::node_t;
        
        ArchiveWriter(archive_t& archive)
        :   m_archive(archive)
        {}
        
        void write(node_t* parent, const UserObject& object)
        {
            // Iterate over the object's properties using its metaclass
            const Class& metaclass = object.getClass();
            for (std::size_t i = 0; i < metaclass.propertyCount(); ++i)
            {
                const Property& property = metaclass.property(i);
    
                // If the property has the exclude tag, ignore it
//                if ((exclude != Value::nothing) && property.hasTag(exclude))
//                    continue;
    
                // Create a child node for the new property
                node_t* child = m_archive.addChild(parent, property.name());
                if (!archive_t::isValid(child))
                    continue;
    
                if (property.kind() == ValueKind::User)
                {
                    // The current property is a composed type: serialize it recursively
                    write(child, property.get(object).to<UserObject>());
                }
//                else if (property.kind() == ValueKind::Array)
//                {
//                    // The current property is an array
//                    const ArrayProperty& arrayProperty = static_cast<const ArrayProperty&>(property);
//
//                    // Iterate over the array elements
//                    std::size_t count = arrayProperty.size(object);
//                    for (std::size_t j = 0; j < count; ++j)
//                    {
//                        // Add a new XML node for each array element
//                        node_t* item = m_archive.addChild(child, "item");
//                        if (archive_t::isValid(item))
//                        {
//                            if (arrayProperty.elementType() == ValueKind::User)
//                            {
//                                // The array elements are composed objects: serialize them recursively
//                                write(item, arrayProperty.get(object, j).to<UserObject>());
//                            }
//                            else
//                            {
//                                // The array elements are simple properties: write them as the text of their XML node
//                                m_archive.setText(item, arrayProperty.get(object, j).to<std::string>());
//                            }
//                        }
//                    }
//                }
                else
                {
                    // The current property is a simple property: write its value as the node's text
                    m_archive.setText(child, property.get(object).to<std::string>());
                }
            }
        }
        
    private:
        
        archive_t& m_archive;

    };

    /*!
     For reading an archive requires the following concepts:
     
     class Archive
     {
     public:
         node_t* findFirstChild(node_t* node, const std::string& name);
         node_t* findNextSibling(node_t* node, const std::string& name);
         std::string getText(node_t* node)
         static bool isValid(node_t* node);
     };
     
     */
    template <class ARCHIVE>
    class ArchiveReader
    {
    public:
        
        using archive_t = ARCHIVE;
        using node_t = typename archive_t::node_t;
        
        ArchiveReader(archive_t& archive)
        :   m_archive(archive)
        {}
        
        void read(node_t* node, const UserObject& object)
        {
            // Iterate over the object's properties using its metaclass
            const Class& metaclass = object.getClass();
            for (std::size_t i = 0; i < metaclass.propertyCount(); ++i)
            {
                const Property& property = metaclass.property(i);
    
                // If the property has the exclude tag, ignore it
//                if ((exclude != Value::nothing) && property.hasTag(exclude))
//                    continue;
    
                // Find the child node corresponding to the new property
                node_t* child = m_archive.findFirstChild(node, property.name());
                if (!archive_t::isValid(child))
                    continue;
    
                if (property.kind() == ValueKind::User)
                {
                    // The current property is a composed type: deserialize it recursively
                    read(child, property.get(object).to<UserObject>());
                }
//                else if (property.kind() == arrayType)
//                {
//                    // The current property is an array
//                    const ArrayProperty& arrayProperty = static_cast<const ArrayProperty&>(property);
//
//                    // Iterate over the child XML node and extract all the array elements
//                    std::size_t index = 0;
//                    for (typename Proxy::NodeType item = Proxy::findFirstChild(child, "item")
//                         ; Proxy::isValid(item)
//                         ; item = Proxy::findNextSibling(item, "item"))
//                    {
//                        // Make sure that there are enough elements in the array
//                        std::size_t count = arrayProperty.size(object);
//                        if (index >= count)
//                        {
//                            if (arrayProperty.dynamic())
//                                arrayProperty.resize(object, index + 1);
//                            else
//                                break;
//                        }
//
//                        if (arrayProperty.elementType() == userType)
//                        {
//                            // The array elements are composed objects: deserialize them recursively
//                            deserialize<Proxy>(arrayProperty.get(object, index).to<UserObject>(), item, exclude);
//                        }
//                        else
//                        {
//                            // The array elements are simple properties: read their value from the text of their XML node
//                            arrayProperty.set(object, index, Proxy::getText(item));
//                        }
//
//                        index++;
//                    }
//                }
                else
                {
                    // The current property is a simple property: read its value from the node's text
                    property.set(object, m_archive.getText(child));
                }
            }
        }
        
    private:
        
        archive_t& m_archive;
        
    };

} // namespace archive
} // namespace ponder

#endif // PONDER_USES_SERIALISE_HPP
