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

#include "function.hpp"
#include <ponder/classget.hpp>
#include <ponder/enumget.hpp>
#include <ponder/function.hpp>
#include <ponder/errors.hpp>
#include "catch.hpp"

using namespace FunctionTest;

struct FunctionFixture
{
    FunctionFixture()
    {
        const ponder::Class& metaclass = ponder::classByType<MyClass>();
        for (int i = 1; i < 23; ++i)
        {
            if (i != 14 && i != 17) // remove when fixed
                functions[i] = &metaclass.function("f" + boost::lexical_cast<std::string>(i));
        }
    }

    const ponder::Function* functions[23];
};

//-----------------------------------------------------------------------------
//                         Tests for ponder::Function
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(FUNCTION, FunctionFixture)

BOOST_AUTO_TEST_CASE(returnType)
{
    BOOST_CHECK_EQUAL(functions[1]->returnType(),  ponder::noType);
    BOOST_CHECK_EQUAL(functions[2]->returnType(),  ponder::intType);
    BOOST_CHECK_EQUAL(functions[3]->returnType(),  ponder::stringType);
    BOOST_CHECK_EQUAL(functions[4]->returnType(),  ponder::userType);
    BOOST_CHECK_EQUAL(functions[5]->returnType(),  ponder::userType);
    BOOST_CHECK_EQUAL(functions[6]->returnType(),  ponder::noType);
    BOOST_CHECK_EQUAL(functions[7]->returnType(),  ponder::userType);
    BOOST_CHECK_EQUAL(functions[8]->returnType(),  ponder::noType);
    BOOST_CHECK_EQUAL(functions[9]->returnType(),  ponder::noType);
    BOOST_CHECK_EQUAL(functions[10]->returnType(), ponder::noType);
    BOOST_CHECK_EQUAL(functions[11]->returnType(), ponder::noType);
    BOOST_CHECK_EQUAL(functions[12]->returnType(), ponder::noType);
    BOOST_CHECK_EQUAL(functions[13]->returnType(), ponder::noType);
    //BOOST_CHECK_EQUAL(functions[14]->returnType(), ponder::noType);
    BOOST_CHECK_EQUAL(functions[15]->returnType(), ponder::noType);
    BOOST_CHECK_EQUAL(functions[16]->returnType(), ponder::intType);
    //BOOST_CHECK_EQUAL(functions[17]->returnType(), ponder::noType);
    BOOST_CHECK_EQUAL(functions[18]->returnType(), ponder::noType);
    BOOST_CHECK_EQUAL(functions[19]->returnType(), ponder::noType);
    BOOST_CHECK_EQUAL(functions[20]->returnType(), ponder::intType);
    BOOST_CHECK_EQUAL(functions[21]->returnType(), ponder::intType);
    BOOST_CHECK_EQUAL(functions[22]->returnType(), ponder::intType);
}

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

