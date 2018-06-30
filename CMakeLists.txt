cmake_minimum_required(VERSION 3.9)
project(fflib VERSION 2.0.0 DESCRIPTION "fflib 2.0")

#--------------------------------------------------
# Setup Output directories
#--------------------------------------------------
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all Executables."
)
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all Libraries"
)
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all static libraries."
)

#--------------------------------------------------
# Add our cmake subdir to cmake search paths for find_package
#--------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

#--------------------------------------------------
# Option to choose network library provider
#--------------------------------------------------
set(fflib_PROVIDER "mpi" CACHE STRING "Choose underlying library to provide communication functions")
set_property(CACHE fflib_PROVIDER PROPERTY STRINGS mpi libfabric)

#--------------------------------------------------
# Standard linux include dirs
#--------------------------------------------------
include(GNUInstallDirs)

#--------------------------------------------------
# find MPI if needed
#--------------------------------------------------
if (fflib_PROVIDER STREQUAL "mpi")
  find_package(MPI REQUIRED)
  include_directories(
    ${MPI_INCLUDE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
  )
  set(FFLIB_HAVE_MPI 1)
  add_definitions(-DFFLIB_HAVE_MPI)
endif()

#--------------------------------------------------
# find Libfabric if needed
#--------------------------------------------------
if (fflib_PROVIDER STREQUAL "libfabric")
  find_package(Libfabric REQUIRED)
  include_directories(
    ${LIBFABRIC_INCLUDE_DIR}
    libfabric
    ${CMAKE_CURRENT_SOURCE_DIR}
  )
  set(FFLIB_HAVE_LIBFABRIC 1)
  add_definitions(-DFFLIB_HAVE_LIBFABRIC)
endif()

#--------------------------------------------------
# define source files for fflib/etc
#--------------------------------------------------
set(FFLIB_SRCS
  src/ff.c
  src/ffdatatype.c
  src/ffop.c
  src/ffop_mem.c
  src/ffcomp.c
  src/ffprogress.c
  src/ffrecv.c
  src/ffsend.c
  src/ffstorage.c
  src/ffnop.c
  src/ffschedule.c
  #
  src/utils/ffarman.c
  src/colls/ffallreduce.c
)


if (FFLIB_HAVE_MPI)
  set(FFLIB_COMPONENT_SRCS
    src/components/mpi/ffmpi.c
    src/components/mpi/ffop_mpi.c
    src/components/mpi/ffop_mpi_init.c
    src/components/mpi/ffop_mpi_progresser.c
    src/components/mpi/ffop_mpi_recv.c
    src/components/mpi/ffop_mpi_send.c
    #
    src/bindings/mpi_binding.c
  )
  set(FFLIB_LIBRARY_DEPENDENCIES ${MPI_LIBRARIES})
endif()

#if (FFLIB_HAVE_GCOMP)
  set(FFLIB_COMPONENT_C_SRCS ${FFLIB_COMPONENT_SRCS}
    src/components/gcomp/ffgcomp.c
    src/components/gcomp/ffop_gcomp.c
    src/components/gcomp/ffop_gcomp_operator.c
  )
#endif()

if (FFLIB_HAVE_LIBFABRIC)
  set(FFLIB_COMPONENT_SRCS ${FFLIB_COMPONENT_SRCS}
    src/components/libfabric/ctx.c
    src/components/libfabric/fflibfabric.c
    src/components/libfabric/ffop_libfabric_init.c
    src/components/libfabric/ffop_libfabric_progresser.c
    src/components/libfabric/ffop_libfabric_recv.c
    src/components/libfabric/ffop_libfabric_send.c
    #
    src/bindings/libfabric_binding.c
  )

  set(FFLIB_CPP_COMPONENT_SRCS ${FFLIB_CPP_COMPONENT_SRCS}
  	src/components/libfabric/connect_libfabric.cpp
  )

  set_source_files_properties(${FFLIB_COMPONENT_SRCS} PROPERTIES LANGUAGE C)
  set_source_files_properties(${FFLIB_CPP_COMPONENT_SRCS} PROPERTIES LANGUAGE CXX)
  set(FFLIB_LIBRARY_DEPENDENCIES ${LIBFABRIC_LIBRARIES})
endif()

#--------------------------------------------------
# Define the fflib library
#--------------------------------------------------
add_library(fflib SHARED
  ${FFLIB_SRCS}
  ${FFLIB_COMPONENT_SRCS}
  ${FFLIB_CPP_COMPONENT_SRCS}
)

set(CMAKE_C_FLAGS_DEBUG "-DFFDEBUG -g")

set_target_properties(fflib PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 2
    PUBLIC_HEADER src/ff.h)
include_directories(fflib src)

target_link_libraries(fflib ${FFLIB_LIBRARY_DEPENDENCIES})

configure_file(fflib.pc.in fflib.pc @ONLY)
install(TARGETS fflib
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/fflib.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

#--------------------------------------------------
# Thomas Heller's libfabric connection experiment
#--------------------------------------------------
if (FFLIB_HAVE_LIBFABRIC)
  add_subdirectory(libfabric)
endif()


#--------------------------------------------------
# Tests
#--------------------------------------------------
add_subdirectory(test)

