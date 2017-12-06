//
// Created by piotr on 25.02.17.
//

#ifndef PPPM_CONFIGLOADER_H
#define PPPM_CONFIGLOADER_H

#include "WorkerConfig.h"
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace crazygoat::shepherd {

class ConfigLoader {
protected:
  unsigned short listenPort;
  int workersCount;
  std::string serverSocketType;
  std::string serverSocketAddress;
  std::string serverSocketPath;
  std::string watchDir;
  std::shared_ptr<WorkerConfig> workerConfig;

public:
  explicit ConfigLoader(const std::string &configFile) {
    boost::property_tree::ptree config;
    boost::property_tree::json_parser::read_json(configFile, config);
    SocketParser parser(config.get<std::string>("listenSocket"));

    this->listenPort = parser.getPort();
    this->serverSocketType = parser.getType();
    this->serverSocketAddress = parser.getAddress();
    this->serverSocketPath = parser.getPath();

    this->watchDir =
        config.get_optional<std::string>("watchDir").get_value_or("");
    this->workerConfig =
        std::make_shared<WorkerConfig>(config.get_child("worker"));

    this->workersCount = config.get<int>("workerCount");
  }

  unsigned short getListenPort() const { return listenPort; }

  int getWorkersCount() const { return workersCount; }

  std::shared_ptr<WorkerConfig> getWorkerConfig() { return workerConfig; }

  const std::string &getServerSocketType() const { return serverSocketType; }

  const std::string &getServerSocketAddress() const {
    return serverSocketAddress;
  }

  const std::string &getServerSocketPath() const { return serverSocketPath; }
  const std::string &getWatchDir() const { return watchDir; }
};
}
#endif // PPPM_CONFIGLOADER_H
