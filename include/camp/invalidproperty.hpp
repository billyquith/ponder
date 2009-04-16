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
class CAMP_API InvalidProperty : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param property Name of the requested property
     * \param ownerClass Owner metaclass
     */
    InvalidProperty(const char* property, const Class& ownerClass) throw();

    /**
     * \brief Copy constructor
     *
     * \param copy Instance to copy
     */
    InvalidProperty(const InvalidProperty& copy) throw();

    /**
     * \brief Destructor
     */
    virtual ~InvalidProperty() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    virtual const char* what() const throw();

    /**
     * \brief Return the requested property name
     *
     * \return Name of the requested property
     */
    const char* property() const throw();

    /**
     * \brief Return the owner metaclass
     *
     * \return Reference to the owner metaclass
     */
    const Class& ownerClass() const throw();

private:

    char m_property[256]; ///< Name of the requested property
    const Class* m_ownerClass; ///< Owner metaclass
};

} // namespace camp


#endif // CAMP_INVALIDPROPERTY_HPP
