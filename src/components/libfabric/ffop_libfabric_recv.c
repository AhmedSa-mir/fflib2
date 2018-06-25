
#include "ffop_libfabric.h"
#include "ffop_libfabric_progresser.h"
#include "ffrecv.h"
#include "libfabric_impl.h"

int ffop_libfabric_recv_post(ffop_t * op, ffop_mem_set_t * mem){
    int res;

    ffrecv_t * recv = &(op->recv);

#ifdef CHECK_ARGS
    if (op == NULL || op != FFRECV) return FFINVALID_ARG;
    
    CHECKBUFFER(recv->buffer, mem); 
   
#endif

    void * buffer;
    GETBUFFER(recv->buffer, mem, buffer);

    ret = fi_recv(ct->ep, ct->rx_buf, MAX(ct->rx_size, PP_MAX_CTRL_MSG) + ct->msg_prefix_size,
                    fi_mr_desc(ct->mr), 0, ct->ctx_ptr);
    
    if (ret)
        return FFERROR;

    return ffop_mpi_progresser_track();
}
