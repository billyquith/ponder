// <insert copyright>

#ifndef CAMP_INVALIDENUMVALUE_HPP
#define CAMP_INVALIDENUMVALUE_HPP


#include <camp/error.hpp>


namespace camp
{
class Enum;

/**
 * \brief Error thrown when requesting the value of a pair which doesn't exist in a metaenum
 */
class InvalidEnumValue : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param value Value of the requested pair
     * \param ownerEnum Owner metaenum
     */
    CAMP_API InvalidEnumValue(long value, const Enum& ownerEnum) throw();

    /**
     * \brief Destructor
     */
    CAMP_API virtual ~InvalidEnumValue() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    CAMP_API virtual const char* what() const throw();

    /**
     * \brief Return the requested pair value
     *
     * \return Value of the requested pair
     */
    CAMP_API long value() const throw();

    /**
     * \brief Return the owner metaenum
     *
     * \return Reference to the owner metaenum
     */
    CAMP_API const Enum& ownerEnum() const throw();

private:

    long m_value; ///< Value of the requested pair
    const Enum* m_ownerEnum; ///< Owner metaenum
};

} // namespace camp


#endif // CAMP_INVALIDENUMVALUE_HPP
