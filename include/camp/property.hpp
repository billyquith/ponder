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


#ifndef CAMP_PROPERTY_HPP
#define CAMP_PROPERTY_HPP


#include <camp/config.hpp>
#include <camp/detail/getter.hpp>
#include <camp/tagholder.hpp>
#include <camp/type.hpp>
#include <string>


namespace camp
{
template <typename T> class ClassBuilder;
class Value;
class UserObject;
class ClassVisitor;

/**
 * \brief Abstract representation of a property
 *
 * Properties are members of metaclasses. Their main purpose is to be get and/or set;
 * They also provide detailed informations about their type.
 *
 * \sa PSimpleroperty, ArrayProperty, EnumProperty, ObjectProperty
 */
class CAMP_API Property : public TagHolder
{
public:

    /**
     * \brief Destructor
     */
    virtual ~Property();

    /**
     * \brief Get the name of the property
     *
     * \return Name of the property
     */
    const std::string& name() const;

    /**
     * \brief Get the type of the property
     *
     * \return Type of the property
     */
    Type type() const;

    /**
     * \brief Check if the property is currently readable for a given object
     *
     * \param object Object
     *
     * \return True if the property can be read, false otherwise
     *
     * \throw InvalidObject object has an invalid value
     */
    bool readable(const UserObject& object) const;

    /**
     * \brief Check if the property is currently writable for a given object
     *
     * \param object Object
     *
     * \return True if the property can be written, false otherwise
     *
     * \throw InvalidObject object has an invalid value
     */
    bool writable(const UserObject& object) const;

    /**
     * \brief Get the current value of the property for a given object
     *
     * \param object Object
     *
     * \return Value of the property
     *
     * \throw InvalidObject object has an invalid value
     * \throw InvalidAccess property is not readable
     */
    Value get(const UserObject& object) const;

    /**
     * \brief Set the current value of the property for a given object
     *
     * \param object Object
     * \param value New value to assign to the property
     *
     * \throw InvalidObject object has an invalid value
     * \throw InvalidAccess property is not writable
     * \throw InvalidValue value can't be converted to the property's type
     */
    void set(const UserObject& object, const Value& value) const;

    /**
     * \brief Accept the visitation of a ClassVisitor
     *
     * \param visitor Visitor to accept
     */
    virtual void accept(ClassVisitor& visitor) const;

protected:

    template <typename T> friend class ClassBuilder;
    friend class UserObject;

    /**
     * \brief Construct the property from its description
     *
     * \param name Name of the property
     * \param type Type of the property
     */
    Property(const std::string& name, Type type);

    /**
     * \brief Do the actual reading of the value
     *
     * This function is a pure virtual which has to be implemented in derived classes.
     *
     * \param object Object
     *
     * \return Value of the property
     */
    virtual Value getValue(const UserObject& object) const = 0;

    /**
     * \brief Do the actual writing of the value
     *
     * This function is a pure virtual which has to be implemented in derived classes.
     *
     * \param object Object
     * \param value New value to assign to the property
     */
    virtual void setValue(const UserObject& object, const Value& value) const = 0;

    /**
     * \brief Check if the property can be read
     *
     * \return True if the property can be read, false otherwise
     */
    virtual bool isReadable() const;

    /**
     * \brief Check if the property can be written
     *
     * \return True if the property can be written, false otherwise
     */
    virtual bool isWritable() const;

private:

    std::string m_name; ///< Name of the property
    Type m_type; ///< Type of the property
    detail::Getter<bool> m_readable; ///< Accessor to get the readable state of the property
    detail::Getter<bool> m_writable; ///< Accessor to get the writable state of the property
};

} // namespace camp


#endif // CAMP_PROPERTY_HPP
