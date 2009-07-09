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


#ifndef CAMP_QT_QTVARIANTCONVERTER_HPP
#define CAMP_QT_QTVARIANTCONVERTER_HPP


#include <camp/value.hpp>
#include <QVariant>


namespace camp_ext
{
/**
 * \brief Utility class to perform conversions between camp::Value and QVariant
 */
class QtVariantConverter
{
public:

    /**
     * \brief Convert a QVariant type to a CAMP type
     *
     * \param type Source QVariant type to convert
     *
     * \return \a type converted to a camp::Type
     */
    static camp::Type type(QVariant::Type type)
    {
        switch (type)
        {
            case QVariant::Invalid:   return camp::noType;
            case QVariant::Bool:      return camp::boolType;
            case QVariant::Char:      return camp::intType;
            case QVariant::Int:       return camp::intType;
            case QVariant::LongLong:  return camp::intType;
            case QVariant::UInt:      return camp::intType;
            case QVariant::ULongLong: return camp::intType;
            case QVariant::Double:    return camp::realType;
            case QVariant::String:    return camp::stringType;
            default:                  return camp::noType;
        }
    }

    /**
     * \brief Convert a CAMP value to a QVariant
     *
     * \param value Source camp::Value to convert
     *
     * \return \a value converted to a QVariant
     */
    static QVariant fromValue(const camp::Value& value)
    {
        switch (value.type())
        {
            case camp::noType:     return QVariant();
            case camp::boolType:   return QVariant(value.to<bool>());
            case camp::intType:    return QVariant(value.to<int>());
            case camp::realType:   return QVariant(value.to<double>());
            case camp::stringType: return QVariant(value.to<QString>());
            case camp::enumType:   return QVariant(value.to<int>());
            default:               return QVariant();
        }
    }

    /**
     * \brief Convert a QVariant to a CAMP value
     *
     * \param variant Source QVariant to convert
     *
     * \return \a variant converted to a camp::Value
     */
    static camp::Value toValue(const QVariant& variant)
    {
        switch (variant.type())
        {
            case QVariant::Invalid:   return camp::Value();
            case QVariant::Bool:      return camp::Value(variant.toBool());
            case QVariant::Char:      return camp::Value(variant.toInt());
            case QVariant::Int:       return camp::Value(variant.toInt());
            case QVariant::LongLong:  return camp::Value(variant.toLongLong());
            case QVariant::UInt:      return camp::Value(variant.toUInt());
            case QVariant::ULongLong: return camp::Value(variant.toULongLong());
            case QVariant::Double:    return camp::Value(variant.toDouble());
            case QVariant::String:    return camp::Value(variant.toString());
            default:                  return camp::Value();
        }
    }
};

} // namespace camp_ext


#endif // CAMP_QT_QTVARIANTCONVERTER_HPP
