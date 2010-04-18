/****************************************************************************
**
** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** CAMP is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** 
** CAMP is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public License
** along with CAMP.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/


#ifndef CAMP_DETAIL_CLASSMANAGER_HPP
#define CAMP_DETAIL_CLASSMANAGER_HPP


#include <camp/config.hpp>
#include <camp/detail/observernotifier.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <string>
#include <vector>


namespace camp
{
class Class;

namespace detail
{
/**
 * \brief Manages creation, storage, retrieval and destruction of metaclasses
 *
 * camp::ClassManager is the place where all metaclasses are stored and accessed.
 * It consists of a singleton which is created on first use and destroyed at global exit.
 *
 * \sa Class
 */
class CAMP_API ClassManager : public ObserverNotifier, boost::noncopyable
{
public:

    /**
     * \brief Get the unique instance of the class
     *
     * \return Reference to the unique instance of ClassManager
     */
    static ClassManager& instance();

    /**
     * \brief Create and register a new metaclass
     *
     * This is the entry point for every metaclass creation. This
     * function also notifies registered observers after successful creations.
     *
     * \param name Name of the metaclass to create (must be unique)
     * \param id Identifier of the C++ class bound to the metaclass
     *
     * \return Reference to the new metaclass
     */
    Class& registerNew(const std::string& name, const std::string& id);

    /**
     * \brief Get a metaclass from its name
     *
     * \param name Name of the metaclass to retrieve
     *
     * \return Reference to the requested metaclass
     *
     * \throw InvalidClass name is not the name of an existing metaclass
     */
    const Class& getByName(const std::string& name) const;

    /**
     * \brief Get the number of metaclasses bound to a C++ type
     *
     * \param id Identifier of the C++ type
     *
     * \return Number of metaclasses bound to the given type
     */
    std::size_t count(const std::string& id) const;

    /**
     * \brief Get a metaclass from a C++ type
     *
     * If multiple metaclasses are bound to the given type, the index parameter
     * is used to choose which one to return. Otherwise this parameter is ignored.
     *
     * \param id Identifier of the C++ type
     * \param index Index of the metaclass to get, within the metaclasses bound to the type
     *
     * \return Reference to the requested metaclass
     *
     * \throw InvalidClass id is not the name of an existing metaclass
     * \throw InvalidIndex index is out of range
     */
    const Class& getById(const std::string& id, std::size_t index) const;

    /**
     * \brief Get a metaclass from a C++ type
     *
     * This version returns a null pointer if no metaclass is found, instead
     * of throwing an exception.
     * If multiple metaclasses are bound to the given type, the index parameter
     * is used to choose which one to return. Otherwise this parameter is ignored.
     *
     * \param id Identifier of the C++ type
     * \param index Index of the metaclass to get, within the metaclasses bound to the type
     *
     * \return Pointer to the requested metaclass, or 0 if not found
     */
    const Class* getByIdSafe(const std::string& id, std::size_t index) const;

    /**
     * \brief Get the total number of metaclasses
     *
     * \return Number of metaclasses that have been registered
     */
    std::size_t count() const;

    /**
     * \brief Get a metaclass from its global index
     *
     * This function, together with ClassManager::count, provides a way to iterate through
     * all the metaclasses that have been declared.
     *
     * \param index Global index of the metaclass to get
     *
     * \return Reference to the index-th metaclass
     *
     * \throw InvalidIndex index is out of range
     */
    const Class& getByIndex(std::size_t index) const;

    /**
     * \brief Check if a given type has a metaclass
     *
     * \param id Identifier of the C++ type
     *
     * \return True if the class exists, false otherwise
     */
    bool classExists(const std::string& id) const;

private:

    /**
     * \brief Default constructor
     */
    ClassManager();

    /**
     * \brief Destructor
     *
     * The destructor destroys all the registered metaclasses and notifies the observers.
     */
    ~ClassManager();

    typedef boost::shared_ptr<Class> ClassPtr;
    typedef std::map<std::string, ClassPtr> ClassByNameTable;
    typedef std::map<std::string, std::vector<ClassPtr> > ClassByIdTable;

    ClassByNameTable m_byName; ///< List of metaclasses sorted by name
    ClassByIdTable m_byId; ///< List of metaclasses sorted by class id
};

} // namespace detail

} // namespace camp


#endif // CAMP_DETAIL_CLASSMANAGER_HPP
