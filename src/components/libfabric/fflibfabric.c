
#include "ff.h"
#include "ffinternal.h"
#include "fflibfabric.h"


int fflibfabric_init(int * argc, char *** argv){

    int ret = libfabric_init(argc, argv);

    if(!ret)
        return FFERROR;

    //initialize the generic computation (gcomp) component
    ffgcomp_init();

    return FFSUCCESS;
}

int fflibfabric_finalize(){

    // fi_close() ?

    ffgcomp_finalize();

    return FFSUCCESS;
}

int fflibfabric_get_rank(int * rank){
    //MPI_Comm_rank(MPI_COMM_WORLD, rank);
    return FFSUCCESS;
}

int fflibfabric_get_size(int * size){
    //MPI_Comm_size(MPI_COMM_WORLD, size);
    return FFSUCCESS;
}
