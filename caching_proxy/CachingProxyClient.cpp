#include <iostream>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <boost/program_options.hpp>

#include "gen-cpp/CachingProxy.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
namespace po = boost::program_options;

using namespace cachingproxy;

#define KB(x) ((x) << 10)
#define MB(x) ((x) << 20)

int main(int argc, char** argv) {
  std::string host;
  int port;
  std::string url;
  po::options_description desc("Options");
  desc.add_options()
          ("help,h", "Produce help message")
          ("host", po::value<std::string>(&host)->default_value("localhost"), "Hostname")
          ("port,p", po::value<int>(&port)->default_value(9090), "Port")
          ("url,u", po::value<std::string>(&url)->default_value("www.google.com"), "Request URL")
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

  CachingProxyClient client(protocol);

  std::cout << "Connecting to server " << host << ":" << port << std::endl;

  try {
    transport->open();

    // Initialize Cache
    client.reset_cache();
    client.set_cache_size(KB(1024));
    client.set_warmup_period(0);

    std::string response;
    std::cout << "Calling RPC method get_url" << std::endl;
    client.get_url(response, url);
    std::cout << response << std::endl;
    std::cout << "Finished!" << std::endl;
  } catch (TException& tx) {
    std::cout << "ERROR: " << tx.what() << std::endl;
  }
}
