cmake_minimum_required (VERSION 3.0)
project(wlm)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_MODULE_PATH}/cmake-files ${CMAKE_CURRENT_SOURCE_DIR}/cmake-files)

find_package(SystemC REQUIRED)
find_package(SystemCAMS REQUIRED)
find_package(AADDLib REQUIRED)


include_directories(${SYSTEMC_INCLUDE_DIR} ${SYSTEMCAMS_INCLUDE_DIR} ${AADDLIB_INCLUDE_DIRS})

add_subdirectory(src)

include(CTest)

add_test( waterlevel waterlevel )
