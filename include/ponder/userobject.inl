/****************************************************************************
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
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


namespace ponder
{
template <typename T>
UserObject::UserObject(const T& object)
    : m_class(&classByObject(object))
    , m_holder()
    , m_parent()
    , m_child(nullptr)
{
    typedef detail::ObjectTraits<T&> Traits;
    typedef detail::ObjectHolderByRef<typename Traits::DataType> Holder;

    m_holder.reset(new Holder(Traits::getPointer(const_cast<T&>(object))));
}

template <typename T>
typename detail::ObjectTraits<T>::RefReturnType UserObject::get() const
{
    // Make sure that we have a valid internal object
    void* ptr = pointer();
    if (!ptr)
        PONDER_ERROR(NullObject(m_class));

    // Get the metaclass of T (we use classByTypeSafe because it may not exist)
    const Class* targetClass = classByTypeSafe<T>();
    if (!targetClass)
        PONDER_ERROR(ClassNotFound("unknown"));

    // Apply the proper offset to the pointer (solves multiple inheritance issues)
    ptr = classCast(ptr, *m_class, *targetClass);

    return detail::ObjectTraits<T>::get(ptr);
}

template <typename T>
UserObject UserObject::ref(T& object)
{
    return UserObject(object);
}

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

} // namespace ponder
