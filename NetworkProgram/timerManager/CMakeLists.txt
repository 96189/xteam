add_definitions("-Wall -g -std=c++0x")
set(CMAKE_CXX_FLAGS
 -pthread
 )
add_executable(main main.cc timerManager.cc timerobj.cc timerContainerManager.cc timerContainer.cc log.cc)
