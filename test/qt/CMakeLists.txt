###############################################################################
##
## Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
## Contact: Tegesoft Information (contact@tegesoft.com)
##
## This file is part of the CAMP library.
##
## CAMP is free software: you can redistribute it and/or modify
## it under the terms of the GNU Lesser General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
## 
## CAMP is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
## 
## You should have received a copy of the GNU Lesser General Public License
## along with CAMP.  If not, see <http://www.gnu.org/licenses/>.
##
###############################################################################


cmake_minimum_required(VERSION 2.6)

# set project's name
project(CAMP_TEST_QT)

# all source files
set(CAMP_TEST_QT_SRCS
    main.cpp
    functionmapping.cpp
    functionmapping.hpp
    propertymapping.cpp
    propertymapping.hpp
    qstringmapping.cpp
)

# find Boost
find_package(Boost 1.38.0)

# find Qt4
find_package(Qt4 4.5.0 COMPONENTS QtCore REQUIRED)

# add some useful macros and variables
# (QT_USE_FILE is a variable defined by find_package( Qt4 ) that contains a path to CMake script)
include(${QT_USE_FILE})

# this will run moc
QT4_WRAP_CPP(CAMP_TEST_QT_MOC_SRCS functionmapping.hpp propertymapping.hpp)

# include files search paths
include_directories(
    ${CAMP_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
)

# linker search paths
link_directories(
    ${CAMP_BINARY_DIR}
)

# no implicit link to boost.test library
add_definitions(-DBOOST_TEST_NO_LIB)

# instruct CMake to build an executable from all of the source files
add_executable(camptest-qt ${CAMP_TEST_QT_SRCS} ${CAMP_TEST_QT_MOC_SRCS})

# last thing we have to do is to tell CMake what libraries our executable needs
target_link_libraries(camptest-qt camp ${QT_LIBRARIES})

