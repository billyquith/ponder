// <insert copyright>

#ifndef CAMP_DETAIL_OBJECTHOLDER_HPP
#define CAMP_DETAIL_OBJECTHOLDER_HPP


#include <camp/classget.hpp>
#include <camp/classcast.hpp>


namespace camp
{
class UserObject;

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
     * \brief Clone the instance
     *
     * \return New holder which is a copy of this
     */
    virtual AbstractObjectHolder* clone() const = 0;

    /**
     * \brief Return a typeless pointer to the stored object
     *
     * \return Pointer to the object
     */
    virtual void* object() = 0;

    /**
     * \brief Update the stored object
     */
    virtual void updateObject();

    /**
     * \brief Retrieve the metaclass of the stored object
     *
     * \return Object metaclass
     */
    virtual const Class& getClass() const = 0;

protected:

    /**
     * \brief Default constructor
     */
    AbstractObjectHolder();
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
     */
    ObjectHolderByRef(const T* object);

    /**
     * \brief Clone the instance
     *
     * \return New holder which is a copy of this
     */
    virtual AbstractObjectHolder* clone() const;

    /**
     * \brief Return a typeless pointer to the stored object
     *
     * \return Pointer to the object
     */
    virtual void* object();

    /**
     * \brief Retrieve the metaclass of the stored object
     *
     * \return Object metaclass
     */
    virtual const Class& getClass() const;

private:

    T* m_object; ///< Pointer to the object
    const Class* m_class; ///< Metaclass of the object
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
     */
    ObjectHolderByCopy(const T* object);

    /**
     * \brief Clone the instance
     *
     * \return New holder which is a copy of this
     */
    virtual AbstractObjectHolder* clone() const;

    /**
     * \brief Return a typeless pointer to the stored object
     *
     * \return Pointer to the object
     */
    virtual void* object();

    /**
     * \brief Retrieve the metaclass of the stored object
     *
     * \return Object metaclass
     */
    virtual const Class& getClass() const;

private:

    T m_object; ///< Copy of the object
    const Class* m_class; ///< Metaclass of the object
};

} // namespace detail

} // namespace camp

#include <camp/detail/objectholder.inl>


#endif // CAMP_DETAIL_OBJECTHOLDER_HPP
