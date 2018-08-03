#include <stdio.h>
#include <stdlib.h>

#include "ff.h"
#include "src/components/libfabric/ctx.h"

#define FFCALL(X) { int ret; if (ret=(X)!=FFSUCCESS) { printf("Error: %i\n", ret); exit(-1); } }

int main(int argc, char * argv[]){

    int count = 10, rank, size, ret;
    char * buffer_ping, * buffer_pong;

    if (argc != 4)
    {
        printf("Usage: %s <port> <remote_host> <remote_port>\n", argv[0]);
        return 1;
    }

    ffinit(&argc, &argv);

    ffrank(&rank);
    ffsize(&size);

    printf("rank: %i; size: %i\n", rank, size);

    ffop_h pingop, pongop;

    ffschedule_h sched;
    FFCALL(ffschedule_create(&sched));

    if (rank == 0){ /* sender */

        buffer_ping = get_send_buffer();
        buffer_pong = get_recv_buffer();

        for (int i = 0; i < count; i++) buffer_ping[i] = ('A' + i);
        for (int i = 0; i < count; i++) buffer_pong[i] = 0;

        FFCALL(ffsend(buffer_ping, count, FFCHAR, 1, 0, 0, &pingop));
        FFCALL(ffrecv(buffer_pong, count, FFCHAR, 1, 0, 0, &pongop));
        ffop_hb(pingop, pongop);
        
        FFCALL(ffschedule_add_op(sched, pingop));
        FFCALL(ffschedule_add_op(sched, pongop));

    }else{ /* receiver */

        buffer_ping = get_recv_buffer();
        buffer_pong = get_send_buffer();
        
        for (int i = 0; i < count; i++) buffer_ping[i] = 0;
        for (int i = 0; i < count; i++) buffer_pong[i] = ('A' + i);

        FFCALL(ffrecv(buffer_ping, count, FFCHAR, 0, 0, 0, &pingop));
        FFCALL(ffsend(buffer_pong, count, FFCHAR, 0, 0, 0, &pongop));
        ffop_hb(pingop, pongop);

        FFCALL(ffschedule_add_op(sched, pingop));
        FFCALL(ffschedule_add_op(sched, pongop));
    }

    FFCALL(ffschedule_post(sched));

    /* wait the pong to be received/sent */
    FFCALL(ffschedule_wait(sched));

    int fail = 0;
    for (int i = 0; i < count && rank == 0; i++){
        if (buffer_pong[i] != ('A' + i)){
            printf("Fail!\n");
            fail = 1;
        }
    }

    for (int i = 0; i < count && rank == 1; i++){
        if (buffer_ping[i] != ('A' + i)){
            printf("Fail!\n");
            fail = 1;
        }
    }

    if (!fail) printf("Success!\n");

    ffop_free(pingop);
    ffop_free(pongop);

    ffschedule_delete(sched);

    fffinalize();

    return 0;
}
