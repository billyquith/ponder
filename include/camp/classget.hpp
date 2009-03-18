// <insert copyright>

#ifndef CAMP_CLASSGET_HPP
#define CAMP_CLASSGET_HPP


#include <camp/config.hpp>
#include <camp/error.hpp>
#include <camp/detail/typeid.hpp>
#include <camp/detail/classmanager.hpp>
#include <string>


namespace camp
{
/**
 * \relates Class
 *
 * \brief Get the total number of existing metaclasses
 *
 * \return Global metaclass count
 */
std::size_t classCount();

/**
 * \relates Class
 *
 * \brief Get the number of metaclasses bound to a C++ type
 *
 * \return Number of metaclasses bound to T
 */
template <typename T>
std::size_t classCount();

/**
 * \relates Class
 *
 * \brief Get the number of metaclasses bound to an object
 *
 * If the object has a dynamic type U which is a derived class of T,
 * then it returns classCount<U>(). Otherwise it's equivalent to classCount<T>().
 *
 * \param object Object to get the metaclass count from
 *
 * \return Number of metaclasses bound to T
 */
template <typename T>
std::size_t classCount(const T& object);

/**
 * \relates Class
 *
 * \brief Get a metaclass from its global index
 *
 * The purpose of this function is to provide a way to iterate through
 * all existing metaclasses. \sa classCount
 *
 * \param index Index of the metaclass to get
 *
 * \return Reference to the index-th metaclass
 *
 * \throw InvalidIndex index is out of range
 */
const Class& classByIndex(std::size_t index);

/**
 * \relates Class
 *
 * \brief Get a metaclass from its C++ type
 *
 * If more than one metaclass is bound to T, then the
 * index parameter is used to choose which metaclass to retrieve.
 * Otherwise it is ignored.
 *
 * \param index Index of the metaclass to retrieve, if multiple metaclasses are bound to T (0 by default)
 *
 * \return Reference to the index-th metaclass bound to type T
 *
 * \throw InvalidClass no metaclass has been declared for T
 * \throw InvalidIndex index is out of range
 */
template <typename T>
const Class& classByType(std::size_t index = 0);

/**
 * \relates Class
 *
 * \brief Get a metaclass from a C++ object
 *
 * If more than one metaclass is bound to the object metaclass, then the
 * index parameter is used to choose which metaclass to retrieve.
 * Otherwise it is ignored.
 *
 * \param object object to get the metaclass of
 * \param index Index of the metaclass to retrieve, if multiple metaclasses are bound to T (0 by default)
 *
 * \return Reference to the index-th metaclass bound to type T
 *
 * \throw InvalidClass no metaclass has been declared for T
 * \throw InvalidIndex index is out of range
 */
template <typename T>
const Class& classByObject(const T& object, std::size_t index = 0);

/**
 * \relates Class
 *
 * \brief Get a metaclass from its name
 *
 * \param name Name of the metaclass to retrieve (case sensitive)
 *
 * \return Reference to the requested metaclass
 *
 * \throw InvalidClass name is not a valid metaclass name
 */
const Class& classByName(const std::string& name);

/**
 * \relates Class
 *
 * \brief Get a metaclass from its C++ type
 *
 * This function, unlike classByType, returns a null pointer on failure. It's meant to
 * be used in contexts where T may not have a valid metaclass declared.
 *
 * \param index Index of the metaclass to retrieve, if multiple metaclasses are bound to T (0 by default)
 *
 * \return Pointer to the index-th metaclass bound to type T, or 0 if no metaclass has been declared
 */
template <typename T>
const Class* classByTypeSafe(std::size_t index = 0);

} // namespace camp

#include <camp/classget.inl>


#endif // CAMP_CLASSGET_HPP
