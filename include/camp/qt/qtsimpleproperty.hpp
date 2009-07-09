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
** contact the sales department at contact@technogerma.fr.
**
****************************************************************************/


#ifndef CAMP_QT_QTSIMPLEPROPERTY_HPP
#define CAMP_QT_QTSIMPLEPROPERTY_HPP


#include <camp/qt/qtvariantconverter.hpp>
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
 * This class is instanciated and returned by QMapper<T>.
 *
 * \sa QMapper
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
        : camp::SimpleProperty(metaProperty.name(), metaProperty.isEnumType() ? camp::enumType : QtVariantConverter::type(metaProperty.type()))
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
        return QtVariantConverter::toValue(m_metaProperty.read(object.get<T*>()));
    }

    /**
     * \brief Set the value of the property for the given object
     *
     * \param object Object to write
     * \param value New value of the property
     */
    virtual void setValue(const camp::UserObject& object, const camp::Value& value) const
    {
        m_metaProperty.write(object.get<T*>(), QtVariantConverter::fromValue(value));
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
