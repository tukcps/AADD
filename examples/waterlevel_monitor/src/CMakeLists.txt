add_executable(run waterlevel.cpp )

target_link_libraries( run  ${SYSTEMC_LIBRARIES} ${SYSTEMCAMS_LIBRARIES} ${AADDLIB_LIBRARIES})

#include(CTest)

# add_test( Run1 waterlevel )

