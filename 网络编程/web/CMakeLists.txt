
cmake_minimum_required(VERSION 3.5)
project(web)

set(CXX_FLAGS
    -pg 
    -g 
    -Wall 
    -fprofile-arcs 
    -ftest-coverage 
)

find_path(ZLOG_INCLUDE_DIR zlog.h)

file(GLOB SRC_LIST ./src/*.c ./third/cJSON/*.c)

include_directories(./third/cJSON ${ZLOG_INCLUDE_DIR})
add_executable(webserver ${SRC_LIST})
target_link_libraries(webserver pthread m ssl crypto zlog gcov)

set(CMAKE_INSTALL_PREFIX ../output)
install(TARGETS webserver 
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib/static)

file(GLOB publish_config_files src/conf/*.*)
install(FILES ${publish_config_files} DESTINATION conf)
install(DIRECTORY src/htdocs/ DESTINATION htdocs)
install(DIRECTORY DESTINATION log)
