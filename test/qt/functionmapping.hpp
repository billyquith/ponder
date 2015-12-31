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

#ifndef PONDERTEST_QT_FUNCTIONMAPPING_HPP
#define PONDERTEST_QT_FUNCTIONMAPPING_HPP

#ifndef Q_MOC_RUN
#include <ponder/camptype.hpp>
#include <ponder/qt/qt.hpp>
#include <ponder/class.hpp>
#endif
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
        ponder::Class::declare<MyClass>("FunctionMappingTest::MyClass")
            .external<camp_ext::QtMapper>();
    }
}

PONDER_AUTO_TYPE_NONCOPYABLE(FunctionMappingTest::MyClass, &FunctionMappingTest::declare);

// To make the boost.test logger happy
inline std::ostream& operator<<(std::ostream& stream, const QString& string)
{
    return stream << string.toStdString();
}

#endif // PONDERTEST_QT_FUNCTIONMAPPING_HPP
