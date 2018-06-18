# - Find AADDLib
# Find the AADDLib libraries (AADDLib)
#
#  This module defines the following variables:
#     AADDLIB_FOUND       - True if AADDLIB_INCLUDE_DIR & AADDLIB_LIBRARY are found
#     AADDLIB_LIBRARIES   - Set when AADDLIB_LIBRARY is found
#     AADDLIB_INCLUDE_DIRS - Set when AADDLIB_INCLUDE_DIR is found
#
#     AADDLIB_INCLUDE_DIR - where to find aadd.h, etc.
#     AADDLIB_LIBRARY     - the extended-affine arithmetic library
#     AADDLIB_VERSION_STRING - the version of aaddlib found (since CMake 2.8.8)
#

#=============================================================================
# Copyright 2012 Carna Radojicic
# Copyright 2012 Carna Radojicic <radojicic@cs.uni-kl.de>
#
# Distributed under the OSI-approved BSD License (the "License");
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================

find_path(AADDLIB_INCLUDE_DIR NAMES aadd.h HINTS $ENV{AADDLIB_BASE} $ENV{AADDLIB}
          PATH_SUFFIXES include
          DOC "The AADDlib include directory"
)

find_library(AADDLIB_LIBRARY NAMES aadd HINTS $ENV{AADDLIB_BASE} $ENV{AADDLIB} 
           PATH_SUFFIXES lib 
          DOC "The AADDlib library"
)

# handle the QUIETLY and REQUIRED arguments and set AADDLIB_FOUND to TRUE if 
# all listed variables are TRUE
include(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(AADDLIB
                                  REQUIRED_VARS AADDLIB_LIBRARY AADDLIB_INCLUDE_DIR
                                  )

if(AADDLIB_FOUND)
  set( AADDLIB_LIBRARIES ${AADDLIB_LIBRARY} )
  set( AADDLIB_INCLUDE_DIRS ${AADDLIB_INCLUDE_DIR} )
endif()

mark_as_advanced(AADDLIB_INCLUDE_DIR AADDLIB_LIBRARY)
