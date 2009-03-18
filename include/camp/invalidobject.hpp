// <insert copyright>

#ifndef CAMP_INVALIDOBJECT_HPP
#define CAMP_INVALIDOBJECT_HPP


#include <camp/error.hpp>


namespace camp
{
class UserObject;

/**
 * \brief Error thrown when using an invalid user object
 *
 * This error may be thrown if user object is null, of if its type doesn't match the requested one
 */
class InvalidObject : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param object User object
     */
    CAMP_API InvalidObject(const UserObject& object) throw();

    /**
     * \brief Copy constructor
     *
     * \param copy Instance to copy
     */
    CAMP_API InvalidObject(const InvalidObject& copy) throw();

    /**
     * \brief Destructor
     */
    CAMP_API virtual ~InvalidObject() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    CAMP_API virtual const char* what() const throw();

    /**
     * \brief Return the user object
     *
     * \return Reference to the user object
     */
    CAMP_API const UserObject& object() const throw();

private:

    const UserObject* m_object; ///< User object
};

} // namespace camp


#endif // CAMP_INVALIDOBJECT_HPP
