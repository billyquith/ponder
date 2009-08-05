###############################################################################
##
## Copyright (C) 2009 TECHNOGERMA Systems France and/or its subsidiary(-ies).
## Contact: Technogerma Systems France Information (contact@technogerma.fr)
##
## This file is part of the CAMP library.
##
## Commercial Usage
## Licensees holding valid CAMP Commercial licenses may use this file in
## accordance with the CAMP Commercial License Agreement provided with the
## Software or, alternatively, in accordance with the terms contained in
## a written agreement between you and TECHNOGERMA Systems France.
##
## GNU General Public License Usage
## Alternatively, this file may be used under the terms of the GNU
## General Public License version 3.0 as published by the Free Software
## Foundation and appearing in the file LICENSE.GPL3.txt included in the
## packaging of this file.  Please review the following information to
## ensure the GNU General Public License version 3.0 requirements will be
## met: http://www.gnu.org/copyleft/gpl.html.
##
## If you are unsure which license is appropriate for your use, please
## contact the sales department at sales@technogerma.fr.
##
###############################################################################


cmake_minimum_required(VERSION 2.6)

# set project's name
project(CAMP)

# setup version numbers
set(VERSION_MAJOR 0)
set(VERSION_MINOR 6)
set(VERSION_PATCH 0)

###############################
# doc
###############################

find_package(Doxygen)
if(DOXYGEN_FOUND)
    # setup variables to be replaced in the source Doxyfile
    set(DOXYGEN_OUTPUT_TYPE php)
    set(DOXYGEN_INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
    set(DOXYGEN_CONFIGURED_INPUT ${CMAKE_BINARY_DIR}/doc/campapi-online.doxy)
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/doc CACHE PATH "Documentation directory")

    set(DOXYGEN_GENERATE_MAN NO)
    set(DOXYGEN_GENERATE_HTMLHELP NO)
    set(HHC_PROGRAM)

    # configure the source Doxyfile by copying it and replacing all @variables@
    configure_file(${DOXYGEN_INPUT_DIR}/doc/api/Doxyfile ${DOXYGEN_CONFIGURED_INPUT} @ONLY)

    # Copy necessary doc files
    file(GLOB image_files ${DOXYGEN_INPUT_DIR}/doc/api/images/*)
    foreach(file ${image_files})
        get_filename_component(filename ${file} NAME)
        configure_file(${file} ${DOXYGEN_OUTPUT_DIR}/${DOXYGEN_OUTPUT_TYPE}/${filename} COPYONLY)
    endforeach()

    add_custom_target(onlinedoc
        COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIGURED_INPUT}
        COMMAND ${CMAKE_COMMAND} -E echo "Done."
        WORKING_DIRECTORY ${DOXYGEN_INPUT_DIR}
    )
endif()
