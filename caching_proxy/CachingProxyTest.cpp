#include <iostream>
#include <fstream>
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

int main(int argc, char** argv) {
  std::string host;
  int port;
  std::string url, test_file;
  po::options_description desc("Options");
  desc.add_options()
          ("help,h", "Produce help message")
          ("host", po::value<std::string>(&host)->default_value("localhost"), "Hostname")
          ("port,p", po::value<int>(&port)->default_value(9090), "Port")
          ("test_input,t", po::value<std::string>(&test_file)->default_value("../tests/top500.csv"), "Test File")
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
  while (!infile.eof()) {
	  getline(infile, url);
  try {
    transport->open();
    std::string response;
    std::cout << "Calling RPC method get_url" << std::endl;
    test.get_url(response, url);
    std::cout << response << std::endl;
    std::cout << "Finished!" << std::endl;
  } catch (TException& tx) {
    std::cout << "ERROR: " << tx.what() << std::endl;
  }
  }
}