BOOST_AUTO_TEST_CASE(argType)
{
    BOOST_CHECK_THROW(functions[1]->argType(0),  ponder::OutOfRange);
    BOOST_CHECK_EQUAL(functions[2]->argType(0),  ponder::intType);
    BOOST_CHECK_THROW(functions[3]->argType(0),  ponder::OutOfRange);
    BOOST_CHECK_THROW(functions[4]->argType(0),  ponder::OutOfRange);
    BOOST_CHECK_THROW(functions[5]->argType(0),  ponder::OutOfRange);
    BOOST_CHECK_THROW(functions[6]->argType(0),  ponder::OutOfRange);
    BOOST_CHECK_EQUAL(functions[7]->argType(0),  ponder::userType);
    BOOST_CHECK_THROW(functions[8]->argType(0),  ponder::OutOfRange);
    BOOST_CHECK_EQUAL(functions[9]->argType(0),  ponder::boolType);
    BOOST_CHECK_EQUAL(functions[10]->argType(0), ponder::realType);
    BOOST_CHECK_EQUAL(functions[10]->argType(1), ponder::realType);
    BOOST_CHECK_EQUAL(functions[11]->argType(0), ponder::intType);
    BOOST_CHECK_EQUAL(functions[11]->argType(1), ponder::intType);
    BOOST_CHECK_EQUAL(functions[11]->argType(2), ponder::intType);
    BOOST_CHECK_EQUAL(functions[12]->argType(0), ponder::stringType);
    BOOST_CHECK_EQUAL(functions[12]->argType(1), ponder::stringType);
    BOOST_CHECK_EQUAL(functions[12]->argType(2), ponder::stringType);
    BOOST_CHECK_EQUAL(functions[12]->argType(3), ponder::stringType);
    BOOST_CHECK_EQUAL(functions[13]->argType(0), ponder::enumType);
    BOOST_CHECK_EQUAL(functions[13]->argType(1), ponder::enumType);
    BOOST_CHECK_EQUAL(functions[13]->argType(2), ponder::enumType);
    BOOST_CHECK_EQUAL(functions[13]->argType(3), ponder::enumType);
    BOOST_CHECK_EQUAL(functions[13]->argType(4), ponder::enumType);
    //BOOST_CHECK_THROW(functions[14]->argType(0), ponder::OutOfRange);
    BOOST_CHECK_THROW(functions[15]->argType(0), ponder::OutOfRange);
    BOOST_CHECK_THROW(functions[16]->argType(0), ponder::OutOfRange);
    //BOOST_CHECK_THROW(functions[17]->argType(0), ponder::OutOfRange);
    BOOST_CHECK_THROW(functions[18]->argType(0), ponder::OutOfRange);
    BOOST_CHECK_THROW(functions[19]->argType(0), ponder::OutOfRange);
    BOOST_CHECK_EQUAL(functions[20]->argType(0), ponder::intType);
    BOOST_CHECK_EQUAL(functions[21]->argType(0), ponder::intType);
    BOOST_CHECK_EQUAL(functions[22]->argType(0), ponder::intType);
}

BOOST_AUTO_TEST_CASE(call)
{
    MyClass object;

    BOOST_CHECK_EQUAL(functions[1]->call(object, ponder::Args()), ponder::Value::nothing);
    BOOST_CHECK_EQUAL(functions[2]->call(object, ponder::Args(10)), ponder::Value(12));
    BOOST_CHECK_EQUAL(functions[3]->call(object, ponder::Args()), ponder::Value("3"));
    BOOST_CHECK_EQUAL(functions[4]->call(object, ponder::Args()).to<MyType>(), MyType(4));
    BOOST_CHECK_EQUAL(functions[5]->call(object, ponder::Args()).to<MyType>(), MyType(5));
    BOOST_CHECK_EQUAL(functions[6]->call(object, ponder::Args()), ponder::Value::nothing);
    BOOST_CHECK_EQUAL(functions[7]->call(object, ponder::Args("hi")), ponder::Value("hi"));
    BOOST_CHECK_EQUAL(functions[8]->call(object, ponder::Args()), ponder::Value::nothing);
    BOOST_CHECK_EQUAL(functions[9]->call(object, ponder::Args(true)), ponder::Value::nothing);
    BOOST_CHECK_EQUAL(functions[10]->call(object, ponder::Args(1., 2.f)), ponder::Value::nothing);
    BOOST_CHECK_EQUAL(functions[11]->call(object, ponder::Args(1, 2, 3)), ponder::Value::nothing);
    BOOST_CHECK_EQUAL(functions[12]->call(object, ponder::Args("1", "2", "3", "4")), ponder::Value::nothing);
    BOOST_CHECK_EQUAL(functions[13]->call(object, ponder::Args(Zero, One, Two, Zero, One)), ponder::Value::nothing);
    //BOOST_CHECK_EQUAL(functions[14]->call(object, ponder::Args()), ponder::Value::nothing);
    BOOST_CHECK_EQUAL(functions[15]->call(object, ponder::Args()), ponder::Value::nothing);
    BOOST_CHECK_EQUAL(functions[16]->call(object, ponder::Args()), ponder::Value(16));
    //BOOST_CHECK_EQUAL(functions[17]->call(object, ponder::Args(20)), ponder::Value::nothing);
    BOOST_CHECK_EQUAL(functions[18]->call(object, ponder::Args()), ponder::Value::nothing);
    BOOST_CHECK_EQUAL(functions[19]->call(object, ponder::Args()), ponder::Value::nothing);
    BOOST_CHECK_EQUAL(functions[20]->call(object, ponder::Args(10)), ponder::Value(10));
    BOOST_CHECK_EQUAL(functions[21]->call(object, ponder::Args(10)), ponder::Value(30));
    BOOST_CHECK_EQUAL(functions[22]->call(object, ponder::Args(10)), ponder::Value(60));
}

