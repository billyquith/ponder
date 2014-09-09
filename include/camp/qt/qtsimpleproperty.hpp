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


#ifndef CAMP_QT_QTSIMPLEPROPERTY_HPP
#define CAMP_QT_QTSIMPLEPROPERTY_HPP


#include <camp/qt/qthelper.hpp>
#include <camp/simpleproperty.hpp>
#include <camp/userobject.hpp>
#include <camp/value.hpp>
#include <QMetaProperty>
#include <QVariant>


namespace camp_ext
{
/**
 * \brief Specialization of camp::SimpleProperty implemented using a Qt property
 *
 * This class is instanciated and returned by QtMapper<T>.
 *
 * \sa QtMapper
 */
template <typename T>
class QtSimpleProperty : public camp::SimpleProperty
{
public:

    /**
     * \brief Construct the property from a QMetaProperty
     *
     * \param metaProperty Qt meta property
     */
    QtSimpleProperty(const QMetaProperty& metaProperty)
        : camp::SimpleProperty(metaProperty.name(), metaProperty.isEnumType() ? camp::enumType : QtHelper::type(metaProperty.type()))
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
    virtual camp::Value getValue(const camp::UserObject& object) const
    {
        return QtHelper::variantToValue(m_metaProperty.read(object.get<T*>()));
    }

    /**
     * \brief Set the value of the property for the given object
     *
     * \param object Object to write
     * \param value New value of the property
     */
    virtual void setValue(const camp::UserObject& object, const camp::Value& value) const
    {
        m_metaProperty.write(object.get<T*>(), QtHelper::valueToVariant(value));
    }

    /**
     * \brief Check if the property is readable
     *
     * \return True if the property is readable, false otherwise
     */
    virtual bool isReadable() const
    {
        return m_metaProperty.isReadable();
    }

    /**
     * \brief Check if the property is writable
     *
     * \return True if the property is writable, false otherwise
     */
    virtual bool isWritable() const
    {
        return m_metaProperty.isWritable();
    }

private:

    QMetaProperty m_metaProperty; ///< Internal Qt property
};

} // namespace camp_ext


#endif // CAMP_QT_QTSIMPLEPROPERTY_HPP
