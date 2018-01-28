cmake_minimum_required(VERSION 3.5)
option(USE_INTERNAL_BOOST "User internal Boost" OFF)

project(shepherd)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
set(SOURCE_FILES src/main.cpp
        src/LoadBalancer.cpp
        src/Worker.cpp
        src/WatchDog.cpp
        src/Acceptor/TcpAcceptor.cpp
        src/Session.cpp
        src/Acceptor/UdsAcceptor.cpp)

find_package(Threads)
add_executable(shepherd ${SOURCE_FILES})
target_include_directories(shepherd PUBLIC vendor/dir_monitor/include)

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/vendor/dir_monitor)
    execute_process(COMMAND git clone https://github.com/berkus/dir_monitor.git
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
endif()

if (${USE_INTERNAL_BOOST})
    message("Downloading Boost")
    execute_process(COMMAND ./initboost.sh
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
    target_include_directories(shepherd PUBLIC vendor/boost)

    find_library(BOOST_SYSTEM libboost_system.a
            HINTS vendor/boost/stage/lib)
    find_library(BOOST_FILESYSTEM libboost_filesystem.a
            HINTS vendor/boost/stage/lib)
    find_library(BOOST_THREAD libboost_thread.a
            HINTS vendor/boost/stage/lib)

    target_link_libraries(shepherd PUBLIC
            ${BOOST_SYSTEM} ${BOOST_THREAD}
            ${BOOST_FILESYSTEM}
            ${CMAKE_THREAD_LIBS_INIT})

else()
    find_package(Boost 1.64 COMPONENTS process filesystem system asio)
    target_link_libraries(shepherd PUBLIC
            Boost::filesystem
            Boost::prcess
            Boost::asio
            Boost::system)
endif()

install(TARGETS shepherd DESTINATION bin)