BOOST_AUTO_TEST_CASE(callNullObject)
{
    BOOST_CHECK_THROW(functions[1]->call(ponder::UserObject(), ponder::Args()),  ponder::NullObject);
    BOOST_CHECK_THROW(functions[3]->call(ponder::UserObject(), ponder::Args()),  ponder::NullObject);
    BOOST_CHECK_THROW(functions[4]->call(ponder::UserObject(), ponder::Args()),  ponder::NullObject);
    BOOST_CHECK_THROW(functions[5]->call(ponder::UserObject(), ponder::Args()),  ponder::NullObject);
    BOOST_CHECK_THROW(functions[6]->call(ponder::UserObject(), ponder::Args()),  ponder::NullObject);
    BOOST_CHECK_THROW(functions[8]->call(ponder::UserObject(), ponder::Args()),  ponder::NullObject);
    BOOST_CHECK_THROW(functions[15]->call(ponder::UserObject(), ponder::Args()), ponder::NullObject);
    BOOST_CHECK_THROW(functions[16]->call(ponder::UserObject(), ponder::Args()), ponder::NullObject);
    BOOST_CHECK_THROW(functions[18]->call(ponder::UserObject(), ponder::Args()), ponder::NullObject);
    BOOST_CHECK_THROW(functions[19]->call(ponder::UserObject(), ponder::Args()), ponder::NullObject);
}

BOOST_AUTO_TEST_CASE(callNotEnoughArguments)
{
    MyClass object;

    BOOST_CHECK_THROW(functions[2]->call(object, ponder::Args()),                      ponder::NotEnoughArguments);
    BOOST_CHECK_THROW(functions[7]->call(object, ponder::Args()),                      ponder::NotEnoughArguments);
    BOOST_CHECK_THROW(functions[9]->call(object, ponder::Args()),                      ponder::NotEnoughArguments);
    BOOST_CHECK_THROW(functions[10]->call(object, ponder::Args(1.)),                   ponder::NotEnoughArguments);
    BOOST_CHECK_THROW(functions[11]->call(object, ponder::Args(1, 2)),                 ponder::NotEnoughArguments);
    BOOST_CHECK_THROW(functions[12]->call(object, ponder::Args("1", "2", "3")),        ponder::NotEnoughArguments);
    BOOST_CHECK_THROW(functions[13]->call(object, ponder::Args(Zero, One, Two, Zero)), ponder::NotEnoughArguments);
    BOOST_CHECK_THROW(functions[20]->call(object, ponder::Args()),                     ponder::NotEnoughArguments);
    BOOST_CHECK_THROW(functions[21]->call(object, ponder::Args()),                     ponder::NotEnoughArguments);
    BOOST_CHECK_THROW(functions[22]->call(object, ponder::Args()),                     ponder::NotEnoughArguments);
}

BOOST_AUTO_TEST_CASE(callBadArgument)
{
    MyClass object;
    MyType arg(0);

    BOOST_CHECK_THROW(functions[2]->call(object, ponder::Args(arg)),                      ponder::BadArgument);
    BOOST_CHECK_THROW(functions[10]->call(object, ponder::Args(arg, arg)),                ponder::BadArgument);
    BOOST_CHECK_THROW(functions[11]->call(object, ponder::Args(arg, arg, arg)),           ponder::BadArgument);
    BOOST_CHECK_THROW(functions[12]->call(object, ponder::Args(arg, arg, arg, arg)),      ponder::BadArgument);
    BOOST_CHECK_THROW(functions[13]->call(object, ponder::Args(arg, arg, arg, arg, arg)), ponder::BadArgument);
    BOOST_CHECK_THROW(functions[20]->call(object, ponder::Args(arg)),                     ponder::BadArgument);
    BOOST_CHECK_THROW(functions[21]->call(object, ponder::Args(arg)),                     ponder::BadArgument);
    BOOST_CHECK_THROW(functions[22]->call(object, ponder::Args(arg)),                     ponder::BadArgument);
}

BOOST_AUTO_TEST_SUITE_END()
