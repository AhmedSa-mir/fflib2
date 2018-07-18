#include <hpx/hpx.hpp>
#include <hpx/hpx_init.hpp>
//
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>
//
extern "C" {
  #include "ff.h"
  #include "components/libfabric/ctx.h"
}
//
#include "plugins/parcelport/libfabric/libfabric_controller.hpp"
#include "plugins/parcelport/libfabric/parcelport_libfabric.hpp"
//
// to run in 3 terminql sessions on the same machine
// ./bin/hpx_fflib_1 --hpx:localities=3 --hpx:node=0 -Ihpx.parcel.bootstrap=tcp --hpx:ini=hpx.parcel.libfabric.enable=1  --hpx:run-hpx-main --operation=allreduce
// ./bin/hpx_fflib_1 --hpx:localities=3 --hpx:node=1 -Ihpx.parcel.bootstrap=tcp --hpx:ini=hpx.parcel.libfabric.enable=1  --hpx:run-hpx-main --operation=allreduce
// ./bin/hpx_fflib_1 --hpx:localities=3 --hpx:node=2 -Ihpx.parcel.bootstrap=tcp --hpx:ini=hpx.parcel.libfabric.enable=1  --hpx:run-hpx-main --operation=allreduce
//
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

  int rank, size;
  int   count = 1000;
  const int N = 100;
  int  failed = 0;

  ffinit(0, nullptr);

  ffrank(&rank);
  ffsize(&size);

  // make sure we use int32 since the reduce test uses FFINT32
  std::vector<int32_t> to_reduce(count);
  std::vector<int32_t> reduced(count);

  ffschedule_h allreduce;
  ffallreduce(to_reduce.data(), reduced.data(), count, 0, FFSUM, FFINT32, &allreduce);

#ifdef FFLIB_HAVE_MPI
  MPI_Barrier(MPI_COMM_WORLD); //not needed, just for having nice output
#endif
  for (int i=0; i<N; i++)
  {
      for (int j=0; j<count; j++){
          to_reduce[j] = i+j;
          reduced[j] = 0;
      }

      ffschedule_post(allreduce);
      ffschedule_wait(allreduce);

      for (int j=0; j<count; j++){
          if (reduced[j] != (i+j)*size){
              printf("FAILED!\n");
              failed=1;
          }
      }

      /* this is ugly... TODO: have internal tags for collectives or allow to change the tag of an operation without changing it */
      /* let's rely on the MPI non-overtaking rule for now */
      //MPI_Barrier(MPI_COMM_WORLD);
  }

  ffschedule_delete(allreduce);

  fffinalize();

  if (!failed){
      printf("PASSED!\n");
  }

  return 0;
}

// ------------------------------------------------------------------------
void test_function(int a) {
    std::cout << a << std::endl;
}
// declare an action type
HPX_DEFINE_PLAIN_ACTION(test_function, test_action);
HPX_REGISTER_ACTION_DECLARATION(test_action);

// register the action type
HPX_REGISTER_ACTION(test_action);

int test_hpx_send_recv()
{
  std::cout << "Testing HPX async function" << std::endl;
  std::vector<hpx::id_type>    remotes = hpx::find_remote_localities();
  std::vector<hpx::id_type> localities = hpx::find_all_localities();
  //
  int x= 0;
  test_action test;
  for (auto l : localities) {
      // execute the function 'test_function' on the remote locality
      // passing the argument x.
      // this will trigger the sending of a message encoding the action and x
      hpx::async(test, l, x);
  }

  // get the parcelhandler
  hpx::parcelset::parcelhandler &ph = hpx::get_runtime().get_parcel_handler();
  std::cout << "Calling background work on local parcelport\n";
  ph.do_background_work();

  std::ostringstream buf;
  ph.list_parcelports(buf);
  std::cout << "parcelports are :\n" << buf.str() << std::endl;

  auto pp = ph.get_default_parcelport();
  if (pp->type()!="libfabric") {
      throw std::runtime_error("This code only work with libfabric parcelport");
  }

  using namespace hpx::parcelset::policies;
  // convert pointer to proper libfabric parcelport type instead of virtual base class
  std::shared_ptr<libfabric::parcelport> lf =
    std::dynamic_pointer_cast<libfabric::parcelport>(pp);
  if (lf==nullptr) {
      throw std::runtime_error("This should not be null");
  }
  std::cout << "Got pointer to Parcelport pointer of type " << lf->type() << std::endl;
  // get a pointer to the libfabric controller
  libfabric::libfabric_controller_ptr lc = lf->libfabric_controller_;
  // you can now access the libfabric controller using
  // lc->do_stuff()...

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
    else if (op == "async") {
        test_hpx_send_recv();
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
