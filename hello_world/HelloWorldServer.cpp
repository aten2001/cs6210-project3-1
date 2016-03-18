#include "gen-cpp/HelloWorld.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <boost/program_options.hpp>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;
namespace po = boost::program_options;

using namespace ::helloworld;


class HelloWorldHandler : virtual public HelloWorldIf {
public:
  HelloWorldHandler() {
    // Your initialization goes here
  }

  void get_time(std::string& _return) {
    std::cout << "get_time called" << std::endl;
    time_t timer;
    time(&timer);
    _return.assign(ctime(&timer));
  }

};

int main(int argc, char** argv) {
  int port;
  po::options_description desc("Options");
  desc.add_options()
          ("help,h", "Produce help message")
          ("port,p", po::value<int>(&port)->default_value(9090), "Server Listening Port")
          ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }

  std::cout << "Starting Server: Listening on Port " << port << std::endl;
  shared_ptr<HelloWorldHandler> handler(new HelloWorldHandler());
  shared_ptr<TProcessor> processor(new HelloWorldProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return EXIT_SUCCESS;
}
