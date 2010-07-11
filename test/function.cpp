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

#include "function.hpp"
#include <camp/classget.hpp>
#include <camp/enumget.hpp>
#include <camp/function.hpp>
#include <camp/errors.hpp>
#include <boost/test/unit_test.hpp>

using namespace FunctionTest;

//-----------------------------------------------------------------------------
struct FunctionFixture
{
    FunctionFixture()
    {
        const camp::Class& metaclass = camp::classByType<MyClass>();
        for (int i = 1; i < 23; ++i)
        {
            if (i != 14 && i != 17) // remove when fixed
                functions[i] = &metaclass.function("f" + boost::lexical_cast<std::string>(i));
        }
    }

    const camp::Function* functions[23];
};

//-----------------------------------------------------------------------------
//                         Tests for camp::Function
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(FUNCTION, FunctionFixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(returnType)
{
    BOOST_CHECK_EQUAL(functions[1]->returnType(),  camp::noType);
    BOOST_CHECK_EQUAL(functions[2]->returnType(),  camp::intType);
    BOOST_CHECK_EQUAL(functions[3]->returnType(),  camp::stringType);
    BOOST_CHECK_EQUAL(functions[4]->returnType(),  camp::userType);
    BOOST_CHECK_EQUAL(functions[5]->returnType(),  camp::userType);
    BOOST_CHECK_EQUAL(functions[6]->returnType(),  camp::noType);
    BOOST_CHECK_EQUAL(functions[7]->returnType(),  camp::userType);
    BOOST_CHECK_EQUAL(functions[8]->returnType(),  camp::noType);
    BOOST_CHECK_EQUAL(functions[9]->returnType(),  camp::noType);
    BOOST_CHECK_EQUAL(functions[10]->returnType(), camp::noType);
    BOOST_CHECK_EQUAL(functions[11]->returnType(), camp::noType);
    BOOST_CHECK_EQUAL(functions[12]->returnType(), camp::noType);
    BOOST_CHECK_EQUAL(functions[13]->returnType(), camp::noType);
    //BOOST_CHECK_EQUAL(functions[14]->returnType(), camp::noType);
    BOOST_CHECK_EQUAL(functions[15]->returnType(), camp::noType);
    BOOST_CHECK_EQUAL(functions[16]->returnType(), camp::intType);
    //BOOST_CHECK_EQUAL(functions[17]->returnType(), camp::noType);
    BOOST_CHECK_EQUAL(functions[18]->returnType(), camp::noType);
    BOOST_CHECK_EQUAL(functions[19]->returnType(), camp::noType);
    BOOST_CHECK_EQUAL(functions[20]->returnType(), camp::intType);
    BOOST_CHECK_EQUAL(functions[21]->returnType(), camp::intType);
    BOOST_CHECK_EQUAL(functions[22]->returnType(), camp::intType);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(argCount)
{
    BOOST_CHECK_EQUAL(functions[1]->argCount(),  0);
    BOOST_CHECK_EQUAL(functions[2]->argCount(),  1);
    BOOST_CHECK_EQUAL(functions[3]->argCount(),  0);
    BOOST_CHECK_EQUAL(functions[4]->argCount(),  0);
    BOOST_CHECK_EQUAL(functions[5]->argCount(),  0);
    BOOST_CHECK_EQUAL(functions[6]->argCount(),  0);
    BOOST_CHECK_EQUAL(functions[7]->argCount(),  1);
    BOOST_CHECK_EQUAL(functions[8]->argCount(),  0);
    BOOST_CHECK_EQUAL(functions[9]->argCount(),  1);
    BOOST_CHECK_EQUAL(functions[10]->argCount(), 2);
    BOOST_CHECK_EQUAL(functions[11]->argCount(), 3);
    BOOST_CHECK_EQUAL(functions[12]->argCount(), 4);
    BOOST_CHECK_EQUAL(functions[13]->argCount(), 5);
    //BOOST_CHECK_EQUAL(functions[14]->argCount(), 0);
    BOOST_CHECK_EQUAL(functions[15]->argCount(), 0);
    BOOST_CHECK_EQUAL(functions[16]->argCount(), 0);
    //BOOST_CHECK_EQUAL(functions[17]->argCount(), 0);
    BOOST_CHECK_EQUAL(functions[18]->argCount(), 0);
    BOOST_CHECK_EQUAL(functions[19]->argCount(), 0);
    BOOST_CHECK_EQUAL(functions[20]->argCount(), 1);
    BOOST_CHECK_EQUAL(functions[21]->argCount(), 1);
    BOOST_CHECK_EQUAL(functions[22]->argCount(), 1);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(argType)
{
    BOOST_CHECK_THROW(functions[1]->argType(0),  camp::OutOfRange);
    BOOST_CHECK_EQUAL(functions[2]->argType(0),  camp::intType);
    BOOST_CHECK_THROW(functions[3]->argType(0),  camp::OutOfRange);
    BOOST_CHECK_THROW(functions[4]->argType(0),  camp::OutOfRange);
    BOOST_CHECK_THROW(functions[5]->argType(0),  camp::OutOfRange);
    BOOST_CHECK_THROW(functions[6]->argType(0),  camp::OutOfRange);
    BOOST_CHECK_EQUAL(functions[7]->argType(0),  camp::userType);
    BOOST_CHECK_THROW(functions[8]->argType(0),  camp::OutOfRange);
    BOOST_CHECK_EQUAL(functions[9]->argType(0),  camp::boolType);
    BOOST_CHECK_EQUAL(functions[10]->argType(0), camp::realType);
    BOOST_CHECK_EQUAL(functions[10]->argType(1), camp::realType);
    BOOST_CHECK_EQUAL(functions[11]->argType(0), camp::intType);
    BOOST_CHECK_EQUAL(functions[11]->argType(1), camp::intType);
    BOOST_CHECK_EQUAL(functions[11]->argType(2), camp::intType);
    BOOST_CHECK_EQUAL(functions[12]->argType(0), camp::stringType);
    BOOST_CHECK_EQUAL(functions[12]->argType(1), camp::stringType);
    BOOST_CHECK_EQUAL(functions[12]->argType(2), camp::stringType);
    BOOST_CHECK_EQUAL(functions[12]->argType(3), camp::stringType);
    BOOST_CHECK_EQUAL(functions[13]->argType(0), camp::enumType);
    BOOST_CHECK_EQUAL(functions[13]->argType(1), camp::enumType);
    BOOST_CHECK_EQUAL(functions[13]->argType(2), camp::enumType);
    BOOST_CHECK_EQUAL(functions[13]->argType(3), camp::enumType);
    BOOST_CHECK_EQUAL(functions[13]->argType(4), camp::enumType);
    //BOOST_CHECK_THROW(functions[14]->argType(0), camp::OutOfRange);
    BOOST_CHECK_THROW(functions[15]->argType(0), camp::OutOfRange);
    BOOST_CHECK_THROW(functions[16]->argType(0), camp::OutOfRange);
    //BOOST_CHECK_THROW(functions[17]->argType(0), camp::OutOfRange);
    BOOST_CHECK_THROW(functions[18]->argType(0), camp::OutOfRange);
    BOOST_CHECK_THROW(functions[19]->argType(0), camp::OutOfRange);
    BOOST_CHECK_EQUAL(functions[20]->argType(0), camp::intType);
    BOOST_CHECK_EQUAL(functions[21]->argType(0), camp::intType);
    BOOST_CHECK_EQUAL(functions[22]->argType(0), camp::intType);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(call)
{
    MyClass object;

    BOOST_CHECK_EQUAL(functions[1]->call(object, camp::Args()), camp::Value::nothing);
    BOOST_CHECK_EQUAL(functions[2]->call(object, camp::Args(10)), camp::Value(12));
    BOOST_CHECK_EQUAL(functions[3]->call(object, camp::Args()), camp::Value("3"));
    BOOST_CHECK_EQUAL(functions[4]->call(object, camp::Args()).to<MyType>(), MyType(4));
    BOOST_CHECK_EQUAL(functions[5]->call(object, camp::Args()).to<MyType>(), MyType(5));
    BOOST_CHECK_EQUAL(functions[6]->call(object, camp::Args()), camp::Value::nothing);
    BOOST_CHECK_EQUAL(functions[7]->call(object, camp::Args("hi")), camp::Value("hi"));
    BOOST_CHECK_EQUAL(functions[8]->call(object, camp::Args()), camp::Value::nothing);
    BOOST_CHECK_EQUAL(functions[9]->call(object, camp::Args(true)), camp::Value::nothing);
    BOOST_CHECK_EQUAL(functions[10]->call(object, camp::Args(1., 2.f)), camp::Value::nothing);
    BOOST_CHECK_EQUAL(functions[11]->call(object, camp::Args(1, 2, 3)), camp::Value::nothing);
    BOOST_CHECK_EQUAL(functions[12]->call(object, camp::Args("1", "2", "3", "4")), camp::Value::nothing);
    BOOST_CHECK_EQUAL(functions[13]->call(object, camp::Args(Zero, One, Two, Zero, One)), camp::Value::nothing);
    //BOOST_CHECK_EQUAL(functions[14]->call(object, camp::Args()), camp::Value::nothing);
    BOOST_CHECK_EQUAL(functions[15]->call(object, camp::Args()), camp::Value::nothing);
    BOOST_CHECK_EQUAL(functions[16]->call(object, camp::Args()), camp::Value(16));
    //BOOST_CHECK_EQUAL(functions[17]->call(object, camp::Args(20)), camp::Value::nothing);
    BOOST_CHECK_EQUAL(functions[18]->call(object, camp::Args()), camp::Value::nothing);
    BOOST_CHECK_EQUAL(functions[19]->call(object, camp::Args()), camp::Value::nothing);
    BOOST_CHECK_EQUAL(functions[20]->call(object, camp::Args(10)), camp::Value(10));
    BOOST_CHECK_EQUAL(functions[21]->call(object, camp::Args(10)), camp::Value(30));
    BOOST_CHECK_EQUAL(functions[22]->call(object, camp::Args(10)), camp::Value(60));
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(callNullObject)
{
    BOOST_CHECK_THROW(functions[1]->call(camp::UserObject(), camp::Args()),  camp::NullObject);
    BOOST_CHECK_THROW(functions[3]->call(camp::UserObject(), camp::Args()),  camp::NullObject);
    BOOST_CHECK_THROW(functions[4]->call(camp::UserObject(), camp::Args()),  camp::NullObject);
    BOOST_CHECK_THROW(functions[5]->call(camp::UserObject(), camp::Args()),  camp::NullObject);
    BOOST_CHECK_THROW(functions[6]->call(camp::UserObject(), camp::Args()),  camp::NullObject);
    BOOST_CHECK_THROW(functions[8]->call(camp::UserObject(), camp::Args()),  camp::NullObject);
    BOOST_CHECK_THROW(functions[15]->call(camp::UserObject(), camp::Args()), camp::NullObject);
    BOOST_CHECK_THROW(functions[16]->call(camp::UserObject(), camp::Args()), camp::NullObject);
    BOOST_CHECK_THROW(functions[18]->call(camp::UserObject(), camp::Args()), camp::NullObject);
    BOOST_CHECK_THROW(functions[19]->call(camp::UserObject(), camp::Args()), camp::NullObject);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(callNotEnoughArguments)
{
    MyClass object;

    BOOST_CHECK_THROW(functions[2]->call(object, camp::Args()),                      camp::NotEnoughArguments);
    BOOST_CHECK_THROW(functions[7]->call(object, camp::Args()),                      camp::NotEnoughArguments);
    BOOST_CHECK_THROW(functions[9]->call(object, camp::Args()),                      camp::NotEnoughArguments);
    BOOST_CHECK_THROW(functions[10]->call(object, camp::Args(1.)),                   camp::NotEnoughArguments);
    BOOST_CHECK_THROW(functions[11]->call(object, camp::Args(1, 2)),                 camp::NotEnoughArguments);
    BOOST_CHECK_THROW(functions[12]->call(object, camp::Args("1", "2", "3")),        camp::NotEnoughArguments);
    BOOST_CHECK_THROW(functions[13]->call(object, camp::Args(Zero, One, Two, Zero)), camp::NotEnoughArguments);
    BOOST_CHECK_THROW(functions[20]->call(object, camp::Args()),                     camp::NotEnoughArguments);
    BOOST_CHECK_THROW(functions[21]->call(object, camp::Args()),                     camp::NotEnoughArguments);
    BOOST_CHECK_THROW(functions[22]->call(object, camp::Args()),                     camp::NotEnoughArguments);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(callBadArgument)
{
    MyClass object;
    MyType arg(0);

    BOOST_CHECK_THROW(functions[2]->call(object, camp::Args(arg)),                      camp::BadArgument);
    BOOST_CHECK_THROW(functions[10]->call(object, camp::Args(arg, arg)),                camp::BadArgument);
    BOOST_CHECK_THROW(functions[11]->call(object, camp::Args(arg, arg, arg)),           camp::BadArgument);
    BOOST_CHECK_THROW(functions[12]->call(object, camp::Args(arg, arg, arg, arg)),      camp::BadArgument);
    BOOST_CHECK_THROW(functions[13]->call(object, camp::Args(arg, arg, arg, arg, arg)), camp::BadArgument);
    BOOST_CHECK_THROW(functions[20]->call(object, camp::Args(arg)),                     camp::BadArgument);
    BOOST_CHECK_THROW(functions[21]->call(object, camp::Args(arg)),                     camp::BadArgument);
    BOOST_CHECK_THROW(functions[22]->call(object, camp::Args(arg)),                     camp::BadArgument);
}

BOOST_AUTO_TEST_SUITE_END()
