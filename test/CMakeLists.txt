###############################################################################
##
## Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
## Contact: Technogerma Systems France Information (contact@technogerma.fr)
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
project(CAMP_TEST)

# all source files
set(CAMP_TEST_SRCS
    arrayproperty.cpp
    arrayproperty.hpp
    class.cpp
    class.hpp
    enum.cpp
    enum.hpp
    enumobject.cpp
    enumobject.hpp
    enumproperty.cpp
    enumproperty.hpp
    observer.cpp
    observer.hpp
    tagholder.cpp
    tagholder.hpp
    value.cpp
    value.hpp
)

# find Boost
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.38.0 COMPONENTS unit_test_framework REQUIRED)

# include files search paths
include_directories(
    ${CAMP_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
)

# linker search paths
link_directories(
    ${CAMP_BINARY_DIR}
)

# instruct CMake to build an executable from all of the source files
add_executable(camptest ${CAMP_TEST_SRCS})

# last thing we have to do is to tell CMake what libraries our executable needs,
target_link_libraries(camptest camp ${Boost_LIBRARIES})

# add the qt subdirectory, but do not build it by default
if(BUILD_TEST_QT)
    add_subdirectory(qt)
endif()

