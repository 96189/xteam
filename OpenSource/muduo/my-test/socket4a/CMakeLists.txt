cmake_minimum_required(VERSION 2.6)

project(socket4a CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "release")
endif()

if(NOT MUDUO_PATH)
  set(MUDUO_PATH "/home/HaoLi/build/${CMAKE_BUILD_TYPE}-install")
endif()

set(CXX_FLAGS
 -g
 # -DVALGRIND
 # -DMUDUO_STD_STRING
 -Wall
 -Wextra
 # -m32
 #-Werror
 -Wconversion
 -Wno-unused-parameter
 -Wold-style-cast
 -Woverloaded-virtual
 -Wpointer-arith
 -Wshadow
 -Wwrite-strings
 -march=native
 # -MMD
 -std=c++0x
 -rdynamic
 )
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

find_path(Muduo_INCLUDE_DIR muduo "${MUDUO_PATH}/include")
find_path(Muduo_LIBRARY_DIR libmuduo_net.a "${MUDUO_PATH}/lib")
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${Muduo_LIBRARY_DIR})
message(STATUS ${Muduo_INCLUDE_DIR})
message(STATUS ${Muduo_LIBRARY_DIR})

include_directories(${Muduo_INCLUDE_DIR})
find_library(muduo_base muduo_base)
find_library(muduo_net muduo_net)
find_library(muduo_inspect muduo_inspect)
find_library(muduo_http muduo_http)
find_library(muduo_protorpc muduo_protorpc)
message(STATUS ${muduo_base})
message(STATUS ${muduo_net})
message(STATUS ${muduo_inspect})
message(STATUS ${muduo_http})
message(STATUS ${muduo_protorpc})

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR})

#add_executable(socket4a socket4a.cc)
#target_link_libraries(socket4a ${muduo_net})
#target_link_libraries(socket4a ${muduo_base})
#target_link_libraries(socket4a pthread rt)

add_executable(tcprelay tcprelay.cc)
target_link_libraries(tcprelay ${muduo_net})
target_link_libraries(tcprelay ${muduo_base})
target_link_libraries(tcprelay pthread rt)
