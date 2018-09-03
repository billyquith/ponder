###############################################################################
##
## Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
## Contact: Tegesoft Information (contact@tegesoft.com)
##
## This file is part of the Ponder library, formerly CAMP.
##
## The MIT License (MIT)
##
## Copyright (c) 2009-2014 TEGESO/TEGESOFT
## Copyright (C) 2015-2018 Nick Trout.
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


# set project's name
project(PONDER_TEST)

# all source files
set(PONDER_TEST_SRCS
    test.hpp
    arrayproperty.cpp
    class.cpp
    classvisitor.cpp
    constructor.cpp
    dictionary.cpp
    enum.cpp
    enumclass.cpp
    enumclassobject.cpp
    enumclassproperty.cpp
    enumobject.cpp
    enumproperty.cpp
    function.cpp
    inheritance.cpp
    main.cpp
    mapper.cpp
    property.cpp
    propertyaccess.cpp
    serialise.cpp
    string_view.cpp
    tagholder.cpp
    traits.cpp
    userdata.cpp
    userobject.cpp
    userproperty.cpp
    value.cpp
)

# Ponder, which was CAMP, used to rely on Boost. This is here in case we need to
# test against Boost, which is a useful reference. At some point in the future
# this may be removed.
#
# find Boost, for unit testing
# find_package(Boost 1.38.0)

# include files search paths
# include_directories(
#   ${Boost_INCLUDE_DIRS}
# )

# no implicit link to boost.test library
# add_definitions(-DBOOST_TEST_NO_LIB)

# linker search paths
link_directories(
    ${PONDER_BINARY_DIR}
)

# instruct CMake to build an executable from all of the source files
add_executable(pondertest ${PONDER_TEST_SRCS})

# last thing we have to do is to tell CMake what libraries our executable needs,
target_link_libraries(pondertest ponder)

# - Add the executable as a CTest
add_test(pondertest pondertest)

