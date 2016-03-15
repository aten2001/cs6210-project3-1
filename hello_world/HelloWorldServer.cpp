#include "gen-cpp/HelloWorld.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::helloworld;

class HelloWorldHandler : virtual public HelloWorldIf {
public:
  HelloWorldHandler() {
    // Your initialization goes here
  }

  void get_time(std::string& _return) {
    std::cout << "get_time called" << std::endl;
    time_t timer;
    time(&timer);
    _return = ctime(&timer);
  }

};

int main(int argc, char **argv) {
  int port = 9090;
  shared_ptr<HelloWorldHandler> handler(new HelloWorldHandler());
  shared_ptr<TProcessor> processor(new HelloWorldProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}
