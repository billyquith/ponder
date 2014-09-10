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

#include "classvisitor.hpp"
#include <camp/classget.hpp>
#include <boost/test/unit_test.hpp>

using namespace ClassVisitorTest;

//-----------------------------------------------------------------------------
//                         Tests for camp::ClassVisitor
//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(CLASSVISITOR)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(visit)
{
    MyClassVisitor visitor;
    camp::classByType<MyClass>().visit(visitor);

    BOOST_CHECK(visitor.simpleVisited);
    BOOST_CHECK(visitor.arrayVisited);
    BOOST_CHECK(visitor.enumVisited);
    BOOST_CHECK(visitor.userVisited);
    BOOST_CHECK(visitor.functionVisited);
}

BOOST_AUTO_TEST_SUITE_END()
