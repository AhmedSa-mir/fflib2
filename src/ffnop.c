#include "ffnop.h"
#include "ffop.h"

int ffnop(int options, ffop_h * _op){
    ffop_t * op;
    ffop_create(&op);
    *_op = (ffop_h) op;
    op->options = options;

    op->type = FFNOP;

    FFLOG("FFNOP ID: %lu; options: %i\n", op->id, options); 
    return FFSUCCESS;
}

int ffnop_post(ffop_t * op, ffop_mem_set_t * mem){
    return FFCOMPLETED;
}

int ffnop_tostring(ffop_t * op, char * str, int len){
    snprintf(str, len, "N.%lu", op->id); 
    return FFSUCCESS;
}

int ffnop_finalize(ffop_t * op){

    return FFSUCCESS;
}
