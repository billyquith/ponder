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

#ifndef CAMPTEST_QT_FUNCTIONMAPPING_HPP
#define CAMPTEST_QT_FUNCTIONMAPPING_HPP

#include <camp/camptype.hpp>
#include <camp/qt/qt.hpp>
#include <camp/class.hpp>
#include <QObject>
#include <QString>
#include <iostream>

namespace FunctionMappingTest
{
    class MyClass : public QObject
    {
        Q_OBJECT

    public slots:

        void f1()
        {
        }

        bool f2(bool b)
        {
            return b;
        }

        int f3(int i1, int i2)
        {
            return i1 + i2;
        }

        long f4(long l1, long l2, long l3)
        {
            return l1 + l2 + l3;
        }

        double f5(double d1, double d2, double d3, double d4)
        {
            return d1 + d2 + d3 + d4;
        }

        QString f6(QString s1, QString s2, QString s3, QString s4, QString s5)
        {
            return s1 + s2 + s3 + s4 + s5;
        }
    };

    inline void declare()
    {
        camp::Class::declare<MyClass>("MyClass")
            .external<camp_ext::QtMapper>();
    }
}

CAMP_AUTO_TYPE_NONCOPYABLE(FunctionMappingTest::MyClass, &FunctionMappingTest::declare);

// To make the boost.test logger happy
inline std::ostream& operator<<(std::ostream& stream, const QString& string)
{
    return stream << string.toStdString();
}

#endif // CAMPTEST_QT_FUNCTIONMAPPING_HPP
