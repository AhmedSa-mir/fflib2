
#include "ff.h"
#include "ffinternal.h"
#include "components/gcomp/ffgcomp.h"
#include "fflibfabric.h"
#include "ctx.h"


int fflibfabric_init(int * argc, char *** argv){

    // Initialize libfabric connection
    int ret = libfabric_init(*argc, *(argv));

    if(!ret)
        return FFERROR;

    //initialize the generic computation (gcomp) component
    ffgcomp_init();

    return FFSUCCESS;
}

int fflibfabric_finalize(){

    // fi_close() ?
    mr_release();

    ffgcomp_finalize();

    return FFSUCCESS;
}

int fflibfabric_get_rank(int * rank){
    *rank = get_locality_id();
    return FFSUCCESS;
}

int fflibfabric_get_size(int * size){
    *size = 2;	// To be changed
    return FFSUCCESS;
}
