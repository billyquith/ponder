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


#ifndef PONDER_QT_QTHELPER_HPP
#define PONDER_QT_QTHELPER_HPP


#include <ponder/value.hpp>
#include <QVariant>


namespace ponder_ext
{
/**
 * \brief Utility class to perform various conversions between Ponder and Qt
 */
class QtHelper
{
public:

    /**
     * \brief Extract the Ponder type of a Qt type in string form
     *
     * \param typeName Qt type
     *
     * \return Corresponding Ponder type
     */
    static ponder::Type type(const char* typeName)
    {
        return type(variantType(QMetaType::type(typeName)));
    }

    /**
     * \brief Convert a QVariant type to a Ponder type
     *
     * \param theType Source QVariant type to convert
     *
     * \return \a theType converted to a ponder::Type
     */
    static ponder::Type type(QVariant::Type theType)
    {
        // We add an extra conversion to QVariant::Type because variant.type() may return metatypes
        // that do not exist in QVariant (such as QMetaType::Long which is considered a user type)
        switch (variantType(theType))
        {
            case QVariant::Invalid:   return ponder::noType;
            case QVariant::Bool:      return ponder::boolType;
            case QVariant::Char:      return ponder::intType;
            case QVariant::Int:       return ponder::intType;
            case QVariant::LongLong:  return ponder::intType;
            case QVariant::UInt:      return ponder::intType;
            case QVariant::ULongLong: return ponder::intType;
            case QVariant::Double:    return ponder::realType;
            case QVariant::String:    return ponder::stringType;
            default:                  return ponder::noType;
        }
    }

    /**
     * \brief Map a Qt metatype to a QVariant type
     *
     * \param metaType Source metatype to map
     *
     * \return \a type converted to a WVariant::Type
     */
    static QVariant::Type variantType(int metaType)
    {
        switch (metaType)
        {
            case QMetaType::Void:         return QVariant::Invalid;
            case QMetaType::Bool:         return QVariant::Bool;
            case QMetaType::Int:          return QVariant::Int;
            case QMetaType::UInt:         return QVariant::UInt;
            case QMetaType::Double:       return QVariant::Double;
            case QMetaType::QChar:        return QVariant::Char;
            case QMetaType::QString:      return QVariant::String;
            case QMetaType::QByteArray:   return QVariant::ByteArray;
            case QMetaType::VoidStar:     return QVariant::Invalid;
            case QMetaType::Long:         return QVariant::Int;
            case QMetaType::LongLong:     return QVariant::LongLong;
            case QMetaType::Short:        return QVariant::Int;
            case QMetaType::Char:         return QVariant::Char;
            case QMetaType::ULong:        return QVariant::UInt;
            case QMetaType::ULongLong:    return QVariant::ULongLong;
            case QMetaType::UShort:       return QVariant::UInt;
            case QMetaType::UChar:        return QVariant::Char;
            case QMetaType::Float:        return QVariant::Double;
            case QMetaType::QObjectStar:  return QVariant::UserType;
            case QMetaType::QWidgetStar:  return QVariant::UserType;
            case QMetaType::QCursor:      return QVariant::Cursor;
            case QMetaType::QDate:        return QVariant::Date;
            case QMetaType::QSize:        return QVariant::Size;
            case QMetaType::QTime:        return QVariant::Time;
            case QMetaType::QVariantList: return QVariant::UserType;
            case QMetaType::QPolygon:     return QVariant::Polygon;
            case QMetaType::QColor:       return QVariant::Color;
            case QMetaType::QSizeF:       return QVariant::SizeF;
            case QMetaType::QRectF:       return QVariant::RectF;
            case QMetaType::QLine:        return QVariant::Line;
            case QMetaType::QTextLength:  return QVariant::TextLength;
            case QMetaType::QStringList:  return QVariant::StringList;
            case QMetaType::QVariantMap:  return QVariant::UserType;
            case QMetaType::QVariantHash: return QVariant::UserType;
            case QMetaType::QIcon:        return QVariant::Icon;
            case QMetaType::QPen:         return QVariant::Pen;
            case QMetaType::QLineF:       return QVariant::LineF;
            case QMetaType::QTextFormat:  return QVariant::TextFormat;
            case QMetaType::QRect:        return QVariant::Rect;
            case QMetaType::QPoint:       return QVariant::Point;
            case QMetaType::QUrl:         return QVariant::Url;
            case QMetaType::QRegExp:      return QVariant::RegExp;
            case QMetaType::QDateTime:    return QVariant::DateTime;
            case QMetaType::QPointF:      return QVariant::UserType;
            case QMetaType::QPalette:     return QVariant::Palette;
            case QMetaType::QFont:        return QVariant::Font;
            case QMetaType::QBrush:       return QVariant::Brush;
            case QMetaType::QRegion:      return QVariant::Region;
            case QMetaType::QBitArray:    return QVariant::BitArray;
            case QMetaType::QImage:       return QVariant::Image;
            case QMetaType::QKeySequence: return QVariant::KeySequence;
            case QMetaType::QSizePolicy:  return QVariant::SizePolicy;
            case QMetaType::QPixmap:      return QVariant::Pixmap;
            case QMetaType::QLocale:      return QVariant::Locale;
            case QMetaType::QBitmap:      return QVariant::Bitmap;
            case QMetaType::QMatrix:      return QVariant::Matrix;
            case QMetaType::QTransform:   return QVariant::Transform;
            case QMetaType::User:         return QVariant::UserType;
            default:                      return QVariant::UserType;
        }
    }

    /**
     * \brief Convert a Ponder value to a QVariant
     *
     * \param value Source ponder::Value to convert
     *
     * \return \a value converted to a QVariant
     */
    static QVariant valueToVariant(const ponder::Value& value)
    {
        switch (value.type())
        {
            default:
            case ponder::noType:     return QVariant();
            case ponder::boolType:   return QVariant(value.to<bool>());
            case ponder::intType:    return QVariant(value.to<int>());
            case ponder::realType:   return QVariant(value.to<double>());
            case ponder::stringType: return QVariant(value.to<QString>());
            case ponder::enumType:   return QVariant(value.to<int>());
            case ponder::userType:   return QVariant();
        }
    }

    /**
     * \brief Convert a QVariant to a Ponder value
     *
     * \param variant Source QVariant to convert
     *
     * \return \a variant converted to a ponder::Value
     */
    static ponder::Value variantToValue(const QVariant& variant)
    {
        // We add an extra conversion to QVariant::Type because variant.type() may return metatypes
        // that do not exist in QVariant (such as QMetaType::Long which is considered a user type)
        switch (variantType(variant.type()))
        {
            default:
            case QVariant::Invalid:   return ponder::Value();
            case QVariant::Bool:      return ponder::Value(variant.toBool());
            case QVariant::Char:      return ponder::Value(variant.toInt());
            case QVariant::Int:       return ponder::Value(variant.toInt());
            case QVariant::LongLong:  return ponder::Value(variant.toLongLong());
            case QVariant::UInt:      return ponder::Value(variant.toUInt());
            case QVariant::ULongLong: return ponder::Value(variant.toULongLong());
            case QVariant::Double:    return ponder::Value(variant.toDouble());
            case QVariant::String:    return ponder::Value(variant.toString());
        }
    }
};

} // namespace ponder_ext


#endif // PONDER_QT_QTHELPER_HPP
