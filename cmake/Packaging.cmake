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
set(CPACK_NSIS_HELP_LINK "http://dev.tegesoft.com/camp/")
set(CPACK_NSIS_URL_INFO_ABOUT "http://dev.tegesoft.com/camp/")
set(CPACK_NSIS_CONTACT "camp@tegesoft.com")
#set(CPACK_NSIS_CREATE_ICONS_EXTRA)
#set(CPACK_NSIS_DELETE_ICONS_EXTRA)

include(CPack)

