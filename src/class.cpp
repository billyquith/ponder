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


#include <camp/class.hpp>
#include <camp/errors.hpp>
#include <camp/detail/classmanager.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
void Class::undeclare(const std::string& name)
{
    detail::ClassManager::instance().removeClass(name);
}

//-------------------------------------------------------------------------------------------------
const std::string& Class::name() const
{
    return m_name;
}

//-------------------------------------------------------------------------------------------------
std::size_t Class::baseCount() const
{
    return m_bases.size();
}

//-------------------------------------------------------------------------------------------------
const Class& Class::base(std::size_t index) const
{
    // Make sure that the index is not out of range
    if (index >= m_bases.size())
        CAMP_ERROR(OutOfRange(index, m_bases.size()));

    return *m_bases[index].base;
}

//-------------------------------------------------------------------------------------------------
std::size_t Class::functionCount() const
{
    return m_functions.size();
}

//-------------------------------------------------------------------------------------------------
bool Class::hasFunction(const std::string& name) const
{
    const FunctionNameIndex& names = m_functions.get<Name>();

    return names.find(name) != names.end();
}

//-------------------------------------------------------------------------------------------------
const Function& Class::function(std::size_t index) const
{
    // Make sure that the index is not out of range
    if (index >= m_functions.size())
        CAMP_ERROR(OutOfRange(index, m_functions.size()));

    return *m_functions[index];
}

//-------------------------------------------------------------------------------------------------
const Function& Class::function(const std::string& name) const
{
    const FunctionNameIndex& names = m_functions.get<Name>();

    FunctionNameIndex::const_iterator it = names.find(name);
    if (it == names.end())
        CAMP_ERROR(FunctionNotFound(name, m_name));

    return **it;
}

//-------------------------------------------------------------------------------------------------
std::size_t Class::propertyCount() const
{
    return m_properties.size();
}

//-------------------------------------------------------------------------------------------------
bool Class::hasProperty(const std::string& name) const
{
    const PropertyNameIndex& names = m_properties.get<Name>();

    return names.find(name) != names.end();
}

//-------------------------------------------------------------------------------------------------
const Property& Class::property(std::size_t index) const
{
    // Make sure that the index is not out of range
    if (index >= m_properties.size())
        CAMP_ERROR(OutOfRange(index, m_properties.size()));

    return *m_properties[index];
}

//-------------------------------------------------------------------------------------------------
const Property& Class::property(const std::string& name) const
{
    const PropertyNameIndex& names = m_properties.get<Name>();

    PropertyNameIndex::const_iterator it = names.find(name);
    if (it == names.end())
        CAMP_ERROR(PropertyNotFound(name, m_name));

    return **it;
}

//-------------------------------------------------------------------------------------------------
void Class::visit(ClassVisitor& visitor) const
{
    // First visit properties
    for (PropertyTable::const_iterator it = m_properties.begin(); it != m_properties.end(); ++it)
    {
        (*it)->accept(visitor);
    }

    // Then visit functions
    for (FunctionTable::const_iterator it = m_functions.begin(); it != m_functions.end(); ++it)
    {
        (*it)->accept(visitor);
    }
}

//-------------------------------------------------------------------------------------------------
void* Class::applyOffset(void* pointer, const Class& target) const
{
    // Special case for null pointers: don't apply offset to leave them null
    if (!pointer)
        return pointer;

    // Check target as a base class of this
    int offset = baseOffset(target);
    if (offset != -1)
        return static_cast<void*>(static_cast<char*>(pointer) + offset);

    // Check target as a derived class of this
    offset = target.baseOffset(*this);
    if (offset != -1)
        return static_cast<void*>(static_cast<char*>(pointer) - offset);

    // No match found, target is not a base class nor a derived class of this
    CAMP_ERROR(ClassUnrelated(name(), target.name()));
}

//-------------------------------------------------------------------------------------------------
bool Class::operator==(const Class& other) const
{
    return m_name == other.m_name;
}

//-------------------------------------------------------------------------------------------------
bool Class::operator!=(const Class& other) const
{
    return m_name != other.m_name;
}

//-------------------------------------------------------------------------------------------------
Class::Class(const std::string& name)
    : m_name(name)
{
}

//-------------------------------------------------------------------------------------------------
int Class::baseOffset(const Class& base) const
{
    // Check self
    if (&base == this)
        return 0;

    // Search base in the base classes
    BaseList::const_iterator end = m_bases.end();
    for (BaseList::const_iterator it = m_bases.begin(); it != end; ++it)
    {
        int offset = it->base->baseOffset(base);
        if (offset != -1)
            return offset + it->offset;
    }

    return -1;
}

} // namespace camp
