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
class CAMP_API InvalidIndex : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param index Invalid index
     * \param size Allowed size
     */
    InvalidIndex(std::size_t index, std::size_t size) throw();

    /**
     * \brief Destructor
     */
    virtual ~InvalidIndex() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    virtual const char* what() const throw();

    /**
     * \brief Return the invalid index which was passed
     *
     * \return Index
     */
    std::size_t index() const throw();

    /**
     * \brief Return the allowed size
     *
     * \return Valid range
     */
    std::size_t size() const throw();

private:

    std::size_t m_index; ///< Invalid index
    std::size_t m_size; ///< Valid size
};

} // namespace camp


#endif // CAMP_INVALIDINDEX_HPP
