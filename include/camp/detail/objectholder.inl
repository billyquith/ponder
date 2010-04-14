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
namespace detail
{
//-------------------------------------------------------------------------------------------------
inline AbstractObjectHolder::~AbstractObjectHolder()
{
}

//-------------------------------------------------------------------------------------------------
inline AbstractObjectHolder::AbstractObjectHolder()
{
}

//-------------------------------------------------------------------------------------------------
template <typename T>
ObjectHolderByConstRef<T>::ObjectHolderByConstRef(const T* object)
    : m_object(object)
    , m_alignedPtr(classCast(const_cast<T*>(object), classByType<T>(), classByObject(object)))
{
}

//-------------------------------------------------------------------------------------------------
template <typename T>
void* ObjectHolderByConstRef<T>::object()
{
    return m_alignedPtr;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
AbstractObjectHolder* ObjectHolderByConstRef<T>::getWritable()
{
    // We hold a read-only object: return a holder which stores a copy of it
    return new ObjectHolderByCopy<T>(m_object);
}

//-------------------------------------------------------------------------------------------------
template <typename T>
ObjectHolderByRef<T>::ObjectHolderByRef(T* object)
    : m_object(object)
    , m_alignedPtr(classCast(object, classByType<T>(), classByObject(object)))
{
}

//-------------------------------------------------------------------------------------------------
template <typename T>
void* ObjectHolderByRef<T>::object()
{
    return m_alignedPtr;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
AbstractObjectHolder* ObjectHolderByRef<T>::getWritable()
{
    // We already store a modifiable object
    return this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
ObjectHolderByCopy<T>::ObjectHolderByCopy(const T* object)
    : m_object(*object)
{
}

//-------------------------------------------------------------------------------------------------
template <typename T>
void* ObjectHolderByCopy<T>::object()
{
    return &m_object;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
AbstractObjectHolder* ObjectHolderByCopy<T>::getWritable()
{
    // We already store a modifiable object
    return this;
}

} // namespace detail

} // namespace camp
