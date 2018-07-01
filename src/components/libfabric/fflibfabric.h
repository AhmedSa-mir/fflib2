#ifndef _FFLIBFABRIC_H_
#define _FFLIBFABRIC_H_

#include "ffinternal.h"

#ifdef __cplusplus
extern "C" {
#endif

int fflibfabric_bind(ffdescr_t * descr);

int fflibfabric_init(int * argc, char *** argv);
int fflibfabric_finalize();

int fflibfabric_register_op(int op, ffop_descriptor_t * descr);

int fflibfabric_get_rank();
int fflibfabric_get_size();

#ifdef __cplusplus
}
#endif

#endif /* _FFLIBFABRIC_H_ */
