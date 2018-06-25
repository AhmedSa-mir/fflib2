#--------------------------------------------------
# setup tests
#--------------------------------------------------
if (FFLIB_HAVE_MPI)
  set(TEST_C_SRCS
    send_recv
    ping_pong_dep
    simple_computation
    custom_computation
    ping_pong_schedule
  )
endif()

if (FFLIB_HAVE_LIBFABRIC)
  set(TEST_C_SRCS
    send_recv_libfabric
  )
endif()

set(TEST_CPP_SRCS
)

foreach(test ${TEST_C_SRCS})
  add_executable(${test} ${test}.c)
  target_link_libraries(${test} fflib)
endforeach()

foreach(test ${TEST_CPP_SRCS})
  add_executable(${test} ${test}.cpp)
  target_link_libraries(${test} fflib)
endforeach()


