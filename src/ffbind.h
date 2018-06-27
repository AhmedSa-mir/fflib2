#ifndef _FFBIND_H_
#define _FFBIND_H_

#ifdef FFLIB_HAVE_MPI
#include "bindings/mpi_binding.h"
#elif FFLIB_HAVE_LIBFABRIC
# include "bindings/libfabric_binding.h"
#endif

int ffbind(ffdescr_t * ffdescr);

#endif /* _FFBIND_H_ */
