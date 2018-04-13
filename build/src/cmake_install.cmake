# Install script for directory: /Users/grimm/GIT/AADD-github/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/Users/grimm/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/Users/grimm/GIT/AADD-github/src/aadd_lp_glpk.h"
    "/Users/grimm/GIT/AADD-github/src/aadd_mgr.h"
    "/Users/grimm/GIT/AADD-github/src/aadd_config.h"
    "/Users/grimm/GIT/AADD-github/src/aadd.h"
    "/Users/grimm/GIT/AADD-github/src/aadd_ddbase.h"
    "/Users/grimm/GIT/AADD-github/src/aadd_ddbase_impl.h"
    "/Users/grimm/GIT/AADD-github/src/aadd_off.h"
    "/Users/grimm/GIT/AADD-github/src/aadd_bdd.h"
    "/Users/grimm/GIT/AADD-github/src/aa.h"
    "/Users/grimm/GIT/AADD-github/src/aa_aaf.h"
    "/Users/grimm/GIT/AADD-github/src/aa_exceptions.h"
    "/Users/grimm/GIT/AADD-github/src/aa_interval.h"
    "/Users/grimm/GIT/AADD-github/src/aa_rounding.h"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/grimm/GIT/AADD-github/build/src/libaadd.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libaadd.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libaadd.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libaadd.dylib")
    endif()
  endif()
endif()

