//
// Created by piotr on 27.02.17.
//

#include "ConfigLoader.h"

int pppm::ConfigLoader::getListenPort() const {
    return listenPort;
}

int pppm::ConfigLoader::getThreads() const {
    return threads;
}

int pppm::ConfigLoader::getWorkersCount() const {
    return workersCount;
}

int pppm::ConfigLoader::getStartPort() const {
    return startPort;
}

const std::string &pppm::ConfigLoader::getHostname() const {
    return hostname;
}
