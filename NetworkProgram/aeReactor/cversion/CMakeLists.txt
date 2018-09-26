cmake_minimum_required(VERSION 3.9)

add_definitions("-Wall -g")

add_executable(echo testEcho.cc ae.c zmalloc.c anet.c)