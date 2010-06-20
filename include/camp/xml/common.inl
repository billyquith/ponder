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

        if (property.type() == userType)
        {
            // The current property is a composed type:
            // create a new XML node and serialize it recursively
            typename Proxy::NodeType child = Proxy::addChild(node, property.name());
            serialize<Proxy>(property.get(object).to<UserObject>(), child, exclude);
        }
        else if (property.type() == arrayType)
        {
            // The current property is an array
            const ArrayProperty& arrayProperty = static_cast<const ArrayProperty&>(property);

            typename Proxy::NodeType child = Proxy::addChild(node, property.name());

            // Iterate over the array elements
            std::size_t count = arrayProperty.size(object);
            for (std::size_t j = 0; j < count; ++j)
            {
                // Add a new XML node for each element
                typename Proxy::NodeType item = Proxy::addChild(child, "item");
                if (arrayProperty.elementType() == userType)
                {
                    // The array elements are composed objects: serialize it recursively
                    serialize<Proxy>(arrayProperty.get(object, j).to<UserObject>(), item, exclude);
                }
                else
                {
                    // The array elements are simple properties:
                    // write them as the text of their XML node
                    Proxy::setText(item, arrayProperty.get(object, j));
                }
            }
        }
        else
        {
            // The current property is a simple property:
            // create a new XML node for it and write its value as the node's text
            typename Proxy::NodeType child = Proxy::addChild(node, property.name());
            Proxy::setText(child, property.get(object));
        }
    }
}

//-------------------------------------------------------------------------------------------------
template <typename Proxy>
void deserialize(const UserObject& object, typename Proxy::NodeType node, const Value& exclude)
{

}

} // namespace detail

} // namespace xml

} // namespace camp
