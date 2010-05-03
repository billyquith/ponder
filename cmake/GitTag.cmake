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

# Setup VERSION_MAJOR, VERSION_MINOR and VERSION_PATCH from the last Git tag.
# The tag must be of the form x.y.z where x, y and z are numbers.
# VERSION_STR will contain the whole information returned by "git describe --tags".
#
# Example:
# $ git describe --tags
# 0.6.0-36-g3d1db6f
#
# Then VERSION_MAJOR will be 0, VERSION_MINOR will be 6, VERSION_PATCH will be 0 and VERSION_STR
# will be "0.6.0-36-g3d1db6f".
#
# GITTAG-NOTFOUND will be defined if an error occurs (not a git repository or git executable not found)

# Check if the source are in a Git repository
if (EXISTS ${PROJECT_SOURCE_DIR}/.git/HEAD)
    # Try to find Git executable
    find_program(GIT_EXECUTABLE git)
    if (NOT GIT_EXECUTABLE)
        message(STATUS "Git executable not found")
        set(GITTAG-NOTFOUND True)
    else()
        message(STATUS "Git: ${GIT_EXECUTABLE}")

        # Get git info
        execute_process(
            COMMAND ${GIT_EXECUTABLE} describe --tags
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            OUTPUT_VARIABLE VERSION_STR)
        string(REPLACE "\n" "" VERSION_STR "${VERSION_STR}")
        string(REGEX REPLACE "^([0-9]+)\\.[0-9]+\\.[0-9]+-?.*" "\\1" VERSION_MAJOR "${VERSION_STR}")
        string(REGEX REPLACE "^[0-9]+\\.([0-9])+\\.[0-9]+-?.*" "\\1" VERSION_MINOR "${VERSION_STR}")
        string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+)-?.*" "\\1" VERSION_PATCH "${VERSION_STR}")
        set(GITTAG-NOTFOUND)
    endif()
else()
    set(GITTAG-NOTFOUND True)
endif()

