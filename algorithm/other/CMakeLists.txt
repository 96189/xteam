cmake_minimum_required(VERSION 3.5)
add_definitions("-Wall -g -std=c++0x")
set(CMAKE_CXX_FLAGS
 -pthread
 )
add_executable(cardtest choicecard.cc cardUnittest.cc)
target_link_libraries(cardtest gtest)
