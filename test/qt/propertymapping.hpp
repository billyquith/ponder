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


#ifndef CAMPTEST_QT_PROPERTYMAPPING_HPP
#define CAMPTEST_QT_PROPERTYMAPPING_HPP

#include <camp/camptype.hpp>
#include <camp/class.hpp>
#include <camp/qt/qt.hpp>
#include <QObject>
#include <QString>
#include <iostream>


namespace PropertyMappingTest
{
    class MyClass : public QObject
    {
        Q_OBJECT

    public:

        enum Enum
        {
            one   = 1,
            two   = 2,
            three = 3
        };
        Q_ENUMS(Enum)

    public :

        // read only
        Q_PROPERTY(bool          m_bool_read   READ getBool)
        Q_PROPERTY(int           m_int_read    READ getInt)
        Q_PROPERTY(unsigned long m_ulong_read  READ getULong)
        Q_PROPERTY(double        m_double_read READ getDouble)
        Q_PROPERTY(QString       m_string_read READ getString)
        Q_PROPERTY(Enum          m_enum_read   READ getEnum)

        // read-write
        Q_PROPERTY(bool          m_bool   READ getBool   WRITE setBool)
        Q_PROPERTY(int           m_int    READ getInt    WRITE setInt)
        Q_PROPERTY(unsigned long m_ulong  READ getULong  WRITE setULong)
        Q_PROPERTY(double        m_double READ getDouble WRITE setDouble)
        Q_PROPERTY(QString       m_string READ getString WRITE setString)
        Q_PROPERTY(Enum          m_enum   READ getEnum   WRITE setEnum)

    public :

        bool getBool() const {return m_bool;}
        void setBool(bool x) {m_bool = x;}

        int getInt() const {return m_int;}
        void setInt(int x) {m_int = x;}

        unsigned long getULong() const {return m_ulong;}
        void setULong(unsigned long x) {m_ulong = x;}

        double getDouble() const {return m_double;}
        void setDouble(double x) {m_double = x;}

        QString getString() const {return m_string;}
        void setString(QString x) {m_string = x;}

        Enum getEnum() const {return m_enum;}
        void setEnum(Enum x) {m_enum = x;}

    private :

        bool m_bool;
        int m_int;
        unsigned long m_ulong;
        double m_double;
        QString m_string;
        Enum m_enum;
    };
}

CAMP_TYPE_NONCOPYABLE(PropertyMappingTest::MyClass);
CAMP_TYPE(PropertyMappingTest::MyClass::Enum);
Q_DECLARE_METATYPE(PropertyMappingTest::MyClass::Enum)

// To make the boost.test logger happy
inline std::ostream& operator<<(std::ostream& stream, const QString& string)
{
    return stream << string.toStdString();
}

#endif // CAMPTEST_QT_PROPERTYMAPPING_HPP
