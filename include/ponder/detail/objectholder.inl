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
namespace detail
{
inline AbstractObjectHolder::~AbstractObjectHolder()
{
}

inline AbstractObjectHolder::AbstractObjectHolder()
{
}

template <typename T>
ObjectHolderByConstRef<T>::ObjectHolderByConstRef(const T* object)
    : m_object(object)
    , m_alignedPtr(classCast(const_cast<T*>(object), classByType<T>(), classByObject(object)))
{
}

template <typename T>
void* ObjectHolderByConstRef<T>::object()
{
    return m_alignedPtr;
}

template <typename T>
AbstractObjectHolder* ObjectHolderByConstRef<T>::getWritable()
{
    // We hold a read-only object: return a holder which stores a copy of it
    return new ObjectHolderByCopy<T>(m_object);
}

template <typename T>
ObjectHolderByRef<T>::ObjectHolderByRef(T* object)
    : m_object(object)
    , m_alignedPtr(classCast(object, classByType<T>(), classByObject(object)))
{
}

template <typename T>
void* ObjectHolderByRef<T>::object()
{
    return m_alignedPtr;
}

template <typename T>
AbstractObjectHolder* ObjectHolderByRef<T>::getWritable()
{
    // We already store a modifiable object
    return this;
}

template <typename T>
ObjectHolderByCopy<T>::ObjectHolderByCopy(const T* object)
    : m_object(*object)
{
}

template <typename T>
void* ObjectHolderByCopy<T>::object()
{
    return &m_object;
}

template <typename T>
AbstractObjectHolder* ObjectHolderByCopy<T>::getWritable()
{
    // We already store a modifiable object
    return this;
}

} // namespace detail

} // namespace ponder
