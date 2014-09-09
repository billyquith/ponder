/****************************************************************************
**
** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
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


#ifndef CAMP_DETAIL_ENUMMANAGER_HPP
#define CAMP_DETAIL_ENUMMANAGER_HPP


#include <camp/config.hpp>
#include <camp/detail/observernotifier.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <string>


namespace bm = boost::multi_index;

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
class CAMP_API EnumManager : public ObserverNotifier, camp::noncopyable
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
    Enum& addClass(const std::string& name, const std::string& id);

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
     * \throw OutOfRange index is out of range
     */
    const Enum& getByIndex(std::size_t index) const;

    /**
     * \brief Get a metaenum from its name
     *
     * \param name Name of the metaenum to retrieve
     *
     * \return Reference to the requested metaenum
     *
     * \throw EnumNotFound name is not the name of an existing metaenum
     */
    const Enum& getByName(const std::string& name) const;

    /**
     * \brief Get a metaenum from a C++ type
     *
     * \param id Identifier of the C++ type
     *
     * \return Reference to the requested metaenum
     *
     * \throw EnumNotFound id is not the name of an existing metaenum
     */
    const Enum& getById(const std::string& id) const;

    /**
     * \brief Get a metaenum from a C++ type
     *
     * This version returns a null pointer if no metaenum is found, instead
     * of throwing an exception.
     *
     * \param id Identifier of the C++ type
     *
     * \return Pointer to the requested metaenum, or 0 if not found
     */
    const Enum* getByIdSafe(const std::string& id) const;

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

    /**
     * \brief Structure gathering an enum, its type identifier and its name
     */
    struct EnumInfo
    {
        std::string id;
        std::string name;
        boost::shared_ptr<Enum> enumPtr;
    };

    struct Id;
    struct Name;

    typedef boost::multi_index_container<EnumInfo,
        bm::indexed_by<bm::ordered_unique<bm::tag<Id>,   bm::member<EnumInfo, std::string, &EnumInfo::id> >,
                       bm::ordered_unique<bm::tag<Name>, bm::member<EnumInfo, std::string, &EnumInfo::name> >
        >
    > EnumTable;

    typedef EnumTable::index<Id>::type IdIndex;
    typedef EnumTable::index<Name>::type NameIndex;

    EnumTable m_enums; ///< Table storing enums indexed by their id and name
};

} // namespace detail

} // namespace camp


#endif // CAMP_DETAIL_ENUMMANAGER_HPP
