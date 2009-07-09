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
** contact the sales department at contact@technogerma.fr.
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
