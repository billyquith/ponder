/****************************************************************************
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
**
** This file is part of the CAMP library.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
** 
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
** 
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
**
****************************************************************************/


#ifndef CAMP_DETAIL_CLASSMANAGER_HPP
#define CAMP_DETAIL_CLASSMANAGER_HPP


#include <camp/config.hpp>
#include <camp/detail/observernotifier.hpp>
#include <camp/detail/singleton.hpp>
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
class CAMP_API ClassManager : public ObserverNotifier, public Singleton<ClassManager>
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
    Class& addClass(const std::string& name, const std::string& id);

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

private:

    /**
     * \brief Structure gathering a class, its type identifier and its name
     */
    struct ClassInfo
    {
        std::string id;
        std::string name;
        Class* classPtr;  // No need for shared pointers in here, we're the one and only instance holder
    };

    struct Id;
    struct Name;

    typedef boost::multi_index_container<ClassInfo,
        bm::indexed_by<bm::ordered_unique<bm::tag<Id>,   bm::member<ClassInfo, std::string, &ClassInfo::id> >,
                       bm::ordered_unique<bm::tag<Name>, bm::member<ClassInfo, std::string, &ClassInfo::name> >
        >
    > ClassTable;

    typedef ClassTable::index<Id>::type IdIndex;
    typedef ClassTable::index<Name>::type NameIndex;

    ClassTable m_classes; ///< Table storing classes indexed by their id and name
};

} // namespace detail

} // namespace camp


#endif // CAMP_DETAIL_CLASSMANAGER_HPP
