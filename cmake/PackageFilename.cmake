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


###############################
# generate package filename
# setup CPACK_PACKAGE_FILE_NAME and CPACK_SOURCE_PACKAGE_FILE_NAME according to
# the current build architecture, system and compiler
# Note that CPACK_PACKAGE_NAME and PONDER_VERSION_STR must be defined
###############################

# detect architecture
include(CheckTypeSize)
check_type_size(void* SIZEOF_VOID_PTR)
if(${SIZEOF_VOID_PTR} MATCHES "^8$")
    set(NATIVE_ARCH 64)
else()
    set(NATIVE_ARCH 32)
endif()

# detect system
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(PACKAGE_SYSTEM_NAME "linux${NATIVE_ARCH}")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(PACKAGE_SYSTEM_NAME "win${NATIVE_ARCH}")
    message(STATUS "MSVC version: ${MSVC_VERSION}")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(PACKAGE_SYSTEM_NAME "osx")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Android")
    set(PACKAGE_SYSTEM_NAME "android")
else()
    message(WARNING "Unsupported system for package generation")
    return()
endif()

#detect compiler
if(CMAKE_COMPILER_IS_GNUCXX)
    set(PACKAGE_COMPILER_NAME "gcc")
    if(WIN32)
        execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "-dumpversion" OUTPUT_VARIABLE GCC_VERSION_OUTPUT)
        string(REGEX REPLACE "([0-9]+\\.[0-9]+).*" "\\1" GCC_VERSION "${GCC_VERSION_OUTPUT}")
        set(PACKAGE_COMPILER_NAME ${PACKAGE_COMPILER_NAME}${GCC_VERSION})
    endif()
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # using regular Clang or AppleClang
    set(PACKAGE_COMPILER_NAME "clang")
elseif(MSVC_VERSION EQUAL 1400)
    set(PACKAGE_COMPILER_NAME "vc2005")
elseif(MSVC_VERSION EQUAL 1500)
    set(PACKAGE_COMPILER_NAME "vc2008")
elseif(MSVC_VERSION EQUAL 1600)
    set(PACKAGE_COMPILER_NAME "vc2010")
elseif(MSVC_VERSION EQUAL 1700)
    set(PACKAGE_COMPILER_NAME "vc2012")
elseif(MSVC_VERSION EQUAL 1800)
    set(PACKAGE_COMPILER_NAME "vc2013")
elseif(MSVC_VERSION EQUAL 1900)
    set(PACKAGE_COMPILER_NAME "vc2015")
elseif((MSVC_VERSION GREATER_EQUAL 1910) AND (MSVC_VERSION LESS 1920))
    set(PACKAGE_COMPILER_NAME "vc2017")
else()
    message(WARNING "Unsupported compiler for package generation")
    return()
endif()
