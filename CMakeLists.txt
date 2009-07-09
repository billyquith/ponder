/****************************************************************************
**
** Copyright (C) 2009 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** Commercial Usage
** Licensees holding valid CAMP Commercial licenses may use this file in
** accordance with the CAMP Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and TECHNOGERMA Systems France.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3.txt included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at contact@technogerma.fr.
**
****************************************************************************/


cmake_minimum_required(VERSION 2.6)

# set project's name
project(CAMP)

# setup version numbers
set(VERSION_MAJOR 0)
set(VERSION_MINOR 6)
set(VERSION_PATCH 0)

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
    include/camp/invalidvalue.hpp
    src/invalidvalue.cpp
    include/camp/invalidproperty.hpp
    src/invalidproperty.cpp
    include/camp/invalidobject.hpp
    src/invalidobject.cpp
    include/camp/invalidindex.hpp
    src/invalidindex.cpp
    include/camp/invalidfunction.hpp
    src/invalidfunction.cpp
    include/camp/invalidenumvalue.hpp
    src/invalidenumvalue.cpp
    include/camp/invalidenumname.hpp
    src/invalidenumname.cpp
    include/camp/invalidenum.hpp
    src/invalidenum.cpp
    include/camp/invalidclass.hpp
    src/invalidclass.cpp
    include/camp/invalidargument.hpp
    src/invalidargument.cpp
    include/camp/invalidaccess.hpp
    src/invalidaccess.cpp
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
    include/camp/qt/qtvariantconverter.hpp
    include/camp/qt/qtsimpleproperty.hpp
    include/camp/qt/qtmapper.hpp
    include/camp/qt/qt.hpp
    include/camp/qt/qstring.hpp
    include/camp/qt/qlist.hpp
    include/camp/qt/qvector.hpp
)

# find Boost
find_package(Boost 1.38.0 REQUIRED)

