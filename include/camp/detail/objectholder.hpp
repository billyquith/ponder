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


#ifndef CAMP_DETAIL_OBJECTHOLDER_HPP
#define CAMP_DETAIL_OBJECTHOLDER_HPP


#include <camp/classget.hpp>
#include <camp/classcast.hpp>


namespace camp
{
namespace detail
{
/**
 * \brief Abstract base class for object holders
 *
 * This class is meant to be used by UserObject.
 * @todo Use an optimized memory pool if there are too many allocations of holders
 */
class AbstractObjectHolder
{
public:

    /**
     * \brief Destructor
     */
    virtual ~AbstractObjectHolder();

    /**
     * \brief Return a typeless pointer to the stored object
     *
     * \return Pointer to the object
     */
    virtual void* object() = 0;

    /**
     * \brief Return a holder which is able to modify its stored object
     *
     * The holder can return itself it it already meets the requirement,
     * otherwise it may return a new holder storing a copy of its object.
     *
     * \return Holder storing a writable object
     */
    virtual AbstractObjectHolder* getWritable() = 0;

protected:

    /**
     * \brief Default constructor
     */
    AbstractObjectHolder();
};

/**
 * \brief Typed specialization of AbstractObjectHolder for storage by const reference
 */
template <typename T>
class ObjectHolderByConstRef : public AbstractObjectHolder
{
public:

    /**
     * \brief Construct the holder from a const object
     *
     * \param object Pointer to the object to store
     */
    ObjectHolderByConstRef(const T* object);

    /**
     * \brief Return a typeless pointer to the stored object
     *
     * \return Pointer to the object
     */
    virtual void* object();

    /**
     * \brief Return a holder which is able to modify its stored object
     *
     * The holder can return itself it it already meets the requirement,
     * otherwise it may return a new holder storing a copy of its object.
     *
     * \return Holder storing a writable object
     */
    virtual AbstractObjectHolder* getWritable();

private:

    const T* m_object; ///< Pointer to the object
    void* m_alignedPtr; ///< Pointer to the actual derived part of the object (may be different than m_object in case of multiple inheritance with offset)
};

/**
 * \brief Typed specialization of AbstractObjectHolder for storage by reference
 */
template <typename T>
class ObjectHolderByRef : public AbstractObjectHolder
{
public:

    /**
     * \brief Construct the holder from an object
     *
     * \param object Pointer to the object to store
     */
    ObjectHolderByRef(T* object);

    /**
     * \brief Return a typeless pointer to the stored object
     *
     * \return Pointer to the object
     */
    virtual void* object();

    /**
     * \brief Return a holder which is able to modify its stored object
     *
     * The holder can return itself it it already meets the requirement,
     * otherwise it may return a new holder storing a copy of its object.
     *
     * \return Holder storing a writable object
     */
    virtual AbstractObjectHolder* getWritable();

private:

    T* m_object; ///< Pointer to the object
    void* m_alignedPtr; ///< Pointer to the actual derived part of the object (may be different than m_object in case of multiple inheritance with offset)
};

/**
 * \brief Typed specialization of AbstractObjectHolder for storage by copy
 */
template <typename T>
class ObjectHolderByCopy : public AbstractObjectHolder
{
public:

    /**
     * \brief Construct the holder from an object
     *
     * \param object Object to store
     */
    ObjectHolderByCopy(const T* object);

    /**
     * \brief Return a typeless pointer to the stored object
     *
     * \return Pointer to the object
     */
    virtual void* object();

    /**
     * \brief Return a holder which is able to modify its stored object
     *
     * The holder can return itself it it already meets the requirement,
     * otherwise it may return a new holder storing a copy of its object.
     *
     * \return Holder storing a writable object
     */
    virtual AbstractObjectHolder* getWritable();

private:

    T m_object; ///< Copy of the object
};

} // namespace detail

} // namespace camp

#include <camp/detail/objectholder.inl>


#endif // CAMP_DETAIL_OBJECTHOLDER_HPP
