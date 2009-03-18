// <insert copyright>

#ifndef CAMP_INVALIDFUNCTION_HPP
#define CAMP_INVALIDFUNCTION_HPP


#include <camp/error.hpp>


namespace camp
{
class Class;

/**
 * \brief Error thrown when requesting a function which doesn't exist in a metaclass
 */
class InvalidFunction : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param function Name of the requested function
     * \param ownerClass Owner metaclass
     */
    CAMP_API InvalidFunction(const char* function, const Class& ownerClass) throw();

    /**
     * \brief Destructor
     */
    CAMP_API virtual ~InvalidFunction() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    CAMP_API virtual const char* what() const throw();

    /**
     * \brief Return the requested function name
     *
     * \return Name of the requested function
     */
    CAMP_API const char* function() const throw();

    /**
     * \brief Return the owner metaclass
     *
     * \return Reference to the owner metaclass
     */
    CAMP_API const Class& ownerClass() const throw();

private:

    char m_function[256]; ///< Name of the requested function
    const Class* m_ownerClass; ///< Owner metaclass
};

} // namespace camp


#endif // CAMP_INVALIDFUNCTION_HPP
