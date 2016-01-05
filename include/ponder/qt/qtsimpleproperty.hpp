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


#ifndef PONDER_QT_QTSIMPLEPROPERTY_HPP
#define PONDER_QT_QTSIMPLEPROPERTY_HPP


#include <ponder/qt/qthelper.hpp>
#include <ponder/simpleproperty.hpp>
#include <ponder/userobject.hpp>
#include <ponder/value.hpp>
#include <QMetaProperty>
#include <QVariant>


namespace ponder_ext
{
/**
 * \brief Specialization of ponder::SimpleProperty implemented using a Qt property
 *
 * This class is instanciated and returned by QtMapper<T>.
 *
 * \sa QtMapper
 */
template <typename T>
class QtSimpleProperty : public ponder::SimpleProperty
{
public:

    /**
     * \brief Construct the property from a QMetaProperty
     *
     * \param metaProperty Qt meta property
     */
    QtSimpleProperty(const QMetaProperty& metaProperty)
        : ponder::SimpleProperty(metaProperty.name(), metaProperty.isEnumType() ? ponder::enumType : QtHelper::type(metaProperty.type()))
        , m_metaProperty(metaProperty)
    {
    }

    /**
     * \brief Get the value of the property for the given object
     *
     * \param object Object to read
     *
     * \return Current value of the property
     */
    virtual ponder::Value getValue(const ponder::UserObject& object) const override
    {
        return QtHelper::variantToValue(m_metaProperty.read(object.get<T*>()));
    }

    /**
     * \brief Set the value of the property for the given object
     *
     * \param object Object to write
     * \param value New value of the property
     */
    virtual void setValue(const ponder::UserObject& object, const ponder::Value& value) const override
    {
        m_metaProperty.write(object.get<T*>(), QtHelper::valueToVariant(value));
    }

    /**
     * \brief Check if the property is readable
     *
     * \return True if the property is readable, false otherwise
     */
    virtual bool isReadable() const override
    {
        return m_metaProperty.isReadable();
    }

    /**
     * \brief Check if the property is writable
     *
     * \return True if the property is writable, false otherwise
     */
    virtual bool isWritable() const override
    {
        return m_metaProperty.isWritable();
    }

private:

    QMetaProperty m_metaProperty; ///< Internal Qt property
};

} // namespace ponder_ext


#endif // PONDER_QT_QTSIMPLEPROPERTY_HPP
