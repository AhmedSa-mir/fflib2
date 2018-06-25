
#include "ffop_libfabric_progresser.h"
#include "utils/ffarman.h"
//#include "../utils/fflock.h"

#include "libfabric_impl.h"

//static ffop_t *    posted_ops[FFMPI_MAX_REQ];
//static MPI_Request requests[FFMPI_MAX_REQ];
static ffarman_t   index_manager;

//static fflock_t progress_lock;

int ffop_libfabric_progresser_init(){

    //FFLOCK_INIT(&progress_lock);
    if (ffarman_create(FFLIBFABRIC_MAX_REQ, &index_manager)!=FFSUCCESS){
        return FFERROR;
    }    

    //for (int i = 0; i < FFLIBFABRIC_MAX_REQ; i++){
    //    requests[i] = MPI_REQUEST_NULL;
    //    posted_ops[i] = NULL;
    //}

    return FFSUCCESS;
}

int ffop_libfabric_progresser_finalize(){
    ffarman_free(&index_manager);
    //FFLOCK_FREE(&progress_lock);
    return FFSUCCESS;
}

int ffop_libfabric_progresser_track(){

    //uint32_t idx = ffarman_get(&index_manager);

    //if (idx < 0){
    //    FFLOG_ERROR("Too many in-flight MPI operations! (check FFMPI_MAX_REQ)");
    //    return FFENOMEM;
    //}

    //posted_ops[idx] = op;
    //requests[idx] = req;
    
    return FFSUCCESS;
}

int ffop_libfabric_progresser_release(uint32_t idx){

    //ffarman_put(&index_manager, idx);
    
    //requests[idx] = MPI_REQUEST_NULL;
    //posted_ops[idx] = NULL;

    return FFSUCCESS;
}



int ffop_libfabric_progresser_progress(ffop_t ** ready_list){

    struct fi_cq_msg_entry entry;
    int ret = 0;

    ret = fi_cq_read(txcq_, &entry, 1);

    if (ret > 0) {
        
        if (entry.flags == (FI_SEND | FI_RECV)) {
            ctx* handler = reinterpret_cast<ctx*>(entry.op_context);
            handler->op_complete(ready_list);
        }
        return FFSUCCESS;
    }
    else if (ret == 0 || ret == -FI_EAGAIN) {
        // return and try again
        return FFSUCCESS;
    }
    else if (ret == -FI_EAVAIL) {
        struct fi_cq_err_entry e = {};
        int err_sz = fi_cq_readerr(txcq_, &e ,0);
        FFLOG_ERROR("txcq read error");
        return FFERROR;
    }
    else {
        FFLOG_ERROR("Unknown error in txcq read");
        return FFERROR;
    }
}


