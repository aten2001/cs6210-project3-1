cmake_minimum_required(VERSION 2.8)
project(project3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        hello_world/HelloWorldClient.cpp
        hello_world/HelloWorldServer.cpp

        simple_proxy/SimpleProxyClient.cpp
        simple_proxy/SimpleProxyServer.cpp

        caching_proxy/CachingProxyClient.cpp
        caching_proxy/CachingProxyServer.cpp
        caching_proxy/CachingProxyTest.cpp

        web_page_cache/WebPageCache.cpp
        web_page_cache/WebPageCache.h

        web_page_cache/RandReplPolicy.cpp
        web_page_cache/RandReplPolicy.h

        web_page_cache/FifoReplPolicy.cpp
        web_page_cache/FifoReplPolicy.h

        web_page_cache/MaxSizeReplPolicy.cpp
        web_page_cache/MaxSizeReplPolicy.h)

add_executable(project3 ${SOURCE_FILES})
