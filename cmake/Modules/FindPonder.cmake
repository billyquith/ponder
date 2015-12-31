# Locate Ponder library
# This module defines
#  PONDER_FOUND, if false, do not try to link to Ponder
#  PONDER_LIBRARIES
#  PONDER_INCLUDE_DIR, where to find ponder/version.hpp

FIND_PATH(PONDER_INCLUDE_DIR ponder/version.hpp
  HINTS
  $ENV{PONDER_DIR}
  PATH_SUFFIXES include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
)

FIND_LIBRARY(PONDER_LIBRARY 
  NAMES ponder
  HINTS
  $ENV{PONDER_DIR}
  PATH_SUFFIXES lib64 lib
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
)

SET(PONDER_LIBRARIES "${PONDER_LIBRARY}" CACHE STRING "Ponder Libraries")

INCLUDE(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set PONDER_FOUND to TRUE if 
# all listed variables are TRUE
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PONDER DEFAULT_MSG PONDER_LIBRARIES PONDER_INCLUDE_DIR)

MARK_AS_ADVANCED(PONDER_INCLUDE_DIR PONDER_LIBRARIES PONDER_LIBRARY)

