
cmake_minimum_required(VERSION 3.9)

project(algorithm CXX)

set(CXX_FLAGS 
-g 
-Wall 
-std=c++0x 
)

if(CMAKE_BUILD_BITS EQUAL 32)
  list(APPEND CXX_FLAGS "-m32")
endif()
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")

set(ROOT_PATH ".")
set(INC_PATH "${ROOT_PATH}/inc")
set(LIB_PATH "${ROOT_PATH}/lib")
set(ARRAY "${ROOT_PATH}/array")
set(STRING "${ROOT_PATH}/string")
message(STATUS ${ROOT_PATH})
message(STATUS ${INC_PATH})
message(STATUS ${LIB_PATH})
message(STATUS ${ARRAY})
message(STATUS ${STRING})

include_directories(${INC_PATH})

set(arrayTest
${LIB_PATH}/pub.cc
${ARRAY}/test.cc 
${ARRAY}/array.cc
)

set(stringTest
${LIB_PATH}/pub.cc
${STRING}/test.cc
${STRING}/string.cc 
)

add_executable(arrayTest ${arrayTest})
add_executable(stringTest ${stringTest})