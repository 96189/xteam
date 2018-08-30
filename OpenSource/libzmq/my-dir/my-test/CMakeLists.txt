
cmake_minimum_required(VERSION 3.9)

add_definitions("-Wall -g")

link_directories("/usr/local/lib")

add_executable(hwserver hwserver.cc)
add_executable(hwclient hwclient.cc)
add_executable(wuserver wuserver.cc)
add_executable(wuclient wuclient.cc)
add_executable(taskvent taskvent.cc)
add_executable(taskwork taskwork.cc)
add_executable(tasksink tasksink.cc)
add_executable(mspoller mspoller.cc)

target_link_libraries(hwserver zmq)
target_link_libraries(hwclient zmq)
target_link_libraries(wuserver zmq)
target_link_libraries(wuclient zmq)
target_link_libraries(taskvent zmq)
target_link_libraries(taskwork zmq)
target_link_libraries(tasksink zmq)
target_link_libraries(mspoller zmq)
