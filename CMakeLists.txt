cmake_minimum_required(VERSION 3.5)
project(shepherd)
find_package(Threads)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
set(SOURCE_FILES src/main.cpp src/LoadBalancer.h src/Config/ConfigLoader.h src/LoadBalancer.cpp src/Worker.cpp src/Worker.h src/WatchDog.cpp src/WatchDog.h src/Acceptor/TcpAcceptor.cpp src/Acceptor/TcpAcceptor.h src/Session.cpp src/Session.h src/Acceptor/Acceptor.h src/Acceptor/UdsAcceptor.cpp src/Acceptor/UdsAcceptor.h src/Config/WorkerConfig.h src/Config/SocketParser.h)
add_executable(shepherd ${SOURCE_FILES})
#set(CMAKE_EXE_LINKER_FLAGS "-fsanitize=memory")

target_include_directories(shepherd PUBLIC vendor/boost)
find_library(BOOST_SYSTEM libboost_system.a HINTS vendor/boost/stage/lib)
find_library(BOOST_FILESYSTEM libboost_filesystem.a HINTS vendor/boost/stage/lib)

target_link_libraries(shepherd PUBLIC ${BOOST_SYSTEM} ${BOOST_THREAD} ${BOOST_FILESYSTEM} ${CMAKE_THREAD_LIBS_INIT})
