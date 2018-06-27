#--------------------------------------------------
# find Libfabric
#--------------------------------------------------
find_package(Libfabric REQUIRED)

include_directories(
  ${LIBFABRIC_INCLUDE_DIR}
  libfabric
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(LF_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime/parcelset/v2/fabric_endpoint.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime/parcelset/v2/locality_resolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime/parcelset/v2/parcelhandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime/parcelset/v2/fabric_domain.cpp
)

add_library(ff_libfabric ${LF_SRCS})
set_property(TARGET ff_libfabric PROPERTY CXX_STANDARD 14)

add_subdirectory(test)
