

#ifndef CAMP_USEROBJECT_HPP
#define CAMP_USEROBJECT_HPP


#include <camp/classcast.hpp>
#include <camp/invalidobject.hpp>
#include <camp/detail/objecttraits.hpp>
#include <camp/detail/objectholder.hpp>
#include <boost/operators.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/utility/enable_if.hpp>
#include <string>


namespace camp
{
class Property;
class UserProperty;
class Value;
class ParentObject;

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
     * \brief Construct the user object from a parent object and a member property
     *
     * This constructor allows to create user objects that are accessed through a property
     * of another user object, and that cannot be stored in memory.
     *
     * \param parent Parent user object
     * \param member Member of \a parent used to access the actual object
     */
    UserObject(const UserObject& parent, const UserProperty& member);

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
     * \brief Assignment operator
     *
     * \param other User object to assign
     *
     * \return Reference to this
     */
    UserObject& operator=(const UserObject& other);

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
     * \return UserObject containing a reference to \a  object
     */
    template <typename T>
    static UserObject ref(T& object);

    /**
     * \brief Construct a user object from a const reference to an object
     *
     * This functions is NOT equivalent to calling UserObject(object).
     *
     * \param object Instance to store in the user object
     *
     * \return UserObject containing a const reference to \a object
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
     * \return UserObject containing a copy of \a object
     */
    template <typename T>
    static UserObject copy(const T& object);

private:

    friend class Property;

    /**
     * \brief Assign a new value to a property of the object
     *
     * This functions is meant to be used only by Property, and
     * is needed for proper propagation of the modification to the
     * parent objects.
     *
     * \param property Property to modify
     * \param value New value to assign
     */
    void set(const Property& property, const Value& value) const;

    /**
     * \brief Recursively set a property
     *
     * See the description of UserObject::set(property, value).
     *
     * \param object Object to modify
     * \param property Property to set when the recursion is over
     * \param value Value to assign when the recursion is over
     */
    void cascadeSet(const UserObject& object, const Property& property, const Value& value) const;

private:

    const Class* m_class; ///< Metaclass of the stored object
    boost::shared_ptr<detail::AbstractObjectHolder> m_holder; ///< Optional abstract holder storing the object
    boost::scoped_ptr<ParentObject> m_parent; ///< Optional parent object
    const UserObject* m_child; ///< Optional pointer to the child object (m_parent.object.m_child == this)
};


/**
 * \brief Structure holding the informations about a parent object
 *
 * A parent object is composed of a <object, property> pair.
 */
class ParentObject : boost::noncopyable
{
public:

    /**
     * \brief Default constructor
     */
    ParentObject(const UserObject& obj, const UserProperty& mem) : object(obj), member(mem) {}

    UserObject object; ///< Parent object
    const UserProperty& member; ///< Member of the parent giving access to the child object
    UserObject lastValue; ///< Last value of the object (stored here for persistency)
};


} // namespace camp

#include <camp/userobject.inl>


#endif // CAMP_USEROBJECT_HPP
