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


cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0042 NEW) # MACOSX_RPATH

set(VERSION_MAJOR 3)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)
set(VERSION_NOTE "") # e.g. "-alpha", "-beta".

set(PONDER_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
set(PONDER_VERSION_STR "${PONDER_VERSION}${VERSION_NOTE}")

project(ponder
        VERSION ${PONDER_VERSION}
        DESCRIPTION "C++ reflection library"
        LANGUAGES C CXX)
message("Project version: ${PONDER_VERSION_STR}")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PONDER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# include project configuration
include(${PONDER_SOURCE_DIR}/cmake/Config.cmake)
include(${PONDER_SOURCE_DIR}/cmake/PackageFilename.cmake)

# all source files
set(SRC_HEADERS
    include/ponder/args.hpp
    include/ponder/arraymapper.hpp
    include/ponder/arrayproperty.hpp
    include/ponder/class.hpp
    include/ponder/class.inl
    include/ponder/classbuilder.hpp
    include/ponder/classbuilder.inl
    include/ponder/classcast.hpp
    include/ponder/classget.hpp
    include/ponder/classget.inl
    include/ponder/classvisitor.hpp
    include/ponder/config.hpp
    include/ponder/constructor.hpp
    include/ponder/enum.hpp
    include/ponder/enum.inl
    include/ponder/enumbuilder.hpp
    include/ponder/enumget.hpp
    include/ponder/enumget.inl
    include/ponder/enumobject.hpp
    include/ponder/enumobject.inl
    include/ponder/enumproperty.hpp
    include/ponder/error.hpp
    include/ponder/error.inl
    include/ponder/errors.hpp
    include/ponder/function.hpp
    include/ponder/observer.hpp
    include/ponder/pondertype.hpp
    include/ponder/property.hpp
    include/ponder/simpleproperty.hpp
    include/ponder/type.hpp
    include/ponder/userdata.hpp
    include/ponder/userobject.hpp
    include/ponder/userobject.inl
    include/ponder/userproperty.hpp
    include/ponder/value.hpp
    include/ponder/value.inl
    include/ponder/valuemapper.hpp
    include/ponder/valuevisitor.hpp
    include/ponder/version.hpp
    # detail
    include/ponder/detail/arraypropertyimpl.hpp
    include/ponder/detail/arraypropertyimpl.inl
    include/ponder/detail/classmanager.hpp
    include/ponder/detail/constructorimpl.hpp
    include/ponder/detail/dictionary.hpp
    include/ponder/detail/enummanager.hpp
    include/ponder/detail/enumpropertyimpl.hpp
    include/ponder/detail/enumpropertyimpl.inl
    include/ponder/detail/functionimpl.hpp
    include/ponder/detail/functiontraits.hpp
    include/ponder/detail/getter.hpp
    include/ponder/detail/getter.inl
    include/ponder/detail/idtraits.hpp
    include/ponder/detail/objectholder.hpp
    include/ponder/detail/objectholder.inl
    include/ponder/detail/objecttraits.hpp
    include/ponder/detail/observernotifier.hpp
    include/ponder/detail/propertyfactory.hpp
    include/ponder/detail/rawtype.hpp
    include/ponder/detail/simplepropertyimpl.hpp
    include/ponder/detail/simplepropertyimpl.inl
    include/ponder/detail/string_view.hpp
    include/ponder/detail/typeid.hpp
    include/ponder/detail/userpropertyimpl.hpp
    include/ponder/detail/userpropertyimpl.inl
    include/ponder/detail/util.hpp
    include/ponder/detail/valueimpl.hpp
    include/ponder/detail/valueprovider.hpp
    include/ponder/detail/variant.hpp
    include/ponder/detail/variant_recursive_wrapper.hpp
    # Qt
    include/ponder/qt/qlist.hpp
    include/ponder/qt/qstring.hpp
    include/ponder/qt/qt.hpp
    include/ponder/qt/qtfunction.hpp
    include/ponder/qt/qthelper.hpp
    include/ponder/qt/qtmapper.hpp
    include/ponder/qt/qtsimpleproperty.hpp
    include/ponder/qt/qvector.hpp
    # Uses
    include/ponder/uses/uses.hpp
    include/ponder/uses/runtime.hpp
    include/ponder/uses/detail/runtime.hpp
    include/ponder/uses/lua.hpp
    include/ponder/uses/detail/lua.hpp
    # Archive
    include/ponder/uses/serialise.hpp
    include/ponder/uses/serialise.inl
    include/ponder/uses/archive/rapidjson.hpp
    include/ponder/uses/archive/rapidxml.hpp
)

set(SRC_SOURCE
    src/args.cpp
    src/arrayproperty.cpp
    src/class.cpp
    src/classcast.cpp
    src/classmanager.cpp
    src/classvisitor.cpp
    src/enum.cpp
    src/enumbuilder.cpp
    src/enummanager.cpp
    src/enumobject.cpp
    src/enumproperty.cpp
    src/error.cpp
    src/errors.cpp
    src/function.cpp
    src/observer.cpp
    src/observernotifier.cpp
    src/pondertype.cpp
    src/property.cpp
    src/simpleproperty.cpp
    src/type.cpp
    src/userdata.cpp
    src/userobject.cpp
    src/userproperty.cpp
    src/util.cpp
    src/value.cpp
    # Uses
)

source_group("Headers"
             REGULAR_EXPRESSION "include/ponder/.+")
source_group("Source"
             REGULAR_EXPRESSION "src/[^/]+\\.cpp")
source_group("Headers\\Detail"
             REGULAR_EXPRESSION "include/ponder/detail/.+")
source_group("Headers\\Qt"
             REGULAR_EXPRESSION "include/ponder/qt/.+")
source_group("Headers\\Uses"
             REGULAR_EXPRESSION "include/ponder/uses/.+")
source_group("Headers\\Uses\\Archive"
             REGULAR_EXPRESSION "include/ponder/uses/archive/.+")
source_group("Headers\\Uses\\Detail"
             REGULAR_EXPRESSION "include/ponder/uses/detail/.+")
source_group("Headers\\XML"
             REGULAR_EXPRESSION "include/ponder-xml/.+")

set(PONDER_SRCS
    ${SRC_HEADERS}
    ${SRC_SOURCE}
)

###############################
# dependencies
###############################

set(PONDER_DEPS_INCLUDES)
set(PONDER_DEPS_LIBRARIES)

if(USES_RAPIDJSON)
    set(RAPIDJSON_INCLUDES ${PONDER_SOURCE_DIR}/deps/rapidjson/include)
    message(STATUS "Including RapidJSON from ${RAPIDJSON_INCLUDES}")
    set(PONDER_DEPS_INCLUDES ${PONDER_DEPS_INCLUDES} ${RAPIDJSON_INCLUDES})
    include_directories(${RAPIDJSON_INCLUDES})
endif()

if(USES_RAPIDXML)
    set(RAPIDXML_INCLUDES ${PONDER_SOURCE_DIR}/deps/rapidxml/include)
    message(STATUS "Including RapidXML from ${RAPIDXML_INCLUDES}")
    set(PONDER_DEPS_INCLUDES ${PONDER_DEPS_INCLUDES} ${RAPIDXML_INCLUDES})
    include_directories(${RAPIDXML_INCLUDES})
endif()

###############################
# compile environment
###############################

# generate version.hpp
message("Generate version.hpp")
configure_file(${PONDER_SOURCE_DIR}/cmake/version.in
               ${PONDER_SOURCE_DIR}/include/ponder/version.hpp @ONLY)

# instruct CMake to build a shared library from all of the source files
add_library(ponder ${PONDER_SRCS})

# include files search paths
target_include_directories(ponder PUBLIC
    $<BUILD_INTERFACE:${PONDER_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# define the export macro
if(BUILD_SHARED_LIBS)
    set_target_properties(ponder PROPERTIES DEFINE_SYMBOL PONDER_EXPORTS)
else()
    add_definitions(-DPONDER_STATIC)
endif()
message(STATUS "Ponder as shared lib: ${BUILD_SHARED_LIBS}")

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
# documentation
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
    VERSION ${PONDER_VERSION}
    COMPATIBILITY AnyNewerVersion
)

###############################
# installing
###############################

install(DIRECTORY include
    DESTINATION .
    COMPONENT devel
)

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
