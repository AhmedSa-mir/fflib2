#include "ffinternal.h"
#include "ffop.h"

#include "rdma/fabric.h"

// context provided with the data transfer operation 
struct ctx
{
    // context used by the provider
    fi_context op_context;

    // associated operation
    ffop_t * op;



    // constructor
    ctx(ffop_t * op) {
    	this.op = op;
    }

    // destructor
    ~ctx() {
    	op = NULL;
    }

    // mark the operation as completed
    void op_complete(ffop_t ** ready_list) {

    	FFOP_COMPLETED(op);
    	FFOP_ENQUEUE(op, ready_list);
    }
};
