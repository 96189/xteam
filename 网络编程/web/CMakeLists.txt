cmake_minimum_required(VERSION 3.5)
project(webserver)

set(CXX_FLAGS
    -pg 
    -g 
    -Wall 
    -fprofile-arcs 
    -ftest-coverage 
)


file(GLOB SRC_LIST ./src/*.c* ./third/cJSON/*.c*)
file(GLOB TEST_SRC_LIST ./unittest/*)

include_directories(./third/cJSON ${ZLOG_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_executable(${PROJECT_NAME} ${SRC_LIST})


# depend
find_path(ZLOG_INCLUDE_DIR zlog.h)
target_link_libraries(${PROJECT_NAME} pthread m ssl crypto zlog gcov)

# testing
#find_package(GTest)
#if(GTEST_FOUND)
#    add_executable(unit_tests ${SRC_LIST} ${TEST_SRC_LIST})
#    target_compile_definitions(unit_tests PUBLIC UNIT_TESTS)
#    set_target_properties(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS on)
#    target_link_libraries(unit_tests PUBLIC ${GTEST_BOTH_LIBRARIES} ${PROJECT_NAME})
#endif()

# pack 
file(GLOB publish_config_files src/conf/*.*)
set(CMAKE_INSTALL_PREFIX ../output)
install(TARGETS ${PROJECT_NAME} 
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib/static)

install(FILES ${publish_config_files} DESTINATION conf)
install(DIRECTORY src/htdocs/ DESTINATION htdocs)
install(DIRECTORY DESTINATION log)
