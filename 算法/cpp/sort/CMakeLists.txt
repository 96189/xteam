add_definitions("-Wall -g")
set(CMAKE_CXX_FLAGS
 -pthread
 )
add_executable(sorttest sortunittest.cc)
target_link_libraries(sorttest gtest)
