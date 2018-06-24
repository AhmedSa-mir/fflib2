#include "ff.h"
#include "ffprogress.h"
#include "ffinternal.h"
#include "ffbind.h"
#include "components/libfabric/fflibfabric.h"
#include "components/libfabric/ffop_libfabric_progresser.h"
#include "components/gcomp/ffgcomp.h"
#include "components/gcomp/ffop_gcomp.h"


int ffbind(ffdescr_t * ffdescr){

    ffdescr->impl.init = fflibfabric_init;
    ffdescr->impl.finalize = fflibfabric_finalize;

    ffdescr->impl.ops[FFSEND].init = ffop_libfabric_init;
    ffdescr->impl.ops[FFSEND].post = ffop_libfabric_send_post;

    ffdescr->impl.ops[FFRECV].init = ffop_libfabric_init;
    ffdescr->impl.ops[FFRECV].post = ffop_libfabric_recv_post;

    ffdescr->impl.ops[FFCOMP].init = ffop_gcomp_init;
    ffdescr->impl.ops[FFCOMP].post = ffop_gcomp_post;

    ffdescr->impl.operator_create = ffop_gcomp_operator_custom_create;
    ffdescr->impl.operator_delete = ffop_gcomp_operator_custom_delete;

    ffprogresser_t libfabric_progresser;
    libfabric_progresser.init = ffop_libfabric_progresser_init;
    libfabric_progresser.finalize = ffop_libfabric_progresser_finalize;
    libfabric_progresser.progress = ffop_libfabric_progresser_progress;
    ffprogresser_register(libfabric_progresser);

    return FFSUCCESS;
}






