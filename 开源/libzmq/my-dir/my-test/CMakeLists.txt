
cmake_minimum_required(VERSION 3.9)

set (CMAKE_CXX_STANDARD 11)
add_definitions("-Wall -g")

find_package(Threads)

link_directories("/usr/local/lib")

add_executable(hwserver hwserver.cc)
add_executable(hwclient hwclient.cc)

add_executable(wuserver wuserver.cc)
add_executable(wuclient wuclient.cc)

add_executable(taskvent taskvent.cc)
add_executable(taskwork taskwork.cc)
add_executable(tasksink tasksink.cc)

add_executable(mspoller mspoller.cc)

add_executable(wuproxy wuproxy.cc)

add_executable(rrclient rrclient.cc)
add_executable(rrserver rrserver.cc)
add_executable(rrbroker rrbroker.cc)

add_executable(msgqueue msgqueue.cc)

add_executable(mtserver mtserver.cc)

add_executable(syncpub syncpub.cc)
add_executable(syncsub syncsub.cc)

add_executable(psenvpub psenvpub.cc)
add_executable(psenvsub psenvsub.cc)

add_executable(durasub durasub.cc)
add_executable(durapub durapub.cc)

add_executable(identity identity.cc)

add_executable(rtreq rtreq.cc)
add_executable(rtdealer rtdealer.cc)
add_executable(rtrep rtrep.cc)

add_executable(lbbroker lbbroker.cc)

add_executable(asyncsrv asyncsrv.cc)

add_executable(brokerstateflow brokerstateflow.cc)
add_executable(localcloudflow localcloudflow.cc)
add_executable(fullclustersimulation fullclustersimulation.cc)

add_executable(lpclient lpclient.cc)
add_executable(lpserver lpserver.cc)

add_executable(spqueue spqueue.cc)
add_executable(spworker spworker.cc)

add_executable(ppqueue ppqueue.cc)
add_executable(ppworker ppworker.cc)

add_executable(mdclient mdclient.cc)
add_executable(mdworker mdworker.cc)
add_executable(mdbroker mdbroker.cc mdbrokerservice.cc mdbrokerworker.cc)
add_executable(mdservicediscovery mdservicediscovery.cc)
add_executable(trippingtest trippingtest.cc)

add_executable(mdclient_async mdclient_async.cc)

add_executable(titanic titanic.cc)
add_executable(ticlient ticlient.cc)

add_executable(bstarsrv bstarsrv.cc)
add_executable(bstarcli bstarcli.cc)

add_executable(bstarreactor bstarreactor.cc)

add_executable(freelancesvr freelancesvr.cc)






target_link_libraries(hwserver zmq)
target_link_libraries(hwclient zmq)

target_link_libraries(wuserver zmq)
target_link_libraries(wuclient zmq)

target_link_libraries(taskvent zmq)
target_link_libraries(taskwork zmq)
target_link_libraries(tasksink zmq)

target_link_libraries(mspoller zmq)

target_link_libraries(wuproxy zmq)

target_link_libraries(rrclient zmq)
target_link_libraries(rrserver zmq)
target_link_libraries(rrbroker zmq)

target_link_libraries(msgqueue zmq)

target_link_libraries(mtserver zmq)
target_link_libraries(mtserver ${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(syncpub zmq)
target_link_libraries(syncsub zmq)

target_link_libraries(psenvpub zmq)
target_link_libraries(psenvsub zmq)

target_link_libraries(durasub zmq)
target_link_libraries(durapub zmq)

target_link_libraries(identity zmq)

target_link_libraries(rtreq zmq)
target_link_libraries(rtdealer zmq)
target_link_libraries(rtrep zmq)

target_link_libraries(lbbroker zmq)

target_link_libraries(asyncsrv zmq)

target_link_libraries(brokerstateflow zmq)
target_link_libraries(localcloudflow zmq czmq)
target_link_libraries(fullclustersimulation zmq czmq)

target_link_libraries(lpclient zmq czmq)
target_link_libraries(lpserver zmq czmq)

target_link_libraries(spqueue zmq czmq)
target_link_libraries(spworker zmq czmq)

target_link_libraries(ppqueue zmq czmq)
target_link_libraries(ppworker zmq czmq)

target_link_libraries(mdclient zmq czmq)
target_link_libraries(mdworker zmq czmq)
target_link_libraries(mdbroker zmq czmq)
target_link_libraries(mdservicediscovery zmq czmq)
target_link_libraries(trippingtest zmq czmq ${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(mdclient_async zmq czmq)

target_link_libraries(titanic zmq czmq uuid)
target_link_libraries(ticlient zmq czmq uuid)

target_link_libraries(bstarsrv zmq czmq)
target_link_libraries(bstarcli zmq czmq)

target_link_libraries(bstarreactor zmq czmq)

target_link_libraries(freelancesvr zmq czmq)