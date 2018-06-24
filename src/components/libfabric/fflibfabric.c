
#include "ff.h"
#include "ffinternal.h"
#include "fflibfabric.h"
#include "libfabric_impl.h"

int fflibfabric_init(int * argc, char *** argv){
    int op, ret = EXIT_SUCCESS;

    init_pingpong();

    ct.hints = fi_allocinfo();
    if (!ct.hints)
        return EXIT_FAILURE;
    ct.hints->ep_attr->type = FI_EP_MSG;
    ct.hints->caps = FI_MSG;
    ct.hints->mode = FI_CONTEXT | FI_CONTEXT2 | FI_MSG_PREFIX;
    ct.hints->domain_attr->mr_mode = FI_MR_BASIC;

    ofi_osd_init();

    while ((op = getopt(argc, argv, "hvd:p:e:I:S:B:P:cm:")) != -1) {
        switch (op) {
        default:
            pp_parse_opts(&ct, op, optarg);
            break;
        case '?':
        case 'h':
            pp_pingpong_usage(&ct, argv[0],
                      "Ping pong client and server");
            return EXIT_FAILURE;
        }
    }

    if (optind < argc)
        ct.opts.dst_addr = argv[optind];

    //ret = run_pingpong_msg(&ct);

    PP_DEBUG("Selected endpoint: MSG\n");

    ret = pp_ctrl_init(ct);
    if (ret)
        return EXIT_FAILURE;

    if (!ct->opts.dst_addr) {
        ret = pp_start_server(ct);
        if (ret)
            return EXIT_FAILURE;
    }

    if (ct->opts.dst_addr) {
        ret = pp_client_connect(ct);
        PP_DEBUG("CLIENT: client_connect=%s\n", ret ? "KO" : "OK");
    } else {
        ret = pp_server_connect(ct);
        PP_DEBUG("SERVER: server_connect=%s\n", ret ? "KO" : "OK");
    }

    if (ret)
        return EXIT_FAILURE;
 
    //initialize the generic computation (gcomp) component 
    ffgcomp_init();
   
    return FFSUCCESS; 
}

int fflibfabric_finalize(){
    
    ret = pp_finalize(ct);
    if(ret)
        return FFERROR;

    fi_shutdown(ct->ep, 0);
    pp_free_res(&ct);

    ffgcomp_finalize();

    return FFSUCCESS;
}

int fflibfabric_get_rank(int * rank){
    //MPI_Comm_rank(MPI_COMM_WORLD, rank);
    return FFSUCCESS;
}

int fflibfabric_get_size(int * size){
    //MPI_Comm_size(MPI_COMM_WORLD, size);
    return FFSUCCESS;
}