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
