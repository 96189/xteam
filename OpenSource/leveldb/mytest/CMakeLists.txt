cmake_minimum_required(VERSION 3.9)

# This project can use C11, but will gracefully decay down to C89.
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED OFF)
set(CMAKE_C_EXTENSIONS OFF)

# This project requires C++11.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_definitions("-Wall -g")

include_directories("/home/lihao/xteam/OpenSource/leveldb/include")
link_directories("/home/lihao/build_leveldb")

find_package(Threads)

add_executable(leveldb_test leveldb_test.cc)
target_link_libraries(leveldb_test leveldb ${CMAKE_THREAD_LIBS_INIT})