#ifndef _CTX_H_
#define _CTX_H_

#include "ffinternal.h"
#include "ffop.h"

#include "rdma/fabric.h"

#ifdef __cplusplus
extern "C" {
#endif

int  libfabric_init(int , char **);
int  check_tx_completions(ffop_t **);
int  check_rx_completions(ffop_t **);
int  post_send(ffop_t * op);
int  post_recv(ffop_t * op);
void mr_release();


// context provided with the data transfer operation
struct ctx
{
    // context used by the provider
    struct fi_context op_context;

    // associated operation
    ffop_t * op;

//    // constructor
//    ctx(ffop_t * op) {
//        this.op = op;
//    }

//    // destructor
//    ~ctx() {
//        op = NULL;
//    }
};

void op_complete(struct ctx *context, ffop_t ** ready_list);

void set_op(struct ctx *context, ffop_t * op);

#ifdef __cplusplus
}
#endif

#endif
