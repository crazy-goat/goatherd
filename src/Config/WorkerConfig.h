//
// Created by piotr on 30.11.17.
//

#ifndef SHEPHERD_WORKERCONFIG_H
#define SHEPHERD_WORKERCONFIG_H

#include "SocketParser.h"
#include <boost/property_tree/ptree.hpp>

namespace crazygoat::shepherd {
class WorkerConfig {
private:
  std::string socketType, command, params, socketPath, socketAddress;
  unsigned short startPort;

public:
  explicit WorkerConfig(boost::property_tree::ptree workerConfig) {
    SocketParser parser(workerConfig.get<std::string>("workerSocket"));

    this->command = workerConfig.get<std::string>("command");
    this->params = workerConfig.get<std::string>("params");

    this->socketType = parser.getType();
    if (this->socketType == SOCKET_TYPE_TCP) {
      this->startPort = parser.getPort();
      this->socketAddress = parser.getAddress();
      this->socketPath = "";
    } else {
      this->startPort = 0;
      this->socketAddress = "";
      this->socketPath = parser.getPath();
    }
  }

  const std::string &getSocketType() const { return socketType; }
  const std::string &getCommand() const { return command; }
  const std::string &getParams() const { return params; }
  const std::string &getSocketPath() const { return socketPath; }
  unsigned short getStartPort() const { return startPort; }
  const std::string &getSocketAddress() const { return socketAddress; }
};
}

#endif // SHEPHERD_WORKERCONFIG_H
