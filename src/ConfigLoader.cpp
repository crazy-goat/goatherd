//
// Created by piotr on 27.02.17.
//

#include "ConfigLoader.h"

namespace crazygoat::shepherd {
    unsigned short ConfigLoader::getListenPort() const {
        return listenPort;
    }

    int ConfigLoader::getWorkersCount() const {
        return workersCount;
    }

    int ConfigLoader::getStartSocket() const {
        return startSocket;
    }

    const std::string &ConfigLoader::getWorkerCommand() const {
        return workerCommand;
    }

    const std::string &ConfigLoader::getWorkerParams() const {
        return workerParams;
    }

    const std::string &ConfigLoader::getWorkerSocketType() const {
        return workerSocketType;
    }

    const std::string &ConfigLoader::getSocketPath() const {
        return socketPath;
    }
}