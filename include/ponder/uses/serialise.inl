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

namespace ponder {
namespace archive {

template <class ARCHIVE>
void ArchiveWriter<ARCHIVE>::write(NodeType parent, const UserObject& object)
{
    const Class& metaclass = object.getClass();

    // Iterate over the object's properties using its metaclass
    for (std::size_t i = 0; i < metaclass.propertyCount(); ++i)
    {
        const Property& property = metaclass.property(i);
        
        // If the property has the exclude tag, ignore it
        //                if ((exclude != Value::nothing) && property.hasTag(exclude))
        //                    continue;
        
        if (property.kind() == ValueKind::User)
        {
            NodeType child = m_archive.beginChild(parent, property.name());

            // recurse
            write(child, property.get(object).to<UserObject>());

            m_archive.endChild(parent, child);
        }
        else if (property.kind() == ValueKind::Array)
        {
            auto const& arrayProperty = static_cast<const ArrayProperty&>(property);

            NodeType arrayNode = m_archive.beginArray(parent, property.name());

            // Iterate over the array elements
            const std::size_t count = arrayProperty.size(object);
            const std::string itemName("item");
            for (std::size_t j = 0; j < count; ++j)
            {
                if (arrayProperty.elementType() == ValueKind::User)
                {
                    write(arrayNode, arrayProperty.get(object, j).to<UserObject>());
                }
                else
                {
                    m_archive.setProperty(arrayNode, itemName, arrayProperty.get(object, j).to<std::string>());
                }
            }

            m_archive.endArray(parent, arrayNode);
        }
        else
        {
            m_archive.setProperty(parent, property.name(), property.get(object).to<std::string>());
        }
    }
}

template <class ARCHIVE>
void ArchiveReader<ARCHIVE>::read(NodeType node, const UserObject& object)
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
        NodeType child = m_archive.findProperty(node, property.name());
        if (!m_archive.isValid(child))
            continue;
        
        if (property.kind() == ValueKind::User)
        {
            // The current property is a composed type: deserialize it recursively
            read(child, property.get(object).to<UserObject>());
        }
        else if (property.kind() == ValueKind::Array)
        {
            auto const& arrayProperty = static_cast<const ArrayProperty&>(property);

            std::size_t index = 0;
            const std::string itemName("item");
            for (ArrayIterator it{ m_archive.createArrayIterator(child, itemName) }; !it.isEnd(); it.next())
            {
                // Make sure that there are enough elements in the array
                std::size_t count = arrayProperty.size(object);
                if (index >= count)
                {
                    if (!arrayProperty.dynamic())
                        break;
                    arrayProperty.resize(object, index + 1);
                }
                
                if (arrayProperty.elementType() == ValueKind::User)
                {
                    read(it.getItem(), arrayProperty.get(object, index).to<UserObject>());
                }
                else
                {
                    arrayProperty.set(object, index, m_archive.getValue(it.getItem()));
                }
                
                ++index;
            }
        }
        else
        {
            property.set(object, m_archive.getValue(child));
        }
    }
}

    
} // namespace archive
} // namespace ponder
