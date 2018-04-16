###############################################################################
##
## Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
## Contact: Tegesoft Information (contact@tegesoft.com)
##
## This file is part of the CAMP library.
##
## The MIT License (MIT)
##
## Copyright (c) 2009-2014 TEGESO/TEGESOFT
##
## Permission is hereby granted, free of charge, to any person obtaining a copy
## of this software and associated documentation files (the "Software"), to deal
## in the Software without restriction, including without limitation the rights
## to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
## copies of the Software, and to permit persons to whom the Software is
## furnished to do so, subject to the following conditions:
##
## The above copyright notice and this permission notice shall be included in
## all copies or substantial portions of the Software.
##
## THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
## IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
## FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
## AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
## LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
## OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
## THE SOFTWARE.
##
###############################################################################


cmake_minimum_required(VERSION 2.6)

# set project's name
project(PONDER_TEST_QT)

# all source files
set(PONDER_TEST_QT_SRCS
    main.cpp
    functionmapping.cpp
    functionmapping.hpp
    propertymapping.cpp
    propertymapping.hpp
    qstringmapping.cpp
)

# find Boost
find_package(Boost 1.38.0)  # TODO - add Qt tests? port to Catch.

# find Qt4
find_package(Qt4 4.5.0 COMPONENTS QtCore REQUIRED)

# add some useful macros and variables
# (QT_USE_FILE is a variable defined by find_package( Qt4 ) that contains a path to CMake script)
include(${QT_USE_FILE})

# this will run moc
QT4_WRAP_CPP(PONDER_TEST_QT_MOC_SRCS functionmapping.hpp propertymapping.hpp)

# include files search paths
include_directories(
    ${Boost_INCLUDE_DIRS}
)

# no implicit link to boost.test library
add_definitions(-DBOOST_TEST_NO_LIB)

# instruct CMake to build an executable from all of the source files
add_executable(pondertest-qt ${PONDER_TEST_QT_SRCS} ${PONDER_TEST_QT_MOC_SRCS})

# last thing we have to do is to tell CMake what libraries our executable needs
target_link_libraries(pondertest-qt ponder ${QT_LIBRARIES})

# Add the CTest
add_test(pondertest-qt pondertest-qt)

