/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2018 Nick Trout.
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

namespace ponder {
    
template <typename T>
UserObject::UserObject(const T& object)
    :   m_class(&classByType<T>())
{
    typedef detail::ReferenceTraits<const T> Traits;
    typedef detail::ObjectHolderByCopy<typename Traits::DataType> Holder;
    m_holder.reset(new Holder(Traits::getPointer(object)));
}

template <typename T>
UserObject::UserObject(T* object)
    :   m_class(&classByType<T>())
{
    typedef detail::ReferenceTraits<T> Traits;
    static_assert(!Traits::isRef, "Cannot make reference to reference");
    
    typedef typename std::conditional<std::is_const<T>::value,
    detail::ObjectHolderByConstRef<typename Traits::DataType>,
    detail::ObjectHolderByRef<typename Traits::DataType>>::type Holder;
    
    m_holder.reset(new Holder(object));
}

template <typename T>
typename detail::ReferenceTraits<T>::ReferenceType UserObject::get() const
{
    // Make sure that we have a valid internal object
    void *ptr = pointer();
    if (!ptr)
        PONDER_ERROR(NullObject(m_class));

    // Get the metaclass of T (we use classByTypeSafe because it may not exist)
    const Class *targetClass = classByTypeSafe<T>();
    if (!targetClass)
        PONDER_ERROR(ClassNotFound("unknown"));

    // Apply the proper offset to the pointer (solves multiple inheritance issues)
    ptr = classCast(ptr, *m_class, *targetClass);

    return detail::ReferenceTraits<T>::get(ptr);
}

template <typename T>
UserObject UserObject::makeRef(T& object)
{
    typedef detail::ReferenceTraits<T> RefTraits;
    static_assert(!RefTraits::isRef, "Cannot make reference to reference");

    typedef typename std::conditional<std::is_const<T>::value,
                 detail::ObjectHolderByConstRef<typename RefTraits::DataType>,
                 detail::ObjectHolderByRef<typename RefTraits::DataType>>::type Holder;

    return UserObject(&classByObject(object), new Holder(RefTraits::getPointer(object)));
}

template <typename T>
inline UserObject UserObject::makeRef(T* object)
{
    return makeRef(*object);
}

template <typename T>
UserObject UserObject::makeCopy(const T& object)
{
    typedef detail::ReferenceTraits<const T> Traits;
    typedef detail::ObjectHolderByCopy<typename Traits::DataType> Holder;
    return UserObject(&classByType<T>(), new Holder(Traits::getPointer(object)));
}

template <typename T>
inline T& UserObject::ref() const
{
    return *reinterpret_cast<T*>(m_holder->object());
}

template <typename T>
inline const T& UserObject::cref() const
{
    return *reinterpret_cast<T*>(m_holder->object());
}

} // namespace ponder
