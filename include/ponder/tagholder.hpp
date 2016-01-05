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


#ifndef PONDER_TAGHOLDER_HPP
#define PONDER_TAGHOLDER_HPP


#include <ponder/detail/getter.hpp>
#include <ponder/value.hpp>
#include <map>


namespace ponder
{
class UserObject;

/**
 * \brief Defines objects that hold a set of tags
 *
 * TagHolder defines an interface for accessing custom tags stored into instances.
 *
 * A tag is an identifier which can be of any supported type (it is stored in a ponder::Value).
 * It can also be associated to a value, either constant (static tags) or evaluated each
 * time the tag is requested (dynamic tags).
 *
 * \code
 * // Check tag existence
 * bool scriptable = tagHolder.hasTag("scriptable");
 *
 * // Retrieve static tag
 * int category = tagHolder.tag("category");
 *
 * // Retrieve dynamic tag whose value depends on an object's state
 * bool hidden = tagHolder.tag("hidden", object);
 * \endcode
 */
class PONDER_API TagHolder
{
public:

    /**
     * \brief Destructor
     */
    virtual ~TagHolder();

    /**
     * \brief Get the total number of tags stored in the object
     *
     * \return Number of tags
     */
    std::size_t tagCount() const;

    /**
     * \brief Get a tag by its index
     *
     * \param index Index of the tag to retrieve
     *
     * \return index-th tag
     *
     * \throw OutOfRange index is out of range
     */
    const Value& tagId(std::size_t index) const;

    /**
     * \brief Check the existence of a tag
     *
     * \param id Identifier of the tag to check
     *
     * \return True if the tag exists, false otherwise
     */
    bool hasTag(const Value& id) const;

    /**
     * \brief Get the value associated to a tag
     *
     * \param id Identifier of the tag to get
     *
     * \return Value associated to the tag, or Value::nothing if the tag doesn't exist
     */
    const Value& tag(const Value& id) const;

    /**
     * \brief Get the value associated to a dynamic tag
     *
     * \param id Identifier of the tag to get
     * \param object User object for which to get the tag value 
     *
     * \return Value associated to the tag for \a object, or Value::nothing if the tag doesn't exist
     */
    Value tag(const Value& id, const UserObject& object) const;

protected:

    /**
     * \brief Default constructor
     */
    TagHolder();

private:

    template <typename T> friend class ClassBuilder;

    typedef std::map<Value, detail::Getter<Value> > TagsTable;

    TagsTable m_tags; ///< Table of tags / values
};

} // namespace ponder


#endif // PONDER_TAGHOLDER_HPP
