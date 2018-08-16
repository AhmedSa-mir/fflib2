#include <stdio.h>
#include <stdlib.h>

#include "ff.h"
#include "src/components/libfabric/ctx.h"

#define FFCALL(X) { int ret; if (ret=(X)!=FFSUCCESS) { printf("Error: %i\n", ret); exit(-1); } }

#define N 100

int main(int argc, char * argv[]){
    
    int count = 10, rank, size;
    int32_t *to_reduce, *reduced;

    if (argc!=4)
    {
        printf("Usage: %s <port> <remote_host> <remote_port>\n", argv[0]);
        return 1;
    }

    ffinit(&argc, &argv);

    ffrank(&rank);
    ffsize(&size);

    printf("rank: %i; size: %i\n", rank, size);

    to_reduce = get_send_buffer();
    reduced = get_recv_buffer();
    
    int failed = 0;
    
    ffschedule_h allreduce;
    ffallreduce(to_reduce, reduced, count, 0, FFSUM, FFINT32, 0, &allreduce);
        
    for (int j=0; j<count; j++){
        to_reduce[j] = j;
        reduced[j] = 0;
    }
        
    ffschedule_post(allreduce);
    ffschedule_wait(allreduce);

    for (int j=0; j<count; j++){
        if (reduced[j] != j*size){
            printf("[rank %i] FAILED! (j: %i) (expected: %u; got: %u; toreduce: %u; csize: %u)\n", rank, j, j*size, reduced[j], to_reduce[j], size);
            failed=1;
        }
    }

    ffschedule_delete(allreduce);

    fffinalize();

    if (!failed){
        printf("PASSED!\n");
    }
    
    return 0;
}
