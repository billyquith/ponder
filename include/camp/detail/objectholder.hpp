// <insert copyright>

#ifndef CAMP_DETAIL_OBJECTHOLDER_HPP
#define CAMP_DETAIL_OBJECTHOLDER_HPP


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
    virtual ~AbstractObjectHolder() {}

    /**
     * \brief Return a typeless pointer to the stored object
     *
     * \return Pointer to the object
     */
    virtual void* object() = 0;

protected:

    /**
     * \brief Default constructor
     */
    AbstractObjectHolder() {}
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
     * \brief Return a typeless pointer to the stored object
     *
     * \return Pointer to the object
     */
    virtual void* object();

private:

    T* m_object; ///< Pointer to the object
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
     * \brief Return a typeless pointer to the stored object
     *
     * \return Pointer to the object
     */
    virtual void* object();

private:

    T m_object; ///< Copy of the object
};

} // namespace detail

} // namespace camp

#include <camp/detail/objectholder.inl>


#endif // CAMP_DETAIL_OBJECTHOLDER_HPP
