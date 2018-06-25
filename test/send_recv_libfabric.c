#include <stdio.h>
#include <stdlib.h>

#include "ff.h"
#include "libfabric_impl.h"
#include "ctx.h"

#define FFCALL(X) { int ret; if (ret=(X)!=FFSUCCESS) { printf("Error: %i\n", ret); exit(-1); } }

int main(int argc, char * argv[]){

    ffinit(&argc, &argv);

    ffop_h op;

    char * buffer;

    ct->ctx_ptr = (struct ctx *)malloc(sizeof(struct ctx));

    if (ct->opts.dst_addr) {
        buffer = (char *)ct->tx_buf + ct->msg_prefix_size;
        for (int i = 0; i < ct->opts.transfer_size; i++) buffer[i] = ('A' + i);
        FFCALL(ffsend(buffer, ct->opts.transfer_size, FFCHAR, ct->remote_fi_addr, 0, 0, &op));
    }else{
        buffer = (char *)ct->rx_buf + ct->msg_prefix_size;
        FFCALL(ffrecv(buffer, ct->opts.transfer_size, FFCHAR, ct->remote_fi_addr, 0, 0, &op));        
    }
    
    ffop_post(op);   
    ffop_wait(op);
    
    if (!ct->opts.dst_addr) {
        int fail = 0;
        for (int i = 0; i < ct->opts.transfer_size; ++i){
            if (buffer[i] != ('A' + i)){
                printf("Fail!\n");
                fail = 1;
            }
        }

        if (!fail) printf("Success!\n");
    }


    ffop_free(op);

    fffinalize();   
    free(buffer);
    free(ct->ctx_ptr);
    
    return 0;
}
