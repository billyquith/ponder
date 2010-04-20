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
