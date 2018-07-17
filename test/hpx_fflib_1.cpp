#include <hpx/hpx.hpp>
#include <hpx/hpx_init.hpp>
//
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

#include "components/libfabric/ctx.h"

// to run in 3 terminql sessions on the same machine
// ./bin/hpx_fflib_1 --hpx:localities=3 --hpx:node=0 -Ihpx.parcel.bootstrap=tcp --hpx:ini=hpx.parcel.libfabric.enable=1  --hpx:run-hpx-main --operation=allreduce
// ./bin/hpx_fflib_1 --hpx:localities=3 --hpx:node=1 -Ihpx.parcel.bootstrap=tcp --hpx:ini=hpx.parcel.libfabric.enable=1  --hpx:run-hpx-main --operation=allreduce
// ./bin/hpx_fflib_1 --hpx:localities=3 --hpx:node=2 -Ihpx.parcel.bootstrap=tcp --hpx:ini=hpx.parcel.libfabric.enable=1  --hpx:run-hpx-main --operation=allreduce

// alternatively, use the hpxrun python script to launch N copies of the binary
// use "--" to separate the arguments for the binary from the arguments to hpxrun
// /home/biddisco/build/hpx-debug/bin/hpxrun.py -l 8 bin/hpx_fflib_1 -- --hpx:ini=hpx.parcel.bootstrap=tcp --hpx:ini=hpx.parcel.libfabric.enable=1 --hpx:run-hpx-main

int  libfabric_init(int , char **)   { return 0; }
int  check_tx_completions(ffop_t **) { return 0; }
int  check_rx_completions(ffop_t **) { return 0; }
int  post_send(ffop_t * op)          { return 0; }
int  post_recv(ffop_t * op)          { return 0; }
void * get_send_buffer()             { return nullptr; }
void * get_recv_buffer()             { return nullptr; }
int get_remote_locality_id()         { return 0; }
void mr_release()                    {}
//
int get_locality_id()                {
    hpx::id_type here = hpx::find_here();
    uint64_t     rank = hpx::naming::get_locality_id_from_id(here);
    return rank;
}


// ------------------------------------------------------------------------
int test_fflib_send_recv()
{
  std::cout << "Testing Send/Recv " << std::endl;
  // insert fflib testing code here
  return 0;
}

// ------------------------------------------------------------------------
int test_fflib_allreduce()
{
  std::cout << "Testing AllReduce " << std::endl;
  // insert fflib testing code here
  return 0;
}

// ------------------------------------------------------------------------
// This runs on an HPX thread
// when hpx::finalize is called, the HPX runtime terminates
// Normally this is only executed on rank 0, but adding
// --hpx:run-hpx-main makes all ranks execute it.
// ------------------------------------------------------------------------
int hpx_main(boost::program_options::variables_map &vm)
{
    hpx::id_type                    here = hpx::find_here();
    uint64_t                        rank = hpx::naming::get_locality_id_from_id(here);
    std::string                     name = hpx::get_locality_name();
    uint64_t                      nranks = hpx::get_num_localities().get();
    //
    std::string op = "";
    //
    if (vm.count("operation"))
        op = vm["operation"].as<std::string>();

    std::cout << "Testing Operation : " << op
              << " on Rank " << rank << " (" << name << ") of " << nranks
              << std::endl;

    if (op == "sendrecv") {
        test_fflib_send_recv();
    }
    else if (op == "allreduce") {
        test_fflib_allreduce();
    }
    else {
        std::cout << "usage --operation=sendrecv/allreduce : " << std::endl;
    }

  return hpx::finalize();
}

// ------------------------------------------------------------------------
// This runs on a normal Kernel thread
// ------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  // add command line option which controls the random number generator seed
  using namespace boost::program_options;
  options_description desc_commandline("Usage: " HPX_APPLICATION_STRING
                                       " [options]");

  desc_commandline.add_options()(
      "operation,o", value<std::string>(),
      "the random number generator seed to use for this run");

  // By default this test should run on all available cores
  std::vector<std::string> const cfg = {"hpx.os_threads=all"};

  // Initialize and run HPX, HPX will stay active until
  // hpx::finalize returns
  int init = hpx::init(desc_commandline, argc, argv, cfg);
  if (init != 0) {
    std::cout << "HPX main exited with non-zero status" << std::endl;
  }
  return init;
}
