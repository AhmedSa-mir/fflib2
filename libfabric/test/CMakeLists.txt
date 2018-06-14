#--------------------------------------------------
# find MPI
#--------------------------------------------------
find_package(MPI REQUIRED)

#--------------------------------------------------
# setup tests
#--------------------------------------------------
set(TEST_C_SRCS
)

set(TEST_CPP_SRCS
  connect_libfabric
)

include_directories(${MPI_C_INCLUDE_PATH})
message("include_directories(${MPI_INCLUDE_PATH})")

foreach(test ${TEST_C_SRCS})
  add_executable(${test} ${test}.c)
  target_link_libraries(${test} fflib ff_libfabric ${LIBFABRIC_LIBRARIES} )
endforeach()

foreach(test ${TEST_CPP_SRCS})
  add_executable(${test} ${test}.cpp)
  target_link_libraries(${test} fflib ff_libfabric ${LIBFABRIC_LIBRARIES} )
endforeach()


