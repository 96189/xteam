cmake_minimum_required(VERSION 2.8.12.2)
add_definitions("-Wall -g -std=c++0x")
set(CMAKE_CXX_FLAGS
 -pthread
 )
add_executable(Server main.cc net.cc server.cc clientdata.cc)
add_executable(Clinet client.cc net.cc)