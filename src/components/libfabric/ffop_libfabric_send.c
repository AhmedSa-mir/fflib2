
#include "ffop_libfabric.h"
#include "ffop_libfabric_progresser.h"
#include "ffsend.h"

extern int post_send(ffop_t * op);

int ffop_libfabric_send_post(ffop_t * op, ffop_mem_set_t * mem){
    int ret;

    ffsend_t * send = &(op->send);

#ifdef CHECK_ARGS
    if (op == NULL || op->type != FFSEND) {
        FFLOG_ERROR("Invalid argument!");
        return FFINVALID_ARG;
    }

    CHECKBUFFER(send->buffer, mem);

#endif

    void * buffer;
    GETBUFFER(send->buffer, mem, buffer)

    ret = post_send(op);

    if (ret == -1)
    {
        return FFERROR;
    }

    return ffop_libfabric_progresser_track(op);
}
