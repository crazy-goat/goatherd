cmake_minimum_required(VERSION 3.5)
project(shepherd)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES src/main.cpp src/LoadBalancer.h src/ConfigLoader.h src/LoadBalancer.cpp src/ConfigLoader.cpp src/Worker.cpp src/Worker.h)
add_executable(shepherd ${SOURCE_FILES})
target_include_directories(shepherd PUBLIC vendor/boost)
link_directories(vendor/boost/stage/lib)
target_link_libraries(shepherd PRIVATE boost_system boost_thread)