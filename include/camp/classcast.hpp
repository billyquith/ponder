// <insert copyright>

#ifndef CAMP_CLASSCAST_HPP
#define CAMP_CLASSCAST_HPP


#include <camp/config.hpp>


namespace camp
{
class Class;

/**
 * \brief Convert a pointer from a source metaclass to a related target metaclass
 *
 * \param pointer Source pointer to convert
 * \param sourceClass Source metaclass to convert from
 * \param targetClass Target metaclass to convert to
 *
 * \return Converted pointer, or 0 on failure
 */
CAMP_API void* classCast(void* pointer, const Class& sourceClass, const Class& targetClass);

} // namespace camp

#endif // CAMP_CLASSCAST_HPP
