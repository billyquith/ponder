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

# Package filename setup
include(${CMAKE_SOURCE_DIR}/cmake/PackageFilename.cmake)

# set package file name
string(TOLOWER ${CPACK_PACKAGE_NAME} LOWER_PACKAGE_NAME)
set(CPACK_PACKAGE_FILE_NAME
    ${LOWER_PACKAGE_NAME}-${VERSION_STR}-${PACKAGE_SYSTEM_NAME}-${PACKAGE_COMPILER_NAME})
set(CPACK_SOURCE_PACKAGE_FILE_NAME
    ${LOWER_PACKAGE_NAME}-${VERSION_STR}-src)

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
#set(CPACK_NSIS_MUI_ICON ${CAMP_SOURCE_DIR}/doc/images\\\\logo.ico)
#set(CPACK_NSIS_MUI_UNIICON ${CAMP_SOURCE_DIR}/doc/images\\\\logo.ico)
set(CPACK_PACKAGE_ICON ${CAMP_SOURCE_DIR}/doc/images\\\\logo_package.bmp)
#set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS)
#set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS)
#set(CPACK_NSIS_COMPRESSOR)
#set(CPACK_NSIS_MODIFY_PATH)
set(CPACK_NSIS_DISPLAY_NAME CAMP)
#set(CPACK_NSIS_INSTALLED_ICON_NAME)
set(CPACK_NSIS_HELP_LINK "http://dev.tegesoft.com")
set(CPACK_NSIS_URL_INFO_ABOUT "http://dev.tegesoft.com")
set(CPACK_NSIS_CONTACT "contact@tegesoft.com")
#set(CPACK_NSIS_CREATE_ICONS_EXTRA)
#set(CPACK_NSIS_DELETE_ICONS_EXTRA)

include(CPack)

