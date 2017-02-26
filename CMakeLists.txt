cmake_minimum_required(VERSION 3.5)
project(pppm)

set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.36.0 COMPONENTS system thread)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp src/LoadBalancer.h src/ConfigLoader.h src/LoadBalancer.cpp)
add_executable(pppm ${SOURCE_FILES})
target_link_libraries(pppm ${Boost_LIBRARIES})