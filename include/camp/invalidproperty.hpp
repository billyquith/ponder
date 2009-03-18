// <insert copyright>

#ifndef CAMP_INVALIDPROPERTY_HPP
#define CAMP_INVALIDPROPERTY_HPP


#include <camp/error.hpp>


namespace camp
{
class Class;

/**
 * \brief Error thrown when requesting a property which doesn't exist in a metaclass
 */
class InvalidProperty : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param property Name of the requested property
     * \param ownerClass Owner metaclass
     */
    CAMP_API InvalidProperty(const char* property, const Class& ownerClass) throw();

    /**
     * \brief Destructor
     */
    CAMP_API virtual ~InvalidProperty() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    CAMP_API virtual const char* what() const throw();

    /**
     * \brief Return the requested property name
     *
     * \return Name of the requested property
     */
    CAMP_API const char* property() const throw();

    /**
     * \brief Return the owner metaclass
     *
     * \return Reference to the owner metaclass
     */
    CAMP_API const Class& ownerClass() const throw();

private:

    char m_property[256]; ///< Name of the requested property
    const Class* m_ownerClass; ///< Owner metaclass
};

} // namespace camp


#endif // CAMP_INVALIDPROPERTY_HPP
