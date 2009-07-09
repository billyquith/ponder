

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
class CAMP_API InvalidObject : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param object User object
     */
    InvalidObject(const UserObject& object) throw();

    /**
     * \brief Destructor
     */
    virtual ~InvalidObject() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    virtual const char* what() const throw();

    /**
     * \brief Return the user object
     *
     * \return Reference to the user object
     */
    const UserObject& object() const throw();

private:

    const UserObject* m_object; ///< User object
};

} // namespace camp


#endif // CAMP_INVALIDOBJECT_HPP
