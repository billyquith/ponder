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
#include <boost/shared_ptr.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <string>


namespace bm = boost::multi_index;

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
     *
     * \throw ClassAlreadyCreated \a name or \a id already exists
     */
    Class& registerNew(const std::string& name, const std::string& id);

    /**
     * \brief Remove an existing metaclass by its name
     *
     * \param id Name of the metaclass to unregister
     *
     * \throw ClassNotFound \a name is not an existing metaclass
     */
    void unregister(const std::string& name);

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
     * \throw OutOfRange index is out of range
     */
    const Class& getByIndex(std::size_t index) const;

    /**
     * \brief Get a metaclass from its name
     *
     * \param name Name of the metaclass to retrieve
     *
     * \return Reference to the requested metaclass
     *
     * \throw ClassNotFound name is not the name of an existing metaclass
     */
    const Class& getByName(const std::string& name) const;

    /**
     * \brief Get a metaclass from a C++ type
     *
     * \param id Identifier of the C++ type
     *
     * \return Reference to the requested metaclass
     *
     * \throw ClassNotFound id is not the name of an existing metaclass
     */
    const Class& getById(const std::string& id) const;

    /**
     * \brief Get a metaclass from a C++ type
     *
     * This version returns a null pointer if no metaclass is found, instead
     * of throwing an exception.
     *
     * \param id Identifier of the C++ type
     *
     * \return Pointer to the requested metaclass, or 0 if not found
     */
    const Class* getByIdSafe(const std::string& id) const;

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

    /**
     * \brief Structure gathering a class, its type identifier and its name
     */
    struct ClassInfo
    {
        std::string id;
        std::string name;
        boost::shared_ptr<Class> classPtr;
    };

    struct Id;
    struct Name;

    typedef boost::multi_index_container<ClassInfo,
        bm::indexed_by<bm::ordered_unique<bm::tag<Id>,   bm::member<ClassInfo, std::string, &ClassInfo::id> >,
                       bm::ordered_unique<bm::tag<Name>, bm::member<ClassInfo, std::string, &ClassInfo::name> >
        >
    > ClassTable;

    typedef ClassTable::nth_index<0>::type IdIndex;
    typedef ClassTable::nth_index<1>::type NameIndex;

    ClassTable m_classes; ///< Table storing classes indexed by their id and name
};

} // namespace detail

} // namespace camp


#endif // CAMP_DETAIL_CLASSMANAGER_HPP
