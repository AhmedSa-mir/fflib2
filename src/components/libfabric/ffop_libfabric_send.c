
#include "ffop_libfabric.h"
#include "ffop_libfabric_progresser.h"
#include "ffsend.h"
#include "libfabric_impl.h"

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

    ret = fi_send(ct.ep, ct.tx_buf, ct.opts.transfer_size + ct.msg_prefix_size,
                    fi_mr_desc(ct.mr), ct.remote_fi_addr, ct.ctx_ptr);

    if (ret)
        return FFERROR;

    return ffop_libfabric_progresser_track(op);
}
