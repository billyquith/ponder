/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2019 Nick Trout.
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

#pragma once
#ifndef PONDER_DETAIL_ENUMMANAGER_HPP
#define PONDER_DETAIL_ENUMMANAGER_HPP

#include <ponder/detail/observernotifier.hpp>
#include <ponder/detail/util.hpp>
#include <string>
#include <map>

namespace ponder
{
class Enum;

namespace detail
{
/**
 * \brief Manages creation, storage, retrieval and destruction of metaenums
 *
 * ponder::EnumManager is the place where all metaenums are stored and accessed.
 * It consists of a singleton which is created on first use and destroyed at global exit.
 *
 * \sa Enum
 */
class PONDER_API EnumManager : public ObserverNotifier
{
    PONDER__NON_COPYABLE(EnumManager);
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
     * \param id Identifier of the C++ enum bound to the metaenum (unique).
     *
     * \return Reference to the new metaenum
     */
    Enum& addClass(TypeId const& id, IdRef name);
    
    /**
     * \brief Unregister a metaenum
     *
     * This unregisters the metaenum so that it may not be used again.
     *
     * \param id Identifier of the C++ enum bound to the metaenum (unique).
     */
    void removeClass(TypeId const& id);

    /**
     * \brief Get the total number of metaenums
     *
     * \return Number of metaenums that have been registered
     */
    std::size_t count() const;

    /**
     * \brief Get a metaenum from a C++ type
     *
     * \param id Identifier of the C++ type
     *
     * \return Reference to the requested metaenum
     *
     * \throw EnumNotFound id is not the name of an existing metaenum
     */
    const Enum& getById(TypeId const& id) const;

    const Enum& getByName(IdRef id) const;

    /**
     * \brief Get a metaenum from a C++ type
     *
     * This version returns a null pointer if no metaenum is found, instead
     * of throwing an exception.
     *
     * \param id Identifier of the C++ type
     *
     * \return Pointer to the requested metaenum, or null pointer if not found
     */
    const Enum* getByIdSafe(TypeId const& id) const;

    /**
     * \brief Check if a given type has a metaenum
     *
     * \param id Identifier of the C++ type
     *
     * \return True if the enum exists, false otherwise
     */
    bool enumExists(TypeId const& id) const;

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

    typedef std::map<TypeId, Enum*> EnumTable;
    typedef std::map<Id, Enum*> NameTable;
    EnumTable m_enums; // Table storing enums indexed by their TypeId
    NameTable m_names; // Table storing enums indexed by their name
};

} // namespace detail

} // namespace ponder


#endif // PONDER_DETAIL_ENUMMANAGER_HPP
