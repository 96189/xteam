cmake_minimum_required(VERSION 3.9)

add_definitions("-Wall -g")

add_executable(test aenet.cc aeEpoll.cc aeEventLoop.cc main.cc)