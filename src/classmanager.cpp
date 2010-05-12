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


#include <camp/detail/classmanager.hpp>
#include <camp/class.hpp>
#include <camp/classnotfound.hpp>
#include <camp/classalreadycreated.hpp>
#include <camp/outofrange.hpp>
#include <camp/observer.hpp>


namespace camp
{
namespace detail
{
//-------------------------------------------------------------------------------------------------
ClassManager& ClassManager::instance()
{
    static ClassManager manager;
    return manager;
}

//-------------------------------------------------------------------------------------------------
Class& ClassManager::registerNew(const std::string& name, const std::string& id)
{
    const IdIndex&   ids   = m_classes.get<Id>();
    const NameIndex& names = m_classes.get<Name>();

    // First make sure that the class doesn't already exist
    if ((ids.find(id) != ids.end()) || (names.find(name) != names.end()))
        CAMP_ERROR(ClassAlreadyCreated(name, id));

    // Create the new class
    boost::shared_ptr<Class> newClass(new Class(name));

    // Insert it into the table
    ClassInfo info;
    info.id = id;
    info.name = name;
    info.classPtr = newClass;
    m_classes.insert(info);

    // Notify observers
    ObserverIterator end = observersEnd();
    for (ObserverIterator it = observersBegin(); it != end; ++it)
    {
        (*it)->classAdded(*newClass);
    }

    return *newClass;
}

//-------------------------------------------------------------------------------------------------
void ClassManager::unregister(const std::string& name)
{
    NameIndex& names = m_classes.get<Name>();

    // Make sure that the metaclass exists
    NameIndex::iterator it = names.find(name);
    if (it == names.end())
        CAMP_ERROR(ClassNotFound(name));

    // Remove it from the table
    names.erase(it);
}

//-------------------------------------------------------------------------------------------------
std::size_t ClassManager::count() const
{
    return m_classes.size();
}

//-------------------------------------------------------------------------------------------------
const Class& ClassManager::getByIndex(std::size_t index) const
{
    // Make sure that the index is not out of range
    if (index >= m_classes.size())
        CAMP_ERROR(OutOfRange(index, m_classes.size()));

    ClassTable::const_iterator it = m_classes.begin();
    std::advance(it, index);

    return *it->classPtr;
}

//-------------------------------------------------------------------------------------------------
const Class& ClassManager::getByName(const std::string& name) const
{
    const NameIndex& names = m_classes.get<Name>();

    NameIndex::const_iterator it = names.find(name);
    if (it == names.end())
        CAMP_ERROR(ClassNotFound(name));

    return *it->classPtr;
}

//-------------------------------------------------------------------------------------------------
const Class& ClassManager::getById(const std::string& id) const
{
    const IdIndex& ids = m_classes.get<Id>();

    IdIndex::const_iterator it = ids.find(id);
    if (it == ids.end())
        CAMP_ERROR(ClassNotFound(id));

    return *it->classPtr;
}

//-------------------------------------------------------------------------------------------------
const Class* ClassManager::getByIdSafe(const std::string& id) const
{
    const IdIndex& ids = m_classes.get<Id>();

    IdIndex::const_iterator it = ids.find(id);
    if (it == ids.end())
        return 0;

    return &*it->classPtr;
}

//-------------------------------------------------------------------------------------------------
bool ClassManager::classExists(const std::string& id) const
{
    const IdIndex& ids = m_classes.get<Id>();

    return ids.find(id) != ids.end();
}

//-------------------------------------------------------------------------------------------------
ClassManager::ClassManager()
{
}

//-------------------------------------------------------------------------------------------------
ClassManager::~ClassManager()
{
    // Notify observers of classes destruction
    ObserverIterator begin = observersBegin();
    ObserverIterator end   = observersEnd();
    for (ClassTable::const_iterator itClass = m_classes.begin(); itClass != m_classes.end(); ++itClass)
    {
        for (ObserverIterator it = begin; it != end; ++it)
        {
            (*it)->classRemoved(*itClass->classPtr);
        }
    }
}

} // namespace detail

} // namespace camp
