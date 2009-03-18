// <insert copyright>

#ifndef CAMP_ARRAYPROPERTY_HPP
#define CAMP_ARRAYPROPERTY_HPP


#include <camp/property.hpp>


namespace camp
{
/**
 * \brief Specialized type of property for arrays
 *
 */
class ArrayProperty : public Property
{
public:

    /**
     * \brief Construct the property from its description
     *
     * \param name Name of the property
     * \param elementType Type of the property
     * \param dynamic Tells if the array is dynamic or not
     */
    CAMP_API ArrayProperty(const std::string& name, Type elementType, bool dynamic);

    /**
     * \brief Destructor
     */
    CAMP_API virtual ~ArrayProperty();

    /**
     * \brief Get the type of the array elements
     *
     * \return Type of elements
     */
    CAMP_API Type elementType() const;

    /**
     * \brief Check if the array is dynamic
     *
     * If the array is not dynamic, the insert and remove functions
     * are disabled.
     *
     * \return True if the array is dynamic, false otherwise
     */
    CAMP_API bool dynamic() const;

    /**
     * \brief Get the current size of the array
     *
     * \param object Object
     *
     * \return Number of elements in the array
     *
     * \throw InvalidObject object has an invalid value
     * \throw InvalidAccess property is not readable
     */
    CAMP_API std::size_t size(const UserObject& object) const;

    /**
     * \brief Get an element of the array for a given object
     *
     * \param object Object
     * \param index Index of the element to get
     *
     * \return Value of the index-th element
     *
     * \throw InvalidObject object has an invalid value
     * \throw InvalidAccess property is not readable
     * \throw InvalidIndex index is out of range
     */
    CAMP_API Value get(const UserObject& object, std::size_t index) const;

    /**
     * \brief Set an element of the array for a given object
     *
     * \param object Object
     * \param index Index of the element to get
     * \param value New value to assign to the element
     *
     * \throw InvalidObject object has an invalid value
     * \throw InvalidAccess property is not writable
     * \throw InvalidValue value can't be converted to the property's type
     * \throw InvalidIndex index is out of range
     */
    CAMP_API void set(const UserObject& object, std::size_t index, const Value& value) const;

    /**
     * \brief Insert a new element into the array
     *
     * This function will throw an error if the array is not dynamic
     *
     * \param object Object
     * \param before Index before which the new element will be inserted
     * \param value Value of the element to insert
     *
     * \throw InvalidObject object has an invalid value
     * \throw InvalidAccess array is not writable or not dynamic
     * \throw InvalidValue value can't be converted to the property's type
     * \throw InvalidIndex index is out of range
     */
    CAMP_API void insert(const UserObject& object, std::size_t before, const Value& value) const;

    /**
     * \brief Remove an element from the array
     *
     * This function will throw an error if the array is not dynamic
     *
     * \param object Object
     * \param index Index of the element to remove
     *
     * \throw InvalidObject object has an invalid value
     * \throw InvalidAccess array is not writable or not dynamic
     * \throw InvalidIndex index is out of range
     */
    CAMP_API void remove(const UserObject& object, std::size_t index) const;

    /**
     * \brief Accept the visitation of a ClassVisitor
     *
     * \param visitor Visitor to accept
     */
    CAMP_API virtual void accept(ClassVisitor& visitor) const;

protected:

    /**
     * \see Property::getValue
     */
    CAMP_API virtual Value getValue(const UserObject& object) const;

    /**
     * \see Property::setValue
     */
    CAMP_API virtual void setValue(const UserObject& object, const Value& value) const;

    /**
     * \brief Do the actual retrieval of the size
     *
     * \param object Object
     *
     * \return Size of the array
     */
    CAMP_API virtual std::size_t getSize(const UserObject& object) const = 0;

    /**
     * \brief Do the actual reading of an element
     *
     * This function is a pure virtual which has to be implemented in derived classes
     *
     * \param object Object
     * \param index Index of the element to get
     *
     * \return Value of the index-th element
     */
    CAMP_API virtual Value getElement(const UserObject& object, std::size_t index) const = 0;

    /**
     * \brief Do the actual writing of an element
     *
     * This function is a pure virtual which has to be implemented in derived classes
     *
     * \param object Object
     * \param index Index of the element to get
     * \param value New value to assign to the element
     */
    CAMP_API virtual void setElement(const UserObject& object, std::size_t index, const Value& value) const = 0;

    /**
     * \brief Do the actual insertion of an element
     *
     * This function is a pure virtual which has to be implemented in derived classes
     *
     * \param object Object
     * \param before Index before which the new element will be inserted
     * \param value Value of the element to insert
     */
    CAMP_API virtual void insertElement(const UserObject& object, std::size_t before, const Value& value) const = 0;

    /**
     * \brief Do the actual removal of an element
     *
     * This function is a pure virtual which has to be implemented in derived classes
     *
     * \param object Object
     * \param index Index of the element to remove
     */
    CAMP_API virtual void removeElement(const UserObject& object, std::size_t index) const = 0;

private:

    Type m_elementType; ///< Type of the individual elements of the array
    bool m_dynamic; ///< Is the array dynamic?
};

} // namespace camp


#endif // CAMP_ARRAYPROPERTY_HPP
