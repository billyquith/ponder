// <insert copyright>

#ifndef CAMP_ENUMOBJECT_HPP
#define CAMP_ENUMOBJECT_HPP


#include <camp/config.hpp>
#include <camp/enumget.hpp>
#include <boost/type_traits.hpp>
#include <boost/operators.hpp>
#include <boost/utility/enable_if.hpp>
#include <string>


namespace camp
{
/**
 * \brief Wrapper to manipulate enumerated values in the CAMP system
 *
 * camp::EnumObject is an abstract representation of enum values, and supports
 * conversions from strings and integers.
 *
 * \sa UserObject
 */
class EnumObject : boost::totally_ordered<EnumObject>
{
public:

    /**
     * \brief Construct the enum object from an enumerated value
     *
     * \param value Value to store in the enum object
     */
    template <typename T>
    EnumObject(T value, typename boost::enable_if<boost::is_enum<T> >::type* = 0);

    /**
     * \brief Get the value of the enum object
     *
     * \return Integer value of the enum object
     */
    CAMP_API long value() const;

    /**
     * \brief Get the name of the enum object
     *
     * \return String containing the name of the enum object
     */
    CAMP_API const std::string& name() const;

    /**
     * \brief Retrieve the metaenum of the stored enum object
     *
     * \return Reference to the object's metaenum
     */
    CAMP_API const Enum& getEnum() const;

    /**
     * \brief Operator == to compare equality between two enum objects
     *
     * Two enum objects are equal if their metaenums and values are both equal
     *
     * \param other Enum object to compare with this
     *
     * \return True if both enum objects are the same, false otherwise
     */
    CAMP_API bool operator==(const EnumObject& other) const;

    /**
     * \brief Operator < to compare two enum objects
     *
     * \param other Enum object to compare with this
     *
     * \return True if this < other
     */
    CAMP_API bool operator<(const EnumObject& other) const;

private:

    long m_value; ///< Value
    const Enum* m_enum; ///< Metaenum associated to the value
};

} // namespace camp

#include <camp/enumobject.inl>


#endif // CAMP_ENUMOBJECT_HPP
