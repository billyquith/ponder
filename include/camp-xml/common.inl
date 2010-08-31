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

namespace camp
{
namespace xml
{
namespace detail
{
//-------------------------------------------------------------------------------------------------
template <typename Proxy>
void serialize(const UserObject& object, typename Proxy::NodeType node, const Value& exclude)
{
    // Iterate over the object's properties using its metaclass
    const Class& metaclass = object.getClass();
    for (std::size_t i = 0; i < metaclass.propertyCount(); ++i)
    {
        const Property& property = metaclass.property(i);

        // If the property has the exclude tag, ignore it
        if ((exclude != Value::nothing) && property.hasTag(exclude))
            continue;

        // Create a child node for the new property
        typename Proxy::NodeType child = Proxy::addChild(node, property.name());
        if (!Proxy::isValid(child))
            continue;

        if (property.type() == userType)
        {
            // The current property is a composed type: serialize it recursively
            serialize<Proxy>(property.get(object).to<UserObject>(), child, exclude);
        }
        else if (property.type() == arrayType)
        {
            // The current property is an array
            const ArrayProperty& arrayProperty = static_cast<const ArrayProperty&>(property);

            // Iterate over the array elements
            std::size_t count = arrayProperty.size(object);
            for (std::size_t j = 0; j < count; ++j)
            {
                // Add a new XML node for each array element
                typename Proxy::NodeType item = Proxy::addChild(child, "item");
                if (Proxy::isValid(item))
                {
                    if (arrayProperty.elementType() == userType)
                    {
                        // The array elements are composed objects: serialize them recursively
                        serialize<Proxy>(arrayProperty.get(object, j).to<UserObject>(), item, exclude);
                    }
                    else
                    {
                        // The array elements are simple properties: write them as the text of their XML node
                        Proxy::setText(item, arrayProperty.get(object, j));
                    }
                }
            }
        }
        else
        {
            // The current property is a simple property: write its value as the node's text
            Proxy::setText(child, property.get(object));
        }
    }
}

//-------------------------------------------------------------------------------------------------
template <typename Proxy>
void deserialize(const UserObject& object, typename Proxy::NodeType node, const Value& exclude)
{
    // Iterate over the object's properties using its metaclass
    const Class& metaclass = object.getClass();
    for (std::size_t i = 0; i < metaclass.propertyCount(); ++i)
    {
        const Property& property = metaclass.property(i);

        // If the property has the exclude tag, ignore it
        if ((exclude != Value::nothing) && property.hasTag(exclude))
            continue;

        // Find the child node corresponding to the new property
        typename Proxy::NodeType child = Proxy::findFirstChild(node, property.name());
        if (!Proxy::isValid(child))
            continue;

        if (property.type() == userType)
        {
            // The current property is a composed type: deserialize it recursively
            deserialize<Proxy>(property.get(object).to<UserObject>(), child, exclude);
        }
        else if (property.type() == arrayType)
        {
            // The current property is an array
            const ArrayProperty& arrayProperty = static_cast<const ArrayProperty&>(property);

            // Iterate over the child XML node and extract all the array elements
            std::size_t index = 0;
            for (typename Proxy::NodeType item = Proxy::findFirstChild(child, "item")
                ; Proxy::isValid(item)
                ; item = Proxy::findNextSibling(item, "item"))
            {
                // Make sure that there are enough elements in the array
                std::size_t count = arrayProperty.size(object);
                if (index >= count)
                {
                    if (arrayProperty.dynamic())
                        arrayProperty.resize(object, index + 1);
                    else
                        break;
                }

                if (arrayProperty.elementType() == userType)
                {
                    // The array elements are composed objects: deserialize them recursively
                    deserialize<Proxy>(arrayProperty.get(object, index).to<UserObject>(), item, exclude);
                }
                else
                {
                    // The array elements are simple properties: read their value from the text of their XML node
                    arrayProperty.set(object, index, Proxy::getText(item));
                }

                index++;
            }
        }
        else
        {
            // The current property is a simple property: read its value from the node's text
            property.set(object, Proxy::getText(child));
        }
    }
}

} // namespace detail

} // namespace xml

} // namespace camp
