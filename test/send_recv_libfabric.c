#include <stdio.h>
#include <stdlib.h>

#include "ff.h"
#include "src/components/libfabric/ctx.h"

#define FFCALL(X) { int ret; if (ret=(X)!=FFSUCCESS) { printf("Error: %i\n", ret); exit(-1); } }

int main(int argc, char * argv[]){

    int count = 10, rank, size;	
    char * buffer;

    ffinit(&argc, &argv);

    ffop_h op;

    ffrank(&rank);
    ffsize(&size);

    if (rank == 0) {
        buffer = get_send_buffer();

        for (int i = 0; i < count; i++) 
            buffer[i] = ('A' + i);

        FFCALL(ffsend(buffer, count, FFCHAR, 1, 0, 0, &op));
    }else{
        buffer = get_recv_buffer();

        FFCALL(ffrecv(buffer, count, FFCHAR, 0, 0, 0, &op));      
    }

    ffop_post(op);
    ffop_wait(op);
    
    if (rank == 1) {
        int fail = 0;
        for (int i = 0; i < count; ++i){
            if (buffer[i] != ('A' + i)){
                printf("Fail!\n");
                fail = 1;
            }
        }
        if (!fail) printf("Success!\n");
    }


    ffop_free(op);

    fffinalize();
    
    return 0;
}
