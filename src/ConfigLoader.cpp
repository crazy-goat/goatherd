//
// Created by piotr on 27.02.17.
//

#include "ConfigLoader.h"

namespace crazygoat::shepherd {
    int ConfigLoader::getListenPort() const {
        return listenPort;
    }

    int ConfigLoader::getWorkersCount() const {
        return workersCount;
    }

    int ConfigLoader::getStartPort() const {
        return startPort;
    }

    const std::string &ConfigLoader::getWorkerCommand() const {
        return workerCommand;
    }

    const std::string &ConfigLoader::getWorkerParams() const {
        return workerParams;
    }
}