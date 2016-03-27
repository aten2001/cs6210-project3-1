#include "gen-cpp/CachingProxy.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <boost/program_options.hpp>
#include <curl/curl.h>
#include "../web_page_cache/WebPageCache.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;
namespace po = boost::program_options;

using namespace ::cachingproxy;

#define KB(x) ((x) << 10)
#define MB(x) ((x) << 20)

size_t WriteStringCallback(char* buf, size_t size, size_t nmemb, void* up) {
  std::string* str = (std::string*)up;
  str->reserve(str->size() + size * nmemb);
  str->append(buf, size * nmemb);
  return size * nmemb;
}

class CachingProxyHandler : virtual public CachingProxyIf {
public:
  CachingProxyHandler(int32_t max_size, const std::string& repl_policy, int32_t warmup = 0) {
    page_cache_ = new WebPageCache(max_size, repl_policy, warmup);
  }

  void get_url(std::string& _return, const std::string& url) {
    // Your implementation goes here
    std::cout << "get_url: " << url << std::endl;
    _return = page_cache_->GetWebPage(url);
  }

 private:
  WebPageCache* page_cache_;
};

int main(int argc, char** argv) {
  int port;
  int cache_size;
  int warmup;
  std::string repl_policy;
  po::options_description desc("Options");
  desc.add_options()
          ("help,h", "Produce help message")
          ("port,p", po::value<int>(&port)->default_value(9090), "Server Listening Port")
          ("repl_policy,r", po::value<std::string>(&repl_policy)->default_value("FIFO"), "Cache Replacement Policy")
          ("cache_size,c", po::value<int>(&cache_size)->default_value(KB(1024)), "Cache Size (KB)")
          ("warmnup,w", po::value<int>(&warmup)->default_value(0), "Warmup Period")
          ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }

  std::cout << "Starting Server: Listening on Port " << port << std::endl;
  shared_ptr<CachingProxyHandler> handler(new CachingProxyHandler(cache_size, repl_policy, warmup));
  shared_ptr<TProcessor> processor(new CachingProxyProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return EXIT_SUCCESS;
}
