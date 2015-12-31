/****************************************************************************
**
** This file is part of the CAMP library.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
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

#ifndef PONDERTEST_QT_PROPERTYMAPPING_HPP
#define PONDERTEST_QT_PROPERTYMAPPING_HPP

#ifndef Q_MOC_RUN
#include <ponder/camptype.hpp>
#include <ponder/class.hpp>
#include <ponder/qt/qt.hpp>
#endif
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

    inline void declare()
    {
        ponder::Enum::declare<MyClass::Enum>("PropertyMappingTest::MyClass::Enum")
            .value("one", MyClass::one)
            .value("two", MyClass::two)
            .value("three", MyClass::three);

        ponder::Class::declare<MyClass>("PropertyMappingTest::MyClass")
            .external<camp_ext::QtMapper>();
    }
}

PONDER_AUTO_TYPE_NONCOPYABLE(PropertyMappingTest::MyClass, &PropertyMappingTest::declare)
PONDER_AUTO_TYPE(PropertyMappingTest::MyClass::Enum, &PropertyMappingTest::declare)
Q_DECLARE_METATYPE(PropertyMappingTest::MyClass::Enum)

// To make the boost.test logger happy
inline std::ostream& operator<<(std::ostream& stream, const QString& string)
{
    return stream << string.toStdString();
}

#endif // PONDERTEST_QT_PROPERTYMAPPING_HPP
