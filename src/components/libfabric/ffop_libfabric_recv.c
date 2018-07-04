
#include "ffop_libfabric.h"
#include "ffop_libfabric_progresser.h"
#include "ffrecv.h"

int ffop_libfabric_recv_post(ffop_t * op, ffop_mem_set_t * mem){
    int ret;

    ffrecv_t * recv = &(op->recv);

#ifdef CHECK_ARGS
    if (op == NULL || op != FFRECV) return FFINVALID_ARG;

    CHECKBUFFER(recv->buffer, mem);

#endif

    void * buffer;
    GETBUFFER(recv->buffer, mem, buffer);

    ret = post_recv(op);

    if (ret == -1)
        return FFERROR;
    
    return ffop_libfabric_progresser_track();
}
