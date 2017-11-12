//
// Created by piotr on 27.02.17.
//

#include "ConfigLoader.h"

namespace crazygoat::shepherd {
    int ConfigLoader::getListenPort() const {
        return listenPort;
    }

    int ConfigLoader::getThreads() const {
        return threads;
    }

    int ConfigLoader::getWorkersCount() const {
        return workersCount;
    }

    int ConfigLoader::getStartPort() const {
        return startPort;
    }

    const std::string &ConfigLoader::getHostname() const {
        return hostname;
    }
}