// <insert copyright>

#ifndef CAMP_YESNOTYPE_HPP
#define CAMP_YESNOTYPE_HPP


namespace camp
{
namespace detail
{
/**
 * \brief Defines a "yes" type to be used in metaprograms
 *
 * To work, sizeof(TypeYes) must be different than sizeof(TypeNo)
 */
typedef char TypeYes;

/**
 * \brief Defines a "no" type to be used in metaprograms
 *
 * To work, sizeof(TypeNo) must be different than sizeof(TypeYes)
 */
struct TypeNo {char padding[8];};

} // namespace detail

} // namespace camp


#endif // CAMP_YESNOTYPE_HPP
