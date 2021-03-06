
#include "ffop_mpi.h"
#include "ffop_mpi_progresser.h"
#include "ffsend.h"

int ffop_mpi_send_post(ffop_t * op, ffop_mem_set_t * mem){
    int res;

    ffsend_t * send = &(op->send);

#ifdef CHECK_ARGS
    if (op==NULL || op->type!=FFSEND) {
        FFLOG_ERROR("Invalid argument!");
        return FFINVALID_ARG;
    }

    CHECKBUFFER(send->buffer, mem);

#endif

    void * buffer;
    GETBUFFER(send->buffer, mem, buffer)

    res = MPI_Isend(buffer, send->buffer.count, 
            datatype_translation_table[send->buffer.datatype], send->peer, 
            send->tag, MPI_COMM_WORLD, &(send->transport.mpireq));

    if (res!=MPI_SUCCESS) {
        FFLOG_ERROR("Error while creating the MPI_Isend!");
        return FFERROR;
    }

    return ffop_mpi_progresser_track(op, send->transport.mpireq);
}

