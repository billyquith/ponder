/****************************************************************************
**
** Copyright (C) 2009 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** Commercial Usage
** Licensees holding valid CAMP Commercial licenses may use this file in
** accordance with the CAMP Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and TECHNOGERMA Systems France.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3.txt included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at sales@technogerma.fr.
**
****************************************************************************/


#ifndef CAMP_TAGHOLDER_HPP
#define CAMP_TAGHOLDER_HPP


#include <camp/config.hpp>
#include <camp/detail/getter.hpp>
#include <camp/value.hpp>
#include <boost/shared_ptr.hpp>
#include <map>


namespace camp
{
template <typename T> class ClassBuilder;
class UserObject;

/**
 * \brief Defines objects that hold a set of tags
 *
 * TagHolder defines an interface for accessing custom tags stored into instances.
 *
 * A tag is an identifier which can be of any supported type (it is stored in a camp::Value).
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
class CAMP_API TagHolder
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
     * \throw InvalidIndex index is out of range
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

} // namespace camp


#endif // CAMP_TAGHOLDER_HPP
