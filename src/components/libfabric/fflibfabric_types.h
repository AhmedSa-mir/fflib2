#ifndef _FFLIBFABRIC_TYPES_H_
#define _FFLIBFABRIC_TYPES_H_

struct ffimpl_data {
    //MPI_Request mpireq;
};

typedef struct ffimpl_data ffimpl_send_data_t;
typedef struct ffimpl_data ffimpl_recv_data_t;

#endif /* _FFLIBFABRIC_TYPES_H_ */
