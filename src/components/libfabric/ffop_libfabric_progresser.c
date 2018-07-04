
#include "ffop_libfabric_progresser.h"
#include "utils/ffarman.h"
#include "ctx.h"

//#include "../utils/fflock.h"

static ffarman_t   index_manager;

//static fflock_t progress_lock;

int ffop_libfabric_progresser_init(){

    //FFLOCK_INIT(&progress_lock);
    if (ffarman_create(FFLIBFABRIC_MAX_REQ, &index_manager)!=FFSUCCESS){
        return FFERROR;
    }

    return FFSUCCESS;
}

int ffop_libfabric_progresser_finalize(){
    ffarman_free(&index_manager);
    //FFLOCK_FREE(&progress_lock);
    return FFSUCCESS;
}

int ffop_libfabric_progresser_track(){

    uint32_t idx = ffarman_get(&index_manager);

    if (idx < 0){
       FFLOG_ERROR("Too many in-flight MPI operations! (check FFMPI_MAX_REQ)");
       return FFENOMEM;
    }

    return FFSUCCESS;
}


int ffop_libfabric_progresser_progress(ffop_t ** ready_list){

    int ret = 0;

    // Look if we received something
    ret = check_rx_completions(ready_list);

    if (!ret) return FFSUCCESS;
    else if (ret == 1) {

        // Look if some transfer completed
        ret = check_tx_completions(ready_list);

        if (!ret || ret == 1) return FFSUCCESS;
        else return FFERROR;
    }
    else return FFERROR;
}