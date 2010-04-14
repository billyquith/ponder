/****************************************************************************
**
** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** CAMP is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** 
** CAMP is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public License
** along with CAMP.  If not, see <http://www.gnu.org/licenses/>.
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
