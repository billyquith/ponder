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
class CAMP_API InvalidValue : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param sourceType Type of the source value
     * \param requestedType Type the value is being converted to
     */
    InvalidValue(Type sourceType, Type requestedType) throw();

    /**
     * \brief Copy constructor
     *
     * \param copy Instance to copy
     */
    InvalidValue(const InvalidValue& copy) throw();

    /**
     * \brief Destructor
     */
    virtual ~InvalidValue() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    virtual const char* what() const throw();

    /**
     * \brief Return the type of the source value
     *
     * \return Source type
     */
    Type sourceType() const throw();

    /**
     * \brief Return the type the value is being converted to
     *
     * \return Requested type
     */
    Type requestedType() const throw();

private:

    Type m_sourceType; ///< Type of the source value
    Type m_requestedType; ///< Type the value is being converted to
};

} // namespace camp


#endif // CAMP_INVALIDVALUE_HPP
