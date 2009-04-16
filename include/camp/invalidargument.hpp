// <insert copyright>

#ifndef CAMP_INVALIDARGUMENT_HPP
#define CAMP_INVALIDARGUMENT_HPP


#include <camp/invalidvalue.hpp>


namespace camp
{
/**
 * \brief Error thrown when passing an invalid argument to a function call
 */
class CAMP_API InvalidArgument : public InvalidValue
{
public:

    /**
     * \brief Constructor
     *
     * \param sourceType Type of the source value
     * \param requestedType Type the value is being converted to
     * \param function Name of the owner function
     * \param index Index of the argument
     */
    InvalidArgument(Type sourceType, Type requestedType, const char* function, std::size_t index) throw();

    /**
     * \brief Destructor
     */
    virtual ~InvalidArgument() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    virtual const char* what() const throw();

    /**
     * \brief Return the name of the owner function
     *
     * \return Name of the function
     */
    const char* function() const throw();

    /**
     * \brief Return the index of the argument
     *
     * \return Index of the argument within the function call
     */
    std::size_t index() const throw();

private:

    char m_function[256]; ///< Name of the owner function
    std::size_t m_index; ///< Index of the argument in the function call
};

} // namespace camp


#endif // CAMP_INVALIDARGUMENT_HPP
