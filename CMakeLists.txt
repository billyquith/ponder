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


cmake_minimum_required(VERSION 3.0)

# set project's name
project(CAMP)

# include project configuration
include(${CMAKE_SOURCE_DIR}/cmake/Config.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/PackageFilename.cmake)

# all source files
set(CAMP_SRCS
    include/camp/userproperty.hpp
    src/userproperty.cpp
    include/camp/userobject.hpp
    include/camp/userobject.inl
    src/userobject.cpp
    include/camp/tagholder.hpp
    src/tagholder.cpp
    include/camp/simpleproperty.hpp
    src/simpleproperty.cpp
    include/camp/property.hpp
    src/property.cpp
    include/camp/observer.hpp
    src/observer.cpp
    include/camp/errors.hpp
    src/errors.cpp
    include/camp/function.hpp
    src/function.cpp
    include/camp/error.hpp
    src/error.cpp
    include/camp/enumproperty.hpp
    src/enumproperty.cpp
    include/camp/enumobject.hpp
    include/camp/enumobject.inl
    src/enumobject.cpp
    include/camp/enumbuilder.hpp
    src/enumbuilder.cpp
    include/camp/enum.hpp
    include/camp/enum.inl
    src/enum.cpp
    include/camp/classvisitor.hpp
    src/classvisitor.cpp
    include/camp/class.hpp
    include/camp/class.inl
    src/class.cpp
    include/camp/arrayproperty.hpp
    src/arrayproperty.cpp
    include/camp/args.hpp
    src/args.cpp
    include/camp/value.hpp
    include/camp/value.inl
    src/value.cpp
    include/camp/detail/classmanager.hpp
    src/classmanager.cpp
    include/camp/detail/enummanager.hpp
    src/enummanager.cpp
    include/camp/detail/observernotifier.hpp
    src/observernotifier.cpp
    include/camp/camptype.hpp
    src/camptype.cpp
    include/camp/classcast.hpp
    src/classcast.cpp
    include/camp/type.hpp
    include/camp/enumget.hpp
    include/camp/enumget.inl
    include/camp/constructor.hpp
    include/camp/config.hpp
    include/camp/classget.hpp
    include/camp/classget.inl
    include/camp/classbuilder.hpp
    include/camp/classbuilder.inl
    include/camp/arraymapper.hpp
    include/camp/valuemapper.hpp
    include/camp/valuevisitor.hpp
    include/camp/detail/valueimpl.hpp
    include/camp/detail/userpropertyimpl.hpp
    include/camp/detail/userpropertyimpl.inl
    include/camp/detail/typeid.hpp
    include/camp/detail/simplepropertyimpl.hpp
    include/camp/detail/simplepropertyimpl.inl
    include/camp/detail/returntype.hpp
    include/camp/detail/rawtype.hpp
    include/camp/detail/propertyfactory.hpp
    include/camp/detail/objecttraits.hpp
    include/camp/detail/issmartpointer.hpp
    include/camp/detail/getter.hpp
    include/camp/detail/getter.inl
    include/camp/detail/functiontraits.hpp
    include/camp/detail/functionimpl.hpp
    include/camp/detail/enumpropertyimpl.hpp
    include/camp/detail/enumpropertyimpl.inl
    include/camp/detail/constructorimpl.hpp
    include/camp/detail/callhelper.hpp
    include/camp/detail/arraypropertyimpl.hpp
    include/camp/detail/arraypropertyimpl.inl
    include/camp/detail/yesnotype.hpp
    include/camp/detail/objectholder.hpp
    include/camp/detail/objectholder.inl
    include/camp/detail/valueprovider.hpp
    include/camp/qt/qthelper.hpp
    include/camp/qt/qtsimpleproperty.hpp
    include/camp/qt/qtmapper.hpp
    include/camp/qt/qt.hpp
    include/camp/qt/qstring.hpp
    include/camp/qt/qlist.hpp
    include/camp/qt/qvector.hpp
    include/camp/qt/qtfunction.hpp
)

# find Boost
find_package(Boost 1.38.0 REQUIRED)

# include files search paths
include_directories(
    ${CAMP_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
)

# generate version.hpp
message("Generate version.hpp")
configure_file(${CMAKE_SOURCE_DIR}/version.in ${CMAKE_BINARY_DIR}/version.hpp @ONLY)

# instruct CMake to build a shared library from all of the source files
add_library(camp ${CAMP_SRCS})

# define the export macro
if(BUILD_SHARED_LIBS)
    set_target_properties(camp PROPERTIES DEFINE_SYMBOL CAMP_EXPORTS)
else()
    add_definitions(-DCAMP_STATIC)
endif()

# define d suffix on windows
if(WIN32)
    set_target_properties(camp PROPERTIES DEBUG_POSTFIX d)
endif()

# gcc 4.x on Windows requires the -static-libgcc linker option to get rid of an extra DLL
if (WIN32 AND CMAKE_COMPILER_IS_GNUCXX)
    if(${GCC_VERSION} MATCHES "4\\..*")
        set_target_properties(camp PROPERTIES LINK_FLAGS -static-libgcc)
    endif()
endif()

# add the test subdirectory, but do not build it by default
if(BUILD_TEST)
    add_subdirectory(test)
endif()

###############################
# doc
###############################

add_subdirectory(doc)

###############################
# installing
###############################

install(DIRECTORY include
    DESTINATION .
    COMPONENT devel
)

install(FILES ${CMAKE_BINARY_DIR}/version.hpp
    DESTINATION include/camp
    COMPONENT devel
)

install(TARGETS camp
    RUNTIME DESTINATION bin COMPONENT bin
    LIBRARY DESTINATION lib COMPONENT bin
    ARCHIVE DESTINATION lib COMPONENT devel
)

install(FILES README.txt COPYING.txt LICENSE.LGPL3.txt
    DESTINATION ${INSTALL_MISC_DIR}
)

install(DIRECTORY cmake
    DESTINATION ${INSTALL_MISC_DIR}
    COMPONENT utils
)

###############################
# packaging
###############################

include(${CAMP_SOURCE_DIR}/cmake/Packaging.cmake OPTIONAL)
