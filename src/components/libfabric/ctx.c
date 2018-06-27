#include "ctx.h"

// mark the operation as completed
void op_complete(struct ctx *context, ffop_t ** ready_list) {

    FFOP_COMPLETED(context->op);
    FFOP_ENQUEUE(context->op, ready_list);
}
