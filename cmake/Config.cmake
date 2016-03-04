
# setup version numbers
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 2)
set(VERSION_STR "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
message("Project version: ${VERSION_STR}")

# Set the default build type to release with debug info
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo
        CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
    )
endif()

# Add an option for choosing the build type (shared or static)
if(NOT BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS TRUE
        CACHE BOOL "TRUE to build Ponder as a shared library, FALSE to build it as a static library."
    )
endif()

# Set the default test build option to false
if(NOT BUILD_TEST)
    set(BUILD_TEST FALSE
        CACHE BOOL "TRUE to build the unit tests (requires the Boost Test Library), FALSE otherwise."
    )
endif()

# Set the default test build option to false
if(NOT BUILD_TEST_QT)
    set(BUILD_TEST_QT FALSE
        CACHE BOOL "TRUE to build the Qt-specific unit tests (requires the Boost Test Library and Qt 4.5), FALSE otherwise."
    )
endif()

# define install directory for miscelleneous files
if(WIN32 AND NOT UNIX)
    set(INSTALL_MISC_DIR .)
elseif(UNIX)
    set(INSTALL_MISC_DIR share/ponder)
endif()

# setup MacOSX build environment if necessary
include(${CMAKE_SOURCE_DIR}/cmake/MacOSX.cmake)

