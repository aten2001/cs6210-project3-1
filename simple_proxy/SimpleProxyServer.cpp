#include "gen-cpp/SimpleProxy.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <boost/program_options.hpp>
#include <curl/curl.h>

#include "SimpleCache.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;
namespace po = boost::program_options;

using namespace ::simpleproxy;

size_t WriteStringCallback(char* buf, size_t size, size_t nmemb, void* up) {
  std::string* str = (std::string*)up;
  str->reserve(str->size() + size * nmemb);
  str->append(buf, size * nmemb);
  return size * nmemb;
}

class SimpleProxyHandler : virtual public SimpleProxyIf {
public:
  SimpleProxyHandler() {
    // Your initialization goes here
  }

  void get_url(std::string& _return, const std::string& url) {
    if (get_url_cached(_return, url))
      return;
    else {
      get_url_curl(_return, url);
      cache_url(_return, url);
    }
  }

private:
  void cache_url(const std::string& _return, const std::string& url) {
    m_web_cache.cache_url(_return, url);
  }
  
  bool get_url_cached(std::string& _return, const std::string& url) {
    if (m_web_cache.is_url_cached(url)) {
      std::cout << "get_url_cached: " << url << std::endl;
      _return = m_web_cache.get_url_cached(url);
      return true;
    }

    return false;
  }
  
  void get_url_curl(std::string& _return, const std::string& url) {
    // Your implementation goes here
    std::cout << "get_url: " << url << std::endl;
    CURL *curl_handle;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);

    /* init the curl session */
    curl_handle = curl_easy_init();

    /* specify URL to get */
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());

    /* send all data to this function  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteStringCallback);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)&_return);

    /* some servers don't like requests that are made without a user-agent
       field, so we provide one */
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

#if DEBUG
    /* output progress from curl */
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L); //tell curl to output its progress
#endif

    /* get it! */
    res = curl_easy_perform(curl_handle);

    /* check for errors */
    if(res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    }

    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);

    /* we're done with libcurl, so clean it up */
    curl_global_cleanup();
  }

private:
  SimpleCache m_web_cache;
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
  shared_ptr<SimpleProxyHandler> handler(new SimpleProxyHandler());
  shared_ptr<TProcessor> processor(new SimpleProxyProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return EXIT_SUCCESS;
}
