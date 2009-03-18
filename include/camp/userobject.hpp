// <insert copyright>

#ifndef CAMP_USEROBJECT_HPP
#define CAMP_USEROBJECT_HPP


#include <camp/classget.hpp>
#include <camp/invalidobject.hpp>
#include <camp/detail/objecttraits.hpp>
#include <boost/operators.hpp>
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
class UserObject : boost::totally_ordered<UserObject>
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
     * \param object Instance to store in the user object
     */
    template <typename T>
    UserObject(const T& object);

    /**
     * \brief Copy constructor
     *
     * \param copy Instance to copy
     */
    CAMP_API UserObject(const UserObject& copy);

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
    CAMP_API void* pointer() const;

    /**
     * \brief Retrieve the metaclass of the stored instance
     *
     * \return Reference to the instance's metaclass
     */
    CAMP_API const Class& getClass() const;

    /**
     * \brief Operator == to compare equality between two user objects
     *
     * Two user objects are equal if their metaclasses and pointers are both equal
     *
     * \param other User object to compare with this
     *
     * \return True if both user objects are the same, false otherwise
     */
    CAMP_API bool operator==(const UserObject& other) const;

    /**
     * \brief Operator < to compare two user objects
     *
     * \param other User object to compare with this
     *
     * \return True if this < other
     */
    CAMP_API bool operator<(const UserObject& other) const;

public:

    /**
     * \brief Special UserObject instance representing an empty object
     */
    CAMP_API static const UserObject nothing;

private:

    /**
     * \brief Convert a pointer to a target metaclass
     *
     * \param pointer Source pointer to convert
     * \param targetClass Target metaclass to convert to
     *
     * \return Converted pointer, or 0 on failure
     */
    CAMP_API void* convertPtr(void* pointer, const Class& targetClass) const;

    void* m_object; ///< Address of the stored instance
    const Class* m_class; ///< Metaclass of the instance
};

} // namespace camp

#include <camp/userobject.inl>


#endif // CAMP_USEROBJECT_HPP
