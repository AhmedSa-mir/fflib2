
#include "ffop_libfabric_progresser.h"
#include "utils/ffarman.h"
//#include "../utils/fflock.h"

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

int ffop_libfabric_progresser_track(ffop_t * op){

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

    int ret = 0;

    // Look if we received something
    ret = check_rx_completions();

    if (!ret) return FFSUCCESS;
    else if (ret == 1) {

        // Look if some transfer completed
        ret = check_tx_completions();

        if (!ret) return FFSUCCESS;
        else if (ret == 1) return FFSUCCESS;
        else return FFERROR;
    }
    else return FFERROR;
}
