add_executable( waterlevel main.cpp )

target_link_libraries( waterlevel  ${SYSTEMC_LIBRARIES} ${SYSTEMCAMS_LIBRARIES} ${AADDLIB_LIBRARIES})

#include(CTest)

# add_test( Run1 waterlevel )

