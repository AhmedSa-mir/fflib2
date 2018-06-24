#ifndef _FFOP_LIBFABRIC_H_
#define _FFOP_LIBFABRIC_H_

#include "ff.h"
#include "ffop_mem.h"

typedef struct ffop ffop_t;

/* Max outstanding operations */
#define FFLIBFABRIC_MAX_REQ 256

int ffop_libfabric_init(ffop_t * op);
int ffop_libfabric_send_post(ffop_t * op, ffop_mem_set_t * mem);
int ffop_libfabric_recv_post(ffop_t * op, ffop_mem_set_t * mem);

#endif /* _FFOP_LIBFABRIC_H_ */
