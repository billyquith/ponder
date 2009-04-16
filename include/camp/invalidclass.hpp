// <insert copyright>

#ifndef CAMP_INVALIDCLASS_HPP
#define CAMP_INVALIDCLASS_HPP


#include <camp/error.hpp>


namespace camp
{
/**
 * \brief Error thrown when requesting a metaclass which doesn't exist
 */
class CAMP_API InvalidClass : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param className Name of the requested metaclass
     */
    InvalidClass(const char* className) throw();

    /**
     * \brief Destructor
     */
    virtual ~InvalidClass() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    virtual const char* what() const throw();

    /**
     * \brief Return the requested metaclass name
     *
     * \return Name of the metaclass
     */
    const char* className() const throw();

private:

    char m_className[256]; ///< Name of the requested metaclass
};

} // namespace camp


#endif // CAMP_INVALIDCLASS_HPP
