cmake_minimum_required (VERSION 3.0)

# Our project
project(AADD)

# The version number. Goes into header files as define and to doxygen. 
set (AADD_VERSION_MAJOR 2)
set (AADD_VERSION_MINOR 0)

# DEBUG or NDEBUG (disables assertions)
set (NDEBUG DEBUG)

# We need GLPK installed
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_MODULE_PATH}/cmake-files ${CMAKE_CURRENT_SOURCE_DIR}/cmake-files)
find_package(GLPK REQUIRED)
include_directories(${GLPK_INCLUDE_DIRS})


# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/AADDConfig.h.in"
  "${PROJECT_SOURCE_DIR}/src/aadd_config.h"
  )


# C Compiler and its options
# SET(CMAKE_CXX_COMPILTER g++)
SET(CMAKE_CXX_FLAGS "-std=c++11")



# The directories that are processed
#  - src has AADD and AAF
#  - test has some tests for regressions
add_subdirectory (src)
add_subdirectory (test) 
add_subdirectory (doc)


# Regression tests
include  (CTest)
add_test ( Copy       test/copy)
add_test ( Comparison test/comparison)
add_test ( Waterlevel test/waterlevel) 
set_tests_properties ( Waterlevel PROPERTIES PASS_REGULAR_EXPRESSION "Hashvalue of numLeafs: 7671")
add_test ( example1   test/example1) 
set_tests_properties ( example1 PROPERTIES PASS_REGULAR_EXPRESSION "a is: 
AADD is internal node; cond: -1e-10 + e1*1, index: 1
---------leaf with value 3 + e1*1
 - - - - leaf with value -1 + e1*1")
add_test ( example2   test/example2)
add_test ( example3   test/example3)
