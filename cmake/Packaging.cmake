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
# packaging
###############################

# Components setup
set(CPACK_COMPONENTS_ALL bin devel apidoc utils)
set(CPACK_ALL_INSTALL_TYPES Runtime Development)

set(CPACK_COMPONENT_BIN_DISPLAY_NAME "Ponder library")
set(CPACK_COMPONENT_BIN_DESCRIPTION "The Ponder library binaries")
set(CPACK_COMPONENT_BIN_INSTALL_TYPES Runtime Development)

set(CPACK_COMPONENT_DEVEL_DISPLAY_NAME "Ponder development files")
set(CPACK_COMPONENT_DEVEL_DESCRIPTION "C++ header files, import library")
set(CPACK_COMPONENT_DEVEL_DEPENDS bin)
set(CPACK_COMPONENT_DEVEL_GROUP "Development")
set(CPACK_COMPONENT_DEVEL_INSTALL_TYPES Development)

set(CPACK_COMPONENT_APIDOC_DISPLAY_NAME "Ponder API documentation")
set(CPACK_COMPONENT_APIDOC_DESCRIPTION "Documentation for developers")
set(CPACK_COMPONENT_APIDOC_GROUP "Development")
set(CPACK_COMPONENT_APIDOC_INSTALL_TYPES Development)

set(CPACK_COMPONENT_UTILS_DISPLAY_NAME "Ponder utilities")
set(CPACK_COMPONENT_UTILS_DESCRIPTION
    "Provides a FindPonder.cmake file to be used with project based on CMake"
)
set(CPACK_COMPONENT_UTILS_GROUP "Development")
set(CPACK_COMPONENT_UTILS_DISABLED TRUE)

set(CPACK_COMPONENT_GROUP_DEVELOPMENT_DESCRIPTION "Components related to software development based on Ponder")
set(CPACK_COMPONENT_GROUP_DEVELOPMENT_EXPANDED TRUE)

# Package setup
set(CPACK_PACKAGE_NAME Ponder)
set(CPACK_PACKAGE_VENDOR Quith)
set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set(CPACK_PACKAGE_DESCRIPTION_FILE ${PONDER_SOURCE_DIR}/README.md)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Ponder is a multi-purpose C++ reflection library")
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_VENDOR}\\\\ponder)
set(CPACK_RESOURCE_FILE_LICENSE ${PONDER_SOURCE_DIR}/COPYING.txt)
set(CPACK_RESOURCE_FILE_README ${PONDER_SOURCE_DIR}/README.md)
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY Ponder)

# Package filename setup
include(${PONDER_SOURCE_DIR}/cmake/PackageFilename.cmake)

# set package file name
string(TOLOWER ${CPACK_PACKAGE_NAME} LOWER_PACKAGE_NAME)
set(CPACK_PACKAGE_FILE_NAME
    ${LOWER_PACKAGE_NAME}-${PONDER_VERSION_STR}-${PACKAGE_SYSTEM_NAME}-${PACKAGE_COMPILER_NAME})
set(CPACK_SOURCE_PACKAGE_FILE_NAME
    ${LOWER_PACKAGE_NAME}-${PONDER_VERSION_STR}-src)

if(WIN32 AND NOT UNIX)
    set(CPACK_GENERATOR ZIP;NSIS)
elseif(UNIX AND NOT APPLE)
    set(CPACK_GENERATOR TGZ;TBZ2)
elseif(APPLE)
    set(CPACK_GENERATOR TGZ;TBZ2;PackageMaker)
endif()

set(CPACK_SOURCE_GENERATOR TGZ;ZIP)

file(GLOB IGNORED_DOT_FILES ".*")
set(CPACK_SOURCE_IGNORE_FILES
    ${IGNORED_DOT_FILES}
)

# NSIS setup
#set(CPACK_NSIS_MUI_ICON ${PONDER_SOURCE_DIR}/doc/images\\\\logo.ico)
#set(CPACK_NSIS_MUI_UNIICON ${PONDER_SOURCE_DIR}/doc/images\\\\logo.ico)
set(CPACK_PACKAGE_ICON ${PONDER_SOURCE_DIR}/doc/images\\\\logo_package.bmp)
#set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS)
#set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS)
#set(CPACK_NSIS_COMPRESSOR)
#set(CPACK_NSIS_MODIFY_PATH)
set(CPACK_NSIS_DISPLAY_NAME Ponder)
#set(CPACK_NSIS_INSTALLED_ICON_NAME)
set(CPACK_NSIS_HELP_LINK "https://github.com/billyquith/ponder")
set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/billyquith/ponder")
#set(CPACK_NSIS_CONTACT "billyquith@github")
#set(CPACK_NSIS_CREATE_ICONS_EXTRA)
#set(CPACK_NSIS_DELETE_ICONS_EXTRA)

include(CPack)

