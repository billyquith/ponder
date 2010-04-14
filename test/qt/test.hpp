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


/*#include <camp/camptype.hpp>
#include <camp/class.hpp>
#include <camp/enum.hpp>
#include <camp/value.hpp>
#include <camp/function.hpp>
#include <camp/property.hpp>
#include <camp/arrayproperty.hpp>
#include <camp/enumobject.hpp>
#include <camp/userobject.hpp>
#include <camp/classvisitor.hpp>
#include <camp/simpleproperty.hpp>
#include <camp/arrayproperty.hpp>
#include <camp/enumproperty.hpp>
#include <camp/userproperty.hpp>
#include <camp/observer.hpp>
#include <boost/shared_ptr.hpp>
#include <QObject>
#include <string>*/


#include <ostream>
#include <QObject>
#include <QStringList>
#include <camp/camptype.hpp>
#include <camp/class.hpp>
#include <camp/qt/qt.hpp>


class MapperTest : public QObject
{
    Q_OBJECT

public :

    // read only
    Q_PROPERTY(bool m_bool_read READ getBool)
    Q_PROPERTY(int m_int_read READ getInt)
    Q_PROPERTY(unsigned int m_uint_read READ getUInt)
    Q_PROPERTY(double m_double_read READ getDouble)
    Q_PROPERTY(QString m_string_read READ getString)
    //Q_PROPERTY(QStringList m_stringlist_read READ getStringList) NOT IMPLEMENTED YET

    // read-write
    Q_PROPERTY(bool m_bool READ getBool WRITE setBool)
    Q_PROPERTY(int m_int READ getInt WRITE setInt)
    Q_PROPERTY(unsigned int m_uint READ getUInt WRITE setUInt)
    Q_PROPERTY(double m_double READ getDouble WRITE setDouble)
    Q_PROPERTY(QString m_string READ getString WRITE setString)
    //Q_PROPERTY(QStringList m_stringlist READ getStringList WRITE setStringList) NOT IMPLEMENTED YET

public :

    bool getBool() const {return m_bool;}
    void setBool(bool x) {m_bool = x;}

    int getInt() const {return m_int;}
    void setInt(int x) {m_int = x;}

    unsigned int getUInt() const {return m_uint;}
    void setUInt(unsigned int x) {m_uint = x;}

    double getDouble() const {return m_double;}
    void setDouble(double x) {m_double = x;}

    QString getString() const {return m_string;}
    void setString(QString x) {m_string = x;}

    QStringList getStringList() const {return m_stringlist;}
    void setStringList(QStringList x) {m_stringlist = x;}

private :

    bool m_bool;
    int m_int;
    unsigned int m_uint;
    double m_double;
    QString m_string;
    QStringList m_stringlist;
};

CAMP_TYPE_NONCOPYABLE(MapperTest);

// So that the print_log_value function of boost.test is happy
inline std::ostream& operator <<(std::ostream& stream, const QString& str)
{
    return stream << str.toStdString();
}

#endif // TESTCLASS_HPP
