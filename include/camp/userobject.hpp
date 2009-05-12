// <insert copyright>

#ifndef CAMP_USEROBJECT_HPP
#define CAMP_USEROBJECT_HPP


#include <camp/classget.hpp>
#include <camp/invalidobject.hpp>
#include <camp/detail/objecttraits.hpp>
#include <camp/detail/objectholder.hpp>
#include <boost/operators.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility/enable_if.hpp>
#include <string>


namespace camp
{
/**
 * \brief Wrapper to manipulate user objects in the CAMP system
 *
 * camp::UserObject is an abstract representation of object instances, which can safely
 * be passed to and manipulated by all the entities in CAMP.
 *
 * \sa EnumObject
 */
class CAMP_API UserObject : boost::totally_ordered<UserObject>
{
public:

    /**
     * \brief Default constructor
     *
     * Constructs an empty / invalid object
     */
    UserObject();

    /**
     * \brief Construct the user object from an instance
     *
     * This constructor is equivalent to calling UserObject::ref(object),
     * i.e. the object is stored by reference.
     *
     * \param object Instance to store in the user object
     */
    template <typename T>
    UserObject(const T& object);

    /**
     * \brief Copy constructor
     *
     * \param copy Instance to copy
     */
    UserObject(const UserObject& copy);

    /**
     * \brief Retrieve the instance stored in the user object
     *
     * The template parameter T is the type to convert the instance to.
     * T must be compatible with the original type of the instance.
     *
     * \return Reference to the instance of the stored object
     *
     * \throw InvalidObject the stored instance is not compatible with T
     */
    template <typename T>
    typename detail::ObjectTraits<T>::RefReturnType get() const;

    /**
     * \brief Retrieve the address of the stored object
     *
     * This function must be used with caution, as the returned address
     * may not be what you expect it to be!
     *
     * \return Pointer to the stored object
     */
    void* pointer() const;

    /**
     * \brief Retrieve the metaclass of the stored instance
     *
     * \return Reference to the instance's metaclass
     */
    const Class& getClass() const;

    /**
     * \brief Operator == to compare equality between two user objects
     *
     * Two user objects are equal if their metaclasses and pointers are both equal
     *
     * \param other User object to compare with this
     *
     * \return True if both user objects are the same, false otherwise
     */
    bool operator==(const UserObject& other) const;

    /**
     * \brief Operator < to compare two user objects
     *
     * \param other User object to compare with this
     *
     * \return True if this < other
     */
    bool operator<(const UserObject& other) const;

public:

    /**
     * \brief Special UserObject instance representing an empty object
     */
    static const UserObject nothing;

    /**
     * \brief Construct a user object from a reference to an object
     *
     * This functions is equivalent to calling UserObject(object).
     *
     * \param object Instance to store in the user object
     *
     * \return UserObject containing a reference to \object
     */
    template <typename T>
    static UserObject ref(const T& object);

    /**
     * \brief Construct a user object with a copy of an object
     *
     * This functions is NOT equivalent to calling UserObject(object).
     *
     * \param object Instance to store in the user object
     *
     * \return UserObject containing a copy of \object
     */
    template <typename T>
    static UserObject copy(const T& object);

private:

    /**
     * \brief Convert a pointer to a target metaclass
     *
     * \param pointer Source pointer to convert
     * \param sourceClass Source metaclass to convert from
     * \param targetClass Target metaclass to convert to
     *
     * \return Converted pointer, or 0 on failure
     */
    static void* convertPtr(void* pointer, const Class& sourceClass, const Class& targetClass);

    boost::shared_ptr<detail::AbstractObjectHolder> m_holder; ///< Abstract holder storing the object
    void* m_pointer; ///< Direct pointer to the stored object
    const Class* m_class; ///< Dynamic metaclass of the object
};

} // namespace camp

#include <camp/userobject.inl>


#endif // CAMP_USEROBJECT_HPP
