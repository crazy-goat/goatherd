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
  const std::shared_ptr<WatchDog> &getWatchDog() const;
  void run();

private:
  void handleAccept(const boost::system::error_code &error);
  boost::asio::io_service ios;
  boost::shared_ptr<Session> session;
  std::shared_ptr<WatchDog> watchDog;
  std::shared_ptr<Acceptor> acceptor;
  std::shared_ptr<DirectoryMonitor> monitor;
};
}

#endif // PPPM_LOADBALANCER_H
