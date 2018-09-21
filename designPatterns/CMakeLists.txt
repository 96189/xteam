cmake_minimum_required(VERSION 3.9)

set (CMAKE_CXX_STANDARD 11)
add_definitions("-Wall -g")
find_package(Threads)

add_executable(duckInstance duckInstance.cc)

add_executable(weatherStation weatherStation.cc)
