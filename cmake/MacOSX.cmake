###############################################################################
##
## Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
## Contact: Tegesoft Information (contact@tegesoft.com)
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


# Apple MacOS X
# Configure to build universal binaries.
# Build intel 32-bit on 10.4 and intel 32/64-bit on >= 10.5

if(APPLE AND NOT NON_NATIVE_TARGET)
    if(NOT OSX_CONFIG_HAS_BEEN_RUN_BEFORE)
    
        # Make sure the version of CMake is compatible with this script
    
        if(${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} EQUAL 4 
            AND ${CMAKE_PATCH_VERSION} LESS 7)
            message(STATUS
                        "Warning: A critical CMake bug exists in 2.4.6 and below.  "
                        "Trying to build Universal Binaries will result in a compile "
                        "error that seems unrelated.  Either avoid building Universal "
                        "Binaries by changing the CMAKE_OSX_ARCHITECTURES field to list "
                        "only your architecture, or upgrade to a newer version of CMake.")
        endif()
            
        # Determine the correct SDK
            
        if(NOT FORCE_32BIT AND (EXISTS /Developer/SDKs/10.5.sdk OR EXISTS /Developer/SDKs/MacOSX10.6.sdk))
            set(CMAKE_OSX_ARCHITECTURES "i386;x86_64" 
                 CACHE STRING "Build architectures for OSX" FORCE)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmacosx-version-min=10.5"
                 CACHE STRING "Flags used by the compiler during all build types." FORCE)
        else()
            if(EXISTS /Developer/SDKs/MacOSX10.4u.sdk OR FORCE_32BIT)
                set(CMAKE_OSX_ARCHITECTURES "i386" 
                     CACHE STRING "Build architectures for OSX" FORCE)
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmacosx-version-min=10.4 -m32"
                     CACHE STRING "Flags used by the compiler during all build types." FORCE)
            else()
                # No Universal Binary support
            endif()
        endif()
        set(OSX_CONFIG_HAS_BEEN_RUN_BEFORE TRUE)
    endif(NOT OSX_CONFIG_HAS_BEEN_RUN_BEFORE)
endif(APPLE AND NOT NON_NATIVE_TARGET)

if(OSX_ARCHITECTURES_OVERRIDE)
    set(CMAKE_OSX_ARCHITECTURES ${OSX_ARCHITECTURES_OVERRIDE})
endif()

if(APPLE)
    message(STATUS "Compiling for Mac OS X architecture(s): " ${CMAKE_OSX_ARCHITECTURES})
endif()

