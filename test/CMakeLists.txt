#--------------------------------------------------
# find MPI
#--------------------------------------------------
find_package(MPI REQUIRED)

#--------------------------------------------------
# setup tests
#--------------------------------------------------
set(TEST_C_SRCS
  send_recv
  #send_recv_libfabric
  ping_pong_dep
  #  custom_computation
  ping_pong_schedule
  ping_pong_sched_libfabric
  #allreduce
  allreduce_libfabric
)

set(TEST_CPP_SRCS
)

include_directories(${MPI_C_INCLUDE_PATH})
message("include_directories(${MPI_INCLUDE_PATH})")

foreach(test ${TEST_C_SRCS})
  add_executable(${test} ${test}.c)
  target_link_libraries(${test} fflib)
endforeach()

foreach(test ${TEST_CPP_SRCS})
  add_executable(${test} ${test}.cpp)
  target_link_libraries(${test} fflib)
endforeach()


