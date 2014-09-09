/****************************************************************************
**
** This file is part of the CAMP library.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
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

#include <camp/value.hpp>
#include <camp/qt/qt.hpp>
#include <boost/test/unit_test.hpp>
#include <QString>

// To make the boost.test logger happy
inline std::ostream& operator<<(std::ostream& stream, const QString& string)
{
    return stream << string.toStdString();
}

//-----------------------------------------------------------------------------
//                         Tests for camp_ext::ValueMapper<QString>
//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(QSTRING)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(type)
{
    camp::Value value = QString("hello");

    BOOST_CHECK_EQUAL(value.type(), camp::stringType);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(conversionsFromQString)
{
    camp::Value value = QString("-10");

    BOOST_CHECK_EQUAL(value.to<int>(),         -10);
    BOOST_CHECK_CLOSE(value.to<double>(),      -10., 1E-5);
    BOOST_CHECK_EQUAL(value.to<std::string>(), "-10");
    BOOST_CHECK_EQUAL(value.to<QString>(),     "-10");
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(conversionsToQString)
{
    BOOST_CHECK_EQUAL(camp::Value(false).to<QString>(),                QString("0"));
    BOOST_CHECK_EQUAL(camp::Value(10).to<QString>(),                   QString("10"));
    BOOST_CHECK_EQUAL(camp::Value(std::string("hello")).to<QString>(), QString("hello"));
}

BOOST_AUTO_TEST_SUITE_END()
