#include "ffinternal.h"
#include "ffop.h"

#include "rdma/fabric.h"

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
