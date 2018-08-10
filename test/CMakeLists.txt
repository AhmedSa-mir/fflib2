#--------------------------------------------------
# setup tests
#--------------------------------------------------
if(FFLIB_HAVE_MPI OR FFLIB_HAVE_LIBFABRIC)
  set(TEST_C_SRCS
  )
endif()

set(TEST_CPP_SRCS
)

#--------------------------------------------------
# files that depend on which backend we are using
if(FFLIB_HAVE_MPI)
  set(TEST_C_SRCS ${TEST_C_SRCS}
    send_recv
    ping_pong_dep
    #  custom_computation
    ping_pong_schedule
    allreduce
  )
endif()

if(FFLIB_HAVE_LIBFABRIC)
  set(TEST_C_SRCS ${TEST_C_SRCS}
    send_recv_libfabric
    ping_pong_sched_libfabric
    allreduce_libfabric
  )
endif()

if(FFLIB_HAVE_HPX)
  set(TEST_CPP_SRCS ${TEST_CPP_SRCS}
    hpx_fflib_1
  )
endif()

#--------------------------------------------------
include_directories(${MPI_C_INCLUDE_PATH})
message("include_directories(${MPI_INCLUDE_PATH})")

foreach(test ${TEST_C_SRCS})
  add_executable(${test} ${test}.c)
  target_link_libraries(${test} fflib)
endforeach()

foreach(test ${TEST_CPP_SRCS})
  add_executable(${test} ${test}.cpp)
  target_link_libraries(${test} fflib)
  if (FFLIB_HAVE_HPX)
      hpx_setup_target(${test})
  endif()
endforeach()


