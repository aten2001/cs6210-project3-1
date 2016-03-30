#include <iostream>
#include <fstream>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <boost/program_options.hpp>

#include "gen-cpp/CachingProxy.h"
#include "timer.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
namespace po = boost::program_options;

using namespace cachingproxy;

#define DEBUG 0
#define KB(x) ((x) << 10)
#define MB(x) ((x) << 20)

int main(int argc, char** argv) {
  std::string host;
  int port;

  int cache_size, warmup_period;

  std::string url, test_file;
  po::options_description desc("Options");
  desc.add_options()
          ("help,h", "Produce help message")
          ("host", po::value<std::string>(&host)->default_value("localhost"), "Hostname")
          ("port,p", po::value<int>(&port)->default_value(9090), "Port")
          ("test_input,t", po::value<std::string>(&test_file)->default_value("../tests/top500.csv"), "Test File")
          ("cache_size,c", po::value<int>(&cache_size)->default_value(1024), "Cache Size (KB)")
          ("warmnup,w", po::value<int>(&warmup_period)->default_value(0), "Warmup Period")
          ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }

  boost::shared_ptr<TTransport> socket(new TSocket(host, port));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

  CachingProxyClient test(protocol);

  std::cout << "Connecting to server " << host << ":" << port << std::endl;
  std::ifstream infile(test_file.c_str());
  if (infile.fail()) {
    std::cerr << "Error: Could not open test file " << test_file << std::endl;
    exit(1);
  }


  try {
    transport->open();

    std::cout << "Test Start: " << std::endl;
    std::cout << "Cache Size=" << cache_size << "KB ";
    std::cout << "Warmup=" << warmup_period << std::endl;

    // Initialize Cache
    test.reset_cache();
    test.set_cache_size(KB(cache_size));
    test.set_warmup_period(warmup_period);

    std::string response;

    for (int i = 0; i < warmup_period; i++) {
      getline(infile, url);
    #if DEBUG
      std::cout << "get_url: " << url << std::endl;
    #endif
      test.get_url(response, url);
    }

    struct timespec before, after;

    int num_calls = 0;
    start_watch(&before);
    while (!infile.eof()) {
      getline(infile, url);
    #if DEBUG
      std::cout << "get_url: " << url << " " << url.size() << std::endl;
    #endif
      if (url.size()) {
        test.get_url(response, url);
        num_calls++;
      }
    }
    stop_watch(&after);

    double hit_rate = test.get_hit_rate();
    transport->close();
    std::cout << "Test Finished!" << std::endl;
    std::cout << "Num Calls: " << num_calls << std::endl;
    std::cout << "Hit Rate: " << hit_rate << std::endl;
    std::cout << "Completion Time: " << get_timer_diff(&before, &after) << std::endl;
  } catch (TException& tx) {
    std::cout << "ERROR: " << tx.what() << std::endl;
  }
}
