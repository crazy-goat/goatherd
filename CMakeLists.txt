cmake_minimum_required(VERSION 3.5)
project(shepherd)
find_package(Threads)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES src/main.cpp src/LoadBalancer.h src/ConfigLoader.h src/LoadBalancer.cpp src/ConfigLoader.cpp src/Worker.cpp src/Worker.h src/WatchDog.cpp src/WatchDog.h)
add_executable(shepherd ${SOURCE_FILES})

target_include_directories(shepherd PUBLIC vendor/boost)
find_library(BOOST_SYSTEM libboost_system.a HINTS vendor/boost/stage/lib)
#find_library(BOOST_THREAD libboost_thread.a HINTS vendor/boost/stage/lib)
find_library(BOOST_FILESYSTEM libboost_filesystem.a HINTS vendor/boost/stage/lib)

target_link_libraries(shepherd PUBLIC ${BOOST_SYSTEM} ${BOOST_THREAD} ${BOOST_FILESYSTEM} ${CMAKE_THREAD_LIBS_INIT})
