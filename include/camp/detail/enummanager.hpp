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


#ifndef CAMP_DETAIL_ENUMMANAGER_HPP
#define CAMP_DETAIL_ENUMMANAGER_HPP


#include <camp/config.hpp>
#include <camp/detail/observernotifier.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <string>
#include <vector>


namespace camp
{
class Enum;

namespace detail
{
/**
 * \brief Manages creation, storage, retrieval and destruction of metaenums
 *
 * camp::EnumManager is the place where all metaenums are stored and accessed.
 * It consists of a singleton which is created on first use and destroyed at global exit.
 *
 * \sa Enum
 */
class CAMP_API EnumManager : public ObserverNotifier, boost::noncopyable
{
public:

    /**
     * \brief Get the unique instance of the class
     *
     * \return Reference to the unique instance of EnumManager
     */
    static EnumManager& instance();

    /**
     * \brief Create and register a new metaenum
     *
     * This is the entry point for every metaenum creation. This
     * function also notifies registered observers after successful creations.
     *
     * \param name Name of the metaenum to create (must be unique)
     * \param id Identifier of the C++ enum bound to the metaenum
     *
     * \return Reference to the new metaenum
     */
    Enum& registerNew(const std::string& name, const std::string& id);

    /**
     * \brief Get a metaenum from its name
     *
     * \param name Name of the metaenum to retrieve
     *
     * \return Reference to the requested metaenum
     *
     * \throw InvalidEnum name is not the name of an existing metaenum
     */
    const Enum& getByName(const std::string& name) const;

    /**
     * \brief Get the number of metaenums bound to a C++ type
     *
     * \param id Identifier of the C++ type
     *
     * \return Number of metaenums bound to the given type
     */
    std::size_t count(const std::string& id) const;

    /**
     * \brief Get a metaenum from a C++ type
     *
     * If multiple metaenums are bound to the given type, the index parameter
     * is used to choose which one to return. Otherwise this parameter is ignored.
     *
     * \param id Identifier of the C++ type
     * \param index Index of the metaenum to get, within the metaenums bound to the type
     *
     * \return Reference to the requested metaenum
     *
     * \throw InvalidEnum id is not the name of an existing metaenum
     * \throw InvalidIndex index is out of range
     */
    const Enum& getById(const std::string& id, std::size_t index) const;

    /**
     * \brief Get the total number of metaenums
     *
     * \return Number of metaenums that have been registered
     */
    std::size_t count() const;

    /**
     * \brief Get a metaenum from its global index
     *
     * This function, together with EnumManager::count, provides a way to iterate through
     * all the metaenums that have been declared.
     *
     * \param index Global index of the metaenum to get
     *
     * \return Reference to the index-th metaenum
     *
     * \throw InvalidIndex index is out of range
     */
    const Enum& getByIndex(std::size_t index) const;

    /**
     * \brief Check if a given type has a metaenum
     *
     * \param id Identifier of the C++ type
     *
     * \return True if the enum exists, false otherwise
     */
    bool enumExists(const std::string& id) const;

private:

    /**
     * \brief Default constructor
     */
    EnumManager();

    /**
     * \brief Destructor
     *
     * The destructor destroys all the registered metaenums and notifies the observers.
     */
    ~EnumManager();

    typedef boost::shared_ptr<Enum> EnumPtr;
    typedef std::map<std::string, EnumPtr> EnumByNameTable;
    typedef std::map<std::string, std::vector<EnumPtr> > EnumByIdTable;

    EnumByNameTable m_byName; ///< List of metaenums sorted by name
    EnumByIdTable m_byId; ///< List of metaenums sorted by class id
};

} // namespace detail

} // namespace camp


#endif // CAMP_DETAIL_ENUMMANAGER_HPP
