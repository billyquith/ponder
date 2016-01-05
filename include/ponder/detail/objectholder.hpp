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


#ifndef PONDER_DETAIL_OBJECTHOLDER_HPP
#define PONDER_DETAIL_OBJECTHOLDER_HPP


#include <ponder/classget.hpp>
#include <ponder/classcast.hpp>


namespace ponder
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
    virtual void* object() override;

    /**
     * \brief Return a holder which is able to modify its stored object
     *
     * The holder can return itself it it already meets the requirement,
     * otherwise it may return a new holder storing a copy of its object.
     *
     * \return Holder storing a writable object
     */
    virtual AbstractObjectHolder* getWritable() override;

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
    virtual void* object() override;

    /**
     * \brief Return a holder which is able to modify its stored object
     *
     * The holder can return itself it it already meets the requirement,
     * otherwise it may return a new holder storing a copy of its object.
     *
     * \return Holder storing a writable object
     */
    virtual AbstractObjectHolder* getWritable() override;

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
    virtual void* object() override;

    /**
     * \brief Return a holder which is able to modify its stored object
     *
     * The holder can return itself it it already meets the requirement,
     * otherwise it may return a new holder storing a copy of its object.
     *
     * \return Holder storing a writable object
     */
    virtual AbstractObjectHolder* getWritable() override;

private:

    T m_object; ///< Copy of the object
};

} // namespace detail

} // namespace ponder

#include <ponder/detail/objectholder.inl>


#endif // PONDER_DETAIL_OBJECTHOLDER_HPP
