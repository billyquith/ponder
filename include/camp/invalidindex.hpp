// <insert copyright>

#ifndef CAMP_INVALIDINDEX_HPP
#define CAMP_INVALIDINDEX_HPP


#include <camp/error.hpp>


namespace camp
{
/**
 * \brief Error thrown when using an index which is out of bounds
 *
 * This error may be thrown by every function which takes an index in its parameters
 */
class InvalidIndex : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param index Invalid index
     * \param size Allowed size
     */
    CAMP_API InvalidIndex(std::size_t index, std::size_t size) throw();

    /**
     * \brief Destructor
     */
    CAMP_API virtual ~InvalidIndex() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    CAMP_API virtual const char* what() const throw();

    /**
     * \brief Return the invalid index which was passed
     *
     * \return Index
     */
    CAMP_API std::size_t index() const throw();

    /**
     * \brief Return the allowed size
     *
     * \return Valid range
     */
    CAMP_API std::size_t size() const throw();

private:

    std::size_t m_index; ///< Invalid index
    std::size_t m_size; ///< Valid size
};

} // namespace camp


#endif // CAMP_INVALIDINDEX_HPP
