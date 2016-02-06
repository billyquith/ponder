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


cmake_minimum_required(VERSION 3.0)

# set project's name
project(Ponder)

set(PONDER_SOURCE_DIR ${PROJECT_SOURCE_DIR})

# include project configuration
include(${CMAKE_SOURCE_DIR}/cmake/Config.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/PackageFilename.cmake)

# all source files
file(GLOB_RECURSE SRC_HEADERS
    include/ponder/*.hpp include/ponder/*.inl
)
file(GLOB SRC_SOURCE src/*.cpp)
set(PONDER_SRCS
    ${SRC_HEADERS}
    ${SRC_SOURCE}
)

# include files search paths
include_directories(
    ${PONDER_SOURCE_DIR}/include
)

# generate version.hpp
message("Generate version.hpp")
configure_file(${CMAKE_SOURCE_DIR}/version.in ${CMAKE_SOURCE_DIR}/include/ponder/version.hpp @ONLY)

# instruct CMake to build a shared library from all of the source files
add_library(ponder ${PONDER_SRCS})

# required standard level (needed to pass -std=c++11 to gcc and clang)
set_property(TARGET ponder PROPERTY CXX_STANDARD 11) # fails silently
target_compile_features(ponder PRIVATE cxx_range_for cxx_variable_templates) # required

# define the export macro
if(BUILD_SHARED_LIBS)
    set_target_properties(ponder PROPERTIES DEFINE_SYMBOL PONDER_EXPORTS)
else()
    add_definitions(-DPONDER_STATIC)
endif()

# define d suffix on windows
if(WIN32)
    set_target_properties(ponder PROPERTIES DEBUG_POSTFIX d)
endif()

# gcc 4.x on Windows requires the -static-libgcc linker option to get rid of an extra DLL
if (WIN32 AND CMAKE_COMPILER_IS_GNUCXX)
    if(${GCC_VERSION} MATCHES "4\\..*")
        set_target_properties(ponder PROPERTIES LINK_FLAGS -static-libgcc)
    endif()
endif()

# add the test subdirectory, but do not build it by default
if(BUILD_TEST)
    enable_testing()
    add_subdirectory(test)
endif()

###############################
# doc
###############################

add_subdirectory(doc)

###############################
# support files
###############################
# pkg-config
configure_file(cmake/ponder.pc.in ${CMAKE_BINARY_DIR}/ponder.pc @ONLY)

# cmake
include(CMakePackageConfigHelpers)

configure_package_config_file(cmake/PonderConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/PonderConfig.cmake
    INSTALL_DESTINATION lib/ponder/cmake
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/PonderConfigVersion.cmake
    VERSION ${VERSION_STR}
    COMPATIBILITY AnyNewerVersion
)

###############################
# installing
###############################

install(DIRECTORY include
    DESTINATION .
    COMPONENT devel
)

#install(FILES ${CMAKE_BINARY_DIR}/include/ponder/version.hpp
#    DESTINATION include/ponder
#    COMPONENT devel
#)

install(TARGETS ponder
    EXPORT PonderTargets
    RUNTIME DESTINATION bin COMPONENT bin
    LIBRARY DESTINATION lib COMPONENT bin
    ARCHIVE DESTINATION lib COMPONENT devel
)

install(FILES ${CMAKE_BINARY_DIR}/ponder.pc
    DESTINATION lib/pkgconfig
    COMPONENT devel
)

install(EXPORT PonderTargets
    NAMESPACE ponder::
    DESTINATION lib/ponder/cmake
    COMPONENT devel
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/PonderConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/PonderConfigVersion.cmake
    DESTINATION lib/ponder/cmake
    COMPONENT devel
)

install(FILES README.md COPYING.txt
    DESTINATION ${INSTALL_MISC_DIR}
)

install(DIRECTORY cmake
    DESTINATION ${INSTALL_MISC_DIR}
    COMPONENT utils
)

###############################
# packaging
###############################

include(${PONDER_SOURCE_DIR}/cmake/Packaging.cmake OPTIONAL)
