// <insert copyright>

#ifndef CAMP_INVALIDACCESS_HPP
#define CAMP_INVALIDACCESS_HPP


#include <camp/error.hpp>


namespace camp
{
/**
 * \brief Error thrown when doing an invalid access (read, write or call) to a property or a function
 *
 * It may happen when:
 *
 * \li Calling \c get on a non-readable property
 * \li Calling \c set on a non-writable property
 * \li Calling \c call on a non-callable function
 */
class InvalidAccess : public Error
{
public:

    /**
     * \brief Enumeration of action types
     */
    enum Action
    {
        Read,  ///< Read action (call to \c get)
        Write, ///< Write action (call to \c set)
        Call   ///< Call action (call to \c call)
    };

    /**
     * \brief Constructor
     *
     * \param attribute Name of the owner property/function
     * \param action Type of action which triggered the error
     */
    CAMP_API InvalidAccess(const char* attribute, Action action) throw();

    /**
     * \brief Destructor
     */
    CAMP_API virtual ~InvalidAccess() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    CAMP_API virtual const char* what() const throw();

    /**
     * \brief Get the owner property/function
     *
     * \return Name of the owner attribute
     */
    CAMP_API const char* attribute() const throw();

    /**
     * \brief Get the type of action
     *
     * \return Type of action which triggered the error
     */
    CAMP_API Action action() const throw();

private:

    char m_attribute[256]; ///< Name of the owner attribute (property or function)
    Action m_action; ///< Type of action which triggered the error
};

} // namespace camp


#endif // CAMP_INVALIDACCESS_HPP
