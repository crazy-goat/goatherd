//
// Created by piotr on 11.11.17.
//

#ifndef SHEPHERD_WORKER_H
#define SHEPHERD_WORKER_H

#include "Config/WorkerConfig.h"
#include <boost/asio/generic/stream_protocol.hpp>
#include <boost/process.hpp>
#include <iostream>

namespace crazygoat::shepherd {

class Worker {
protected:
  unsigned short port;
  std::shared_ptr<boost::process::child> process;
  std::shared_ptr<WorkerConfig> workerConfig;

  std::vector<std::string> replaceSocket(std::string subject,
                                         const std::string &search,
                                         const std::string &replace);

  bool isWorking;

public:
  typedef boost::function<void(const boost::system::error_code &error)>
      UpstreamConnect;

  unsigned short getPort() const;

  explicit Worker(std::shared_ptr<WorkerConfig> workerConfig,
                  unsigned short workerNumber) {
    this->workerConfig = workerConfig;
    this->port = workerConfig->getStartPort() + workerNumber;
    this->isWorking = false;
  };

  void spawn();

  bool isProcessRunning() const;

  bool isIsWorking() const;

  void setIsWorking(bool isWorking);

  void
  handleUpstreamConnect(boost::asio::generic::stream_protocol::socket &socket,
                        UpstreamConnect function);
};
}

#endif // SHEPHERD_WORKER_H
