#ifndef _CTX_H_
#define _CTX_H_

#include "ffinternal.h"
#include "ffop.h"

#ifdef __cplusplus
extern "C" {
#endif

int  libfabric_init(int , char **);
int  check_tx_completions(ffop_t **);
int  check_rx_completions(ffop_t **);
int  post_send(ffop_t * op);
int  post_recv(ffop_t * op);
void * get_send_buffer();
void * get_recv_buffer();
int get_locality_id();
int get_remote_locality_id();
void mr_release();

#ifdef __cplusplus
}
#endif

#endif /* _CTX_H_ */
