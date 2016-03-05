/****************************************************************************
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
**
** This file is part of the Ponder library, formerly CAMP.
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


#ifndef PONDER_DETAIL_CLASSMANAGER_HPP
#define PONDER_DETAIL_CLASSMANAGER_HPP

#include <ponder/detail/observernotifier.hpp>
#include <string>
#include <map>

namespace ponder
{
class Class;

namespace detail
{
/**
 * \brief Manages creation, storage, retrieval and destruction of metaclasses
 *
 * ponder::ClassManager is the place where all metaclasses are stored and accessed.
 * It consists of a singleton which is created on first use and destroyed at global exit.
 *
 * \sa Class
 */
class PONDER_API ClassManager : public ObserverNotifier
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
     * \param id Identifier of the C++ class bound to the metaclass (unique)
     *
     * \return Reference to the new metaclass
     *
     * \throw ClassAlreadyCreated \a name or \a id already exists
     */
    Class& addClass(const std::string& id);

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
     * \return Pointer to the requested metaclass, or null pointer if not found
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

    typedef std::map<std::string, Class*> ClassTable; ///< No need for shared pointers in here, we're the one and only instance holder
    ClassTable m_classes; ///< Table storing classes indexed by their ID
};

} // namespace detail

} // namespace ponder


#endif // PONDER_DETAIL_CLASSMANAGER_HPP
