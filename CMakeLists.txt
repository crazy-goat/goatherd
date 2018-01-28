cmake_minimum_required(VERSION 3.5)
project(goatherd)
SET(MAJOR_VERSION "0")
SET(MINOR_VERSION "3")
SET(PATCH_VERSION "0")

option(USE_INTERNAL_BOOST "User internal Boost" OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
set(SOURCE_FILES src/main.cpp
        src/LoadBalancer.cpp
        src/Worker.cpp
        src/WatchDog.cpp
        src/Acceptor/TcpAcceptor.cpp
        src/Session.cpp
        src/Acceptor/UdsAcceptor.cpp)

find_package(Threads)
add_executable(goatherd ${SOURCE_FILES})
target_include_directories(goatherd PUBLIC vendor/dir_monitor/include)
set_target_properties(goatherd PROPERTIES LINK_FLAGS "-static" )

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/vendor/dir_monitor)
    execute_process(COMMAND git clone https://github.com/berkus/dir_monitor.git
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
endif()

if (${USE_INTERNAL_BOOST})
    message("Downloading Boost")
    execute_process(COMMAND ./initboost.sh
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
    target_include_directories(goatherd PUBLIC vendor/boost)

    find_library(BOOST_SYSTEM libboost_system.a
            HINTS vendor/boost/stage/lib)
    find_library(BOOST_FILESYSTEM libboost_filesystem.a
            HINTS vendor/boost/stage/lib)
    find_library(BOOST_THREAD libboost_thread.a
            HINTS vendor/boost/stage/lib)

    target_link_libraries(goatherd PUBLIC
            ${BOOST_SYSTEM} ${BOOST_THREAD}
            ${BOOST_FILESYSTEM}
            ${CMAKE_THREAD_LIBS_INIT})

else()
    find_package(Boost 1.64 COMPONENTS process filesystem system asio)
    target_link_libraries(goatherd PUBLIC
            Boost::filesystem
            Boost::process
            Boost::asio
            Boost::system)
endif()

install(TARGETS goatherd DESTINATION bin)

SET(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "goatherd")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Piotr Hałas")
SET(CPACK_PACKAGE_VENDOR "crazy-goat.com")
SET(CPACK_PACKAGE_DESCRIPTION "TCP load balancer and proces manager ")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Goatherd is free, open source, languange independend, \
fast and lightweight load balancer with build-in process manager for any TCP/IP serivces. \
It's written using modern c++17 and well-known libraries like Boost.Asio for networking \
or Boost.Process for process monitoring.")
SET(CPACK_PACKAGE_CONTACT "piotr@halas.net.pl")
SET(CPACK_PACKAGE_VERSION_MAJOR "${MAJOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_MINOR "${MINOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")
SET(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")
SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
SET(CPACK_DEBIAN_PACKAGE_SECTION "net")
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE)
set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
SET(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
INCLUDE(CPack)