// <insert copyright>

#ifndef CAMP_INVALIDENUMNAME_HPP
#define CAMP_INVALIDENUMNAME_HPP


#include <camp/error.hpp>


namespace camp
{
class Enum;

/**
 * \brief Error thrown when requesting the name of a pair which doesn't exist in a metaenum
 */
class CAMP_API InvalidEnumName : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param name Name of the requested pair
     * \param ownerEnum Owner metaenum
     */
    InvalidEnumName(const char* name, const Enum& ownerEnum) throw();

    /**
     * \brief Copy constructor
     *
     * \param copy Instance to copy
     */
    InvalidEnumName(const InvalidEnumName& copy) throw();

    /**
     * \brief Destructor
     */
    virtual ~InvalidEnumName() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    virtual const char* what() const throw();

    /**
     * \brief Return the requested pair name
     *
     * \return Name of the requested pair
     */
    const char* name() const throw();

    /**
     * \brief Return the owner metaenum
     *
     * \return Reference to the owner metaenum
     */
    const Enum& ownerEnum() const throw();

private:

    char m_name[256]; ///< Name of the requested pair
    const Enum* m_ownerEnum; ///< Owner metaenum
};

} // namespace camp


#endif // CAMP_INVALIDENUMNAME_HPP
