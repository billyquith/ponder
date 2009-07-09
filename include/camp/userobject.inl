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
UserObject::UserObject(const T& object)
    : m_class(&classByObject(object))
    , m_holder()
    , m_parent()
    , m_child(0)
{
    typedef detail::ObjectTraits<T&> Traits;
    typedef detail::ObjectHolderByRef<typename Traits::DataType> Holder;

    m_holder.reset(new Holder(Traits::getPointer(const_cast<T&>(object))));
}

//-------------------------------------------------------------------------------------------------
template <typename T>
typename detail::ObjectTraits<T>::RefReturnType UserObject::get() const
{
    // Make sure we have a valid internal object
    void* ptr = pointer();
    if (!ptr)
        CAMP_ERROR(InvalidObject(*this));

    // Get the metaclass of T (we use classByTypeSafe because it may not exist)
    const Class* targetClass = classByTypeSafe<T>();
    if (!targetClass)
        CAMP_ERROR(InvalidObject(*this));

    // Apply the proper offset to the pointer (solves multiple inheritance issues)
    ptr = classCast(ptr, *m_class, *targetClass);

    // Check if the conversion was successful (i.e. if T is related to the actual metaclass of the object)
    if (!ptr)
        CAMP_ERROR(InvalidObject(*this));

    return detail::ObjectTraits<T>::get(ptr);
}

//-------------------------------------------------------------------------------------------------
template <typename T>
UserObject UserObject::ref(T& object)
{
    return UserObject(object);
}

//-------------------------------------------------------------------------------------------------
template <typename T>
UserObject UserObject::ref(const T& object)
{
    typedef detail::ObjectTraits<const T&> Traits;
    typedef detail::ObjectHolderByConstRef<typename Traits::DataType> Holder;

    UserObject userObject;
    userObject.m_class = &classByObject(object);
    userObject.m_holder.reset(new Holder(Traits::getPointer(object)));

    return userObject;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
UserObject UserObject::copy(const T& object)
{
    typedef detail::ObjectTraits<const T&> Traits;
    typedef detail::ObjectHolderByCopy<typename Traits::DataType> Holder;

    UserObject userObject;
    userObject.m_class = &classByType<T>();
    userObject.m_holder.reset(new Holder(Traits::getPointer(object)));

    return userObject;
}

} // namespace camp
