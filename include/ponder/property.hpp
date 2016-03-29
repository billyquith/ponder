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


#ifndef PONDER_PROPERTY_HPP
#define PONDER_PROPERTY_HPP


#include <ponder/tagholder.hpp>
#include <ponder/type.hpp>

namespace ponder
{
class ClassVisitor;

/**
 * \brief Abstract representation of a property
 *
 * Properties are members of metaclasses. Their main purpose is to be get and/or set;
 * They also provide detailed informations about their type.
 *
 * \sa SimpleProperty, ArrayProperty, EnumProperty, ObjectProperty
 */
class PONDER_API Property : public TagHolder
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
     * \throw NullObject object is invalid
     */
    bool readable(const UserObject& object) const;

    /**
     * \brief Check if the property is currently writable for a given object
     *
     * \param object Object
     *
     * \return True if the property can be written, false otherwise
     *
     * \throw NullObject object is invalid
     */
    bool writable(const UserObject& object) const;

    /**
     * \brief Get the current value of the property for a given object
     *
     * \param object Object
     *
     * \return Value of the property
     *
     * \throw NullObject object is invalid
     * \throw ForbiddenRead property is not readable
     */
    Value get(const UserObject& object) const;

    /**
     * \brief Set the current value of the property for a given object
     *
     * \param object Object
     * \param value New value to assign to the property
     *
     * \throw NullObject \a object is invalid
     * \throw ForbiddenWrite property is not writable
     * \throw BadType \a value can't be converted to the property's type
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

} // namespace ponder


#endif // PONDER_PROPERTY_HPP