# include files search paths
include_directories(
    ${CAMP_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
)

# instruct CMake to build a shared library from all of the source files
add_library(camp SHARED ${CAMP_SRCS})

# define the export macro
set_target_properties(camp PROPERTIES DEFINE_SYMBOL CAMP_EXPORTS)

# add the test subdirectory, but do not build it by default
#add_subdirectory(test EXCLUDE_FROM_ALL)

###############################
# doc
###############################

find_package(Doxygen)
if(DOXYGEN_FOUND)
    # setup variables to be replaced in the source Doxyfile
    set(DOXYGEN_CONFIGURED_INPUT ${CMAKE_BINARY_DIR}/doc/campapi.doxy)
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/doc/api CACHE PATH "Documentation directory")
    set(DOXYGEN_OUTPUT ${DOXYGEN_OUTPUT_DIR}/html/index.html)

    if(WIN32)
        set(DOXYGEN_GENERATE_MAN NO)
        find_program(HHC_PROGRAM 
            NAMES hhc.exe 
            PATHS "c:/Program Files/HTML Help Workshop"
            DOC "HTML Help Compiler program")
        if(HHC_PROGRAM)
            set(DOXYGEN_GENERATE_HTMLHELP YES)
        else()
            set(DOXYGEN_GENERATE_HTMLHELP NO)
        endif()
    else()
        set(DOXYGEN_GENERATE_MAN NO)
        set(DOXYGEN_GENERATE_HTMLHELP NO)
        set(HHC_PROGRAM)
    endif()

    # configure the source Doxyfile by copying it and replacing all @variables@
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc/api/Doxyfile ${DOXYGEN_CONFIGURED_INPUT} @ONLY)

    # Copy necessary doc files
    file(GLOB image_files ${CMAKE_CURRENT_SOURCE_DIR}/doc/api/images/*)
    foreach(file ${image_files})
        get_filename_component(filename ${file} NAME)
        configure_file(${file} ${DOXYGEN_OUTPUT_DIR}/html/${filename} COPYONLY)
    endforeach()

    # add new targets to build the documentation
    add_custom_command(
        OUTPUT ${DOXYGEN_OUTPUT}
        COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIGURED_INPUT}
        COMMAND ${CMAKE_COMMAND} -E echo "Done."
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS ${DOXYGEN_CONFIGURED_INPUT}
    )

    add_custom_target(apidoc ALL DEPENDS ${DOXYGEN_OUTPUT})

    add_custom_target(apidoc_forced
        COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIGURED_INPUT}
        COMMAND ${CMAKE_COMMAND} -E echo "Done."
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

endif()

###############################
# installing
###############################

if(WIN32 AND NOT UNIX)
    set(INSTALL_MISC_DIR .)
elseif(UNIX)
    set(INSTALL_MISC_DIR share/camp)
endif()

install(DIRECTORY include
    DESTINATION .
    COMPONENT devel
)

install(TARGETS camp
    RUNTIME DESTINATION bin COMPONENT bin
    LIBRARY DESTINATION lib COMPONENT bin 
    ARCHIVE DESTINATION lib COMPONENT devel
)

install(FILES README.txt COPYING.txt LICENSE.GPL3.txt
    DESTINATION ${INSTALL_MISC_DIR}
)

if(DOXYGEN_FOUND)
    install(DIRECTORY ${DOXYGEN_OUTPUT_DIR}
        DESTINATION ${INSTALL_MISC_DIR}/doc
        COMPONENT apidoc
    )
endif()

install(DIRECTORY utils
    DESTINATION ${INSTALL_MISC_DIR}
    COMPONENT utils
)

###############################
# packaging
###############################

# Components setup
set(CPACK_COMPONENTS_ALL bin devel apidoc utils)
set(CPACK_ALL_INSTALL_TYPES Runtime Development)

set(CPACK_COMPONENT_BIN_DISPLAY_NAME "CAMP library")
set(CPACK_COMPONENT_BIN_DESCRIPTION "The CAMP library binaries")
set(CPACK_COMPONENT_BIN_INSTALL_TYPES Runtime Development)

set(CPACK_COMPONENT_DEVEL_DISPLAY_NAME "CAMP development files")
set(CPACK_COMPONENT_DEVEL_DESCRIPTION "C++ header files, import library")
set(CPACK_COMPONENT_DEVEL_DEPENDS bin)
set(CPACK_COMPONENT_DEVEL_GROUP "Development")
set(CPACK_COMPONENT_DEVEL_INSTALL_TYPES Development)

set(CPACK_COMPONENT_APIDOC_DISPLAY_NAME "CAMP API documentation")
set(CPACK_COMPONENT_APIDOC_DESCRIPTION "Documentation for developers")
set(CPACK_COMPONENT_APIDOC_GROUP "Development")
set(CPACK_COMPONENT_APIDOC_INSTALL_TYPES Development)

set(CPACK_COMPONENT_UTILS_DISPLAY_NAME "CAMP utilities")
set(CPACK_COMPONENT_UTILS_DESCRIPTION
    "Provides a FindCAMP.cmake file to be used with project based on CMake"
)
set(CPACK_COMPONENT_UTILS_GROUP "Development")
set(CPACK_COMPONENT_UTILS_DISABLED TRUE)

set(CPACK_COMPONENT_GROUP_DEVELOPMENT_DESCRIPTION "Components related to software development based on CAMP")
set(CPACK_COMPONENT_GROUP_DEVELOPMENT_EXPANDED TRUE)

# Package setup
set(CPACK_PACKAGE_NAME CAMP)
set(CPACK_PACKAGE_VENDOR Tegesoft)
set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CAMP_SOURCE_DIR}/README.txt)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "CAMP is a multi-purpose C++ reflection library")
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_VENDOR}\\\\camp)
set(CPACK_RESOURCE_FILE_LICENSE ${CAMP_SOURCE_DIR}/COPYING.txt)
set(CPACK_RESOURCE_FILE_README ${CAMP_SOURCE_DIR}/README.txt)
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY CAMP)

if(WIN32 AND NOT UNIX)
    set(CPACK_GENERATOR ZIP;NSIS)
elseif(UNIX AND NOT APPLE)
    set(CPACK_GENERATOR TGZ;TBZ2)
elseif(APPLE)
    set(CPACK_GENERATOR TGZ;TBZ2;PackageMaker)
endif()
#set(CPACK_PACKAGE_EXECUTABLES "camptest;Unit Tests")

set(CPACK_SOURCE_GENERATOR TGZ;ZIP)

file(GLOB IGNORED_DOT_FILES ".*")
set(CPACK_SOURCE_IGNORE_FILES
    ${IGNORED_DOT_FILES}
    doc/requirements_specification
    doc/technical_specification
    doc/property_editor
)

# NSIS setup
#set(CPACK_NSIS_MUI_ICON ${CAMP_SOURCE_DIR}/doc/images\\\\logo.ico)
#set(CPACK_NSIS_MUI_UNIICON ${CAMP_SOURCE_DIR}/doc/images\\\\logo.ico)
set(CPACK_PACKAGE_ICON ${CAMP_SOURCE_DIR}/doc/images\\\\logo_package.bmp)
#set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS)
#set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS)
#set(CPACK_NSIS_COMPRESSOR)
#set(CPACK_NSIS_MODIFY_PATH)
#set(CPACK_NSIS_DISPLAY_NAME pouet)
#set(CPACK_NSIS_INSTALLED_ICON_NAME)
set(CPACK_NSIS_HELP_LINK "http://www.tegesoft.com")
set(CPACK_NSIS_URL_INFO_ABOUT "http://www.tegesoft.com")
set(CPACK_NSIS_CONTACT "contact@tegesoft.com")
#set(CPACK_NSIS_CREATE_ICONS_EXTRA)
#set(CPACK_NSIS_DELETE_ICONS_EXTRA)

include(CPack)

