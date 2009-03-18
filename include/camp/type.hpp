// <insert copyright>

#ifndef CAMP_TYPE_HPP
#define CAMP_TYPE_HPP


#include <string>
#include <cassert>


namespace camp
{
/**
 * \brief Special empty type associated to \c noType
 *
 * This type is mainly used when writing custom \c Value visitors
 */
struct NoType
{
};

/**
 * \brief Enumeration of abstract types supported by CAMP
 *
 * \sa ValueMapper
 */
enum Type
{
    noType,     ///< No type has been defined yet
    boolType,   ///< Boolean type (bool)
    intType,    ///< Integer types (unsigned/signed char short int long)
    realType,   ///< Real types (float, double)
    stringType, ///< String types (char*, std::string)
    enumType,   ///< Enumerated types
    arrayType,  ///< Array types (std::vector, std::list, T[])
    userType    ///< User-defined classes
};

} // namespace camp


#endif // CAMP_TYPE_HPP
