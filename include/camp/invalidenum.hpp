// <insert copyright>

#ifndef CAMP_INVALIDENUM_HPP
#define CAMP_INVALIDENUM_HPP


#include <camp/error.hpp>


namespace camp
{
/**
 * \brief Error thrown when requesting a metaenum which doesn't exist
 */
class InvalidEnum : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param enumName Name of the requested metaenum
     */
    CAMP_API InvalidEnum(const char* enumName) throw();

    /**
     * \brief Copy constructor
     *
     * \param copy Instance to copy
     */
    CAMP_API InvalidEnum(const InvalidEnum& copy) throw();

    /**
     * \brief Destructor
     */
    CAMP_API virtual ~InvalidEnum() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    CAMP_API virtual const char* what() const throw();

    /**
     * \brief Return the requested metaenum name
     *
     * \return Name of the metaenum
     */
    CAMP_API const char* enumName() const throw();

private:

    char m_enumName[256]; ///< Name of the requested metaenum
};

} // namespace camp


#endif // CAMP_INVALIDENUM_HPP
