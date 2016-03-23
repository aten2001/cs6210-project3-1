cmake_minimum_required(VERSION 3.3)
project(project3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    hello_world/HelloWorldClient.cpp
    hello_world/HelloWorldServer.cpp
    simple_proxy/SimpleProxyClient.cpp
    simple_proxy/SimpleProxyServer.cpp)

add_executable(project3 ${SOURCE_FILES})