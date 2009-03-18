// <insert copyright>

#ifndef CAMP_INVALIDVALUE_HPP
#define CAMP_INVALIDVALUE_HPP


#include <camp/error.hpp>
#include <camp/type.hpp>


namespace camp
{
/**
 * \brief Error thrown when using an invalid value
 *
 * This error may be thrown when a value can't be converted to the requested type
 */
class InvalidValue : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param sourceType Type of the source value
     * \param requestedType Type the value is being converted to
     */
    CAMP_API InvalidValue(Type sourceType, Type requestedType) throw();

    /**
     * \brief Destructor
     */
    CAMP_API virtual ~InvalidValue() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    CAMP_API virtual const char* what() const throw();

    /**
     * \brief Return the type of the source value
     *
     * \return Source type
     */
    CAMP_API Type sourceType() const throw();

    /**
     * \brief Return the type the value is being converted to
     *
     * \return Requested type
     */
    CAMP_API Type requestedType() const throw();

private:

    Type m_sourceType; ///< Type of the source value
    Type m_requestedType; ///< Type the value is being converted to
};

} // namespace camp


#endif // CAMP_INVALIDVALUE_HPP
