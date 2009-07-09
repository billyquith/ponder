/****************************************************************************
**
** Copyright (C) 2009 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** Commercial Usage
** Licensees holding valid CAMP Commercial licenses may use this file in
** accordance with the CAMP Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and TECHNOGERMA Systems France.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3.txt included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at sales@technogerma.fr.
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
T* Class::construct(const Args& args) const
{
    void* object = 0;

    // Search an args match among the list of available constructors
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

    if (!object)
    {
        // Error: no constructor found for the given list of arguments
        // @todo CAMP_ERROR(InvalidConstructor(args));
        return 0;
    }

    // Get the metaclass of T, if any
    const Class* targetClass = classByTypeSafe<T>();

    // Apply the proper offset in case T is a base of this class
    if (!targetClass || applyOffset(object, *targetClass))
    {
        return static_cast<T*>(object);
    }
    else
    {
        // Error: T is unrelated to this class
        // @todo CAMP_ERROR(InvalidConstructor(args));
        return 0;
    }
}

//-------------------------------------------------------------------------------------------------
template <typename T>
void Class::destroy(const T* object) const
{
    delete object;
}

} // namespace camp
