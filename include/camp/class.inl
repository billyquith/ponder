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
//-------------------------------------------------------------------------------------------------
template <typename T>
ClassBuilder<T> Class::declare(const std::string& name)
{
    Class& newClass = detail::ClassManager::instance().registerNew(name, detail::StaticTypeId<T>::get(false));
    return ClassBuilder<T>(newClass);
}

//-------------------------------------------------------------------------------------------------
template <typename T>
void Class::undeclare()
{
    undeclare(classByType<T>().name());
}

//-------------------------------------------------------------------------------------------------
template <typename T>
T* Class::construct(const Args& args) const
{
    void* object = 0;

    // Search an arguments match among the list of available constructors
    ConstructorList::const_iterator end = m_constructors.end();
    for (ConstructorList::const_iterator it = m_constructors.begin();
         (it != end) && !object;
         ++it)
    {
        Constructor& constructor = **it;

        if (constructor.matches(args))
        {
            // Match found: use the constructor to create the new instance
            object = constructor.create(args);
        }
    }

    // Check if we found a constructor matching the provided list of arguments
    if (!object)
        CAMP_ERROR(ConstructorNotFound(name()));

    // Get the metaclass of T (we use classByTypeSafe because it may not exist)
    const Class* targetClass = classByTypeSafe<T>();

    // Apply the proper offset in case T is a base of this class
    if (targetClass)
        object = applyOffset(object, *targetClass);

    return static_cast<T*>(object);
}

//-------------------------------------------------------------------------------------------------
template <typename T>
void Class::destroy(const T* object) const
{
    delete object;
}

} // namespace camp
