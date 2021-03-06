//
// Created by piotr on 11.11.17.
//

#ifndef GOATHERD_WORKER_H
#define GOATHERD_WORKER_H

#include "Config/WorkerConfig.h"
#include <boost/asio/generic/stream_protocol.hpp>
#include <boost/process.hpp>
#include <iostream>

namespace crazygoat::goatherd {

class Worker {
protected:
  unsigned short port;
  std::shared_ptr<boost::process::child> process;
  std::shared_ptr<WorkerConfig> workerConfig;

protected:
  std::vector<std::string> replaceSocket(std::string subject,
                                         const std::string &search,
                                         const std::string &replace);

  bool working, needRestart;

  void restartWorker();

public:
  typedef boost::function<void(const boost::system::error_code &error)>
      UpstreamConnect;

  unsigned short getPort() const;

  explicit Worker(std::shared_ptr<WorkerConfig> workerConfig,
                  unsigned short workerNumber, boost::asio::io_service &ios) {
    this->workerConfig = workerConfig;
    this->port = workerConfig->getStartPort() + workerNumber;
    this->working = false;
    this->needRestart = false;
  };

  void spawn();

  bool isProcessRunning() const;

  bool isWorking() const;

  void setIsWorking(bool isWorking);
  void setNeedRestart();
  void
  handleUpstreamConnect(boost::asio::generic::stream_protocol::socket &socket,
                        UpstreamConnect function);
};
}

#endif // GOATHERD_WORKER_H
