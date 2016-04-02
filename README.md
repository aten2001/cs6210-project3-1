# cs6210-project3
Authors: Pranith Kumar, Dilan Manatunga

This project contains the code for an RPC based caching proxy server.
The RPC mechanism is done through Apache Thrift library, while web
calls are made using CURL.

Requirements:
- Apache Thrift 0.9.3
- CURL

## Project Structure
- hello_world/    
  - Simple RPC client/server
- simple_proxy/ 
  - Simple RPC proxy server
- caching_proxy
  - Caching Proxy Server
- web_cache/
  -Implementation of Web page cache and cache replacement policy
- tests/
  -  Test workloads and test generation scripts

### hello_world
The hello_world directory contains a simple implementation of the RPC client and server.
The server listens at the inputted port for any get_time RPC methods and returns the current time.
The client takes in a hostname and port and performs a get_time RPC method call.

Compile:
Run make in hello_world directory.
```bash
make
```
To run the server:
```bash
./HelloWorldServer -p PORT
```

To run the client:
```bash
./HelloWorldClient --host HOST -p PORT
```

### simple_proxy
The simple_proxy directory contains a simple implementation of an RPC proxy server.
The server listens at the inputted port for any get_url RPC methods and returns the current webpage for the given url.
The client takes in a hostname, port, and url and performs a get_url RPC method call with.

Compile:
Run make in simple_proxy directory.
```bash
make
```
To run the server:
```bash
./SimpleProxyServer -p PORT
```

To run the client:
```bash
./SimpleProxyClient --host HOST -p PORT -u URL
```
### caching_proxy
The caching_proxy directory contains the implementation for caching proxy server, as well
as the test scripts used to test the server and evaluate different cache policies.

Compile:
Run make in caching_proxy directory.
```bash
make
```
To run the server:
```bash
./CachingProxyServer -p PORT -r REPL_POLICY
```
REPL_POLICY can be one of the following (RAND, FIFO, LRU, MAXS)

The client currently set the server cache to 1024KB and fetches the inputted url.
To run the client:
```bash
./CachingProxyClient --host HOST -p PORT -u URL
```

To run a test:
```bash
./CachingProxyTest --host HOST -p PORT -t TEST_FILE -r REPL_POLICY -c CACHE_SIZE -w WARMUP_PERIOD
```
REPL_POLICY can be one of the following (RAND, FIFO, LRU, MAXS). 
The test workloads can be found in the tests directory.

### web_cache
The web_cache directory contains the implementation of the web page cache. 
The WebPageCache.h file specifies the interface for the web page cache, as 
well as the CacheReplPolicy interface.

Each policy is implement in the files <POLICY>ReplPolicy.h and <POLICY>ReplPolicy.cpp.
There are currently implementations of a Random, First-In-First-Out, Least Recently Used, 
and Max Size replacement policies.
  
### tests
The test directory contains the workloads used to evaluate replacement policies. The
script (gen_test.y) and data (similarweb_list.csv) to generate the policies are also included.

To generate a test workload, run the following command:
```bash
./gen_test --webpage_list_file FILE --ouput_file FILE TYPE TEST_LENGTH
```
TYPE can one of the following: random, freq, size, revsize
