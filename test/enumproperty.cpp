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


#define BOOST_TEST_NO_LIB
#include <boost/test/unit_test.hpp>

#include "enumproperty.hpp"
#include <camp/class.hpp>
#include <camp/enum.hpp>
#include <camp/classget.hpp>
#include <camp/enumget.hpp>
#include <camp/errors.hpp>
#include <camp/enumproperty.hpp>

using namespace EnumPropertyTest;

//-----------------------------------------------------------------------------
struct EnumPropertyFixture
{
    EnumPropertyFixture()
    {
        camp::Enum::declare<MyEnum>("MyEnum");

        camp::Class::declare<MyClass>("MyClass")
            .property("x", &MyClass::x);

        const camp::Class& metaclass = camp::classByType<MyClass>();
        property = &static_cast<const camp::EnumProperty&>(metaclass.property("x"));
    }

    ~EnumPropertyFixture()
    {
        camp::Class::undeclare<MyClass>();
        camp::Enum::undeclare<MyEnum>();
    }

    const camp::EnumProperty* property;
};

//-----------------------------------------------------------------------------
//                         Tests for camp::EnumProperty
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(ENUMPROPERTY, EnumPropertyFixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(type)
{
    BOOST_CHECK_EQUAL(property->type(), camp::enumType);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(getEnum)
{
    BOOST_CHECK(property->getEnum() == camp::enumByType<MyEnum>());
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(get)
{
    BOOST_CHECK_EQUAL(property->get(MyClass(Zero)), camp::Value(Zero));
    BOOST_CHECK_EQUAL(property->get(MyClass(One)),  camp::Value(One));
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(set)
{
    MyClass object(Zero);
    property->set(object, One);

    BOOST_CHECK_EQUAL(property->get(object), camp::Value(One));
}

BOOST_AUTO_TEST_SUITE_END()
