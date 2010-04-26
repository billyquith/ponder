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


#ifndef TESTCLASS_HPP
#define TESTCLASS_HPP

#include <ostream>
#include <QObject>
#include <QStringList>
#include <camp/camptype.hpp>
#include <camp/class.hpp>
#include <camp/qt/qt.hpp>
#include <iostream>

struct Test
{
    Test(int i = -1) : x(i) {}
    int x;
};

CAMP_TYPE(Test);
Q_DECLARE_METATYPE(Test)


class PropertiesTest : public QObject
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
    Q_PROPERTY(bool m_bool_read READ getBool)
    Q_PROPERTY(int m_int_read READ getInt)
    Q_PROPERTY(unsigned long m_ulong_read READ getULong)
    Q_PROPERTY(double m_double_read READ getDouble)
    Q_PROPERTY(QString m_string_read READ getString)
    Q_PROPERTY(Enum m_enum_read READ getEnum)

    // read-write
    Q_PROPERTY(bool m_bool READ getBool WRITE setBool)
    Q_PROPERTY(int m_int READ getInt WRITE setInt)
    Q_PROPERTY(unsigned long m_ulong READ getULong WRITE setULong)
    Q_PROPERTY(double m_double READ getDouble WRITE setDouble)
    Q_PROPERTY(QString m_string READ getString WRITE setString)
    Q_PROPERTY(Enum m_enum READ getEnum WRITE setEnum)

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

CAMP_TYPE_NONCOPYABLE(PropertiesTest);

CAMP_TYPE(PropertiesTest::Enum);
Q_DECLARE_METATYPE(PropertiesTest::Enum)


class FunctionsTest : public QObject
{
    Q_OBJECT

public Q_SLOTS:

    void          f1()                                                           {}
    bool          f2(bool b)                                                     {return b;}
    int           f3(int i1, int i2)                                             {return i1 + i2;}
    unsigned long f4(unsigned long l1, unsigned long l2, unsigned long l3)       {return l1 + l2 + l3;}
    double        f5(double d1, double d2, double d3, double d4)                 {return d1 + d2 + d3 + d4;}
    QString       f6(QString s1, QString s2, QString s3, QString s4, QString s5) {return s1 + s2 + s3 + s4 + s5;}
};

CAMP_TYPE_NONCOPYABLE(FunctionsTest);


// So that the print_log_value function of boost.test is happy
inline std::ostream& operator <<(std::ostream& stream, const QString& str)
{
    return stream << str.toStdString();
}

#endif // TESTCLASS_HPP
