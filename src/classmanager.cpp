/****************************************************************************
**
** This file is part of the CAMP library.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
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


#include <camp/detail/classmanager.hpp>
#include <camp/class.hpp>
#include <camp/errors.hpp>
#include <camp/observer.hpp>


namespace camp
{
namespace detail
{
//-------------------------------------------------------------------------------------------------
ClassManager& ClassManager::instance()
{
    return inst();
}

//-------------------------------------------------------------------------------------------------
Class& ClassManager::addClass(const std::string& name, const std::string& id)
{
    const IdIndex&   ids   = m_classes.get<Id>();
    const NameIndex& names = m_classes.get<Name>();

    // First make sure that the class doesn't already exist
    if ((ids.find(id) != ids.end()) || (names.find(name) != names.end()))
        CAMP_ERROR(ClassAlreadyCreated(name, id));

    // Create the new class
    std::shared_ptr<Class> newClass(new Class(name));

    // Insert it into the table
    ClassInfo info;
    info.id = id;
    info.name = name;
    info.classPtr = newClass;
    m_classes.insert(info);

    // Notify observers
    notifyClassAdded(*newClass);

    return *newClass;
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
    // Notify observers
    for (ClassTable::const_iterator it = m_classes.begin(); it != m_classes.end(); ++it)
    {
        notifyClassRemoved(*it->classPtr);
    }
}

} // namespace detail

} // namespace camp
