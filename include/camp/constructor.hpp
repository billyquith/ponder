// <insert copyright>

#ifndef CAMP_CONSTRUCTOR_HPP
#define CAMP_CONSTRUCTOR_HPP


#include <camp/config.hpp>
#include <camp/args.hpp>
#include <vector>


namespace camp
{
class Value;

/**
 * \brief camp::Constructor represents a metaconstructor which is used to create objects instances from metaclasses
 *
 * This class is an interface which has to be derived to implement typed constructors.
 *
 * \sa Property, Function
 */
class Constructor
{
public:

    /**
     * \brief Destructor
     */
    virtual ~Constructor() {}

    /**
     * \brief Check if the constructor matches the given set of arguments
     *
     * \param args Set of arguments to check
     *
     * \return True if the constructor is compatible with the given arguments
     */
    virtual bool matches(const Args& args) const = 0;

    /**
     * \brief Use the constructor to create a new object
     *
     * \param args Set of arguments to pass to the constructor
     *
     * \return Pointer to the new object, or 0 on failure
     */
    virtual void* create(const Args& args) const = 0;
};

} // namespace camp


#endif // CAMP_CONSTRUCTOR_HPP
