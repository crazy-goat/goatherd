//
// Created by piotr on 25.02.17.
//

#ifndef PPPM_LOADBALANCER_H
#define PPPM_LOADBALANCER_H

#include "Acceptor/Acceptor.h"
#include "DirectoryMonitor.h"
#include "Session.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <cstddef>
#include <cstdlib>
#include <iostream>

namespace crazygoat::goatherd {

class LoadBalancer {
public:
  explicit LoadBalancer(const std::shared_ptr<ConfigLoader> &config);
  bool acceptConnections();
  void run();

  void addFreeWorker(Worker *worker);
  Worker *getFreeWorker();
  boost::asio::io_service ios;
private:
  void handleAccept(const boost::system::error_code &error);

  std::queue<Worker*> freeWorkers;
  std::vector<std::unique_ptr<Worker>> workers;

  boost::shared_ptr<Session> session;
  std::shared_ptr<WatchDog> watchDog;
  std::unique_ptr<Acceptor> acceptor;
  std::unique_ptr<DirectoryMonitor> monitor;
};
}

#endif // PPPM_LOADBALANCER_H
