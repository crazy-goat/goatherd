//
// Created by piotr on 26.02.17.
//

#include "LoadBalancer.h"
#include "Acceptor/TcpAcceptor.h"
#include "Acceptor/UdsAcceptor.h"

namespace crazygoat::shepherd {

void LoadBalancer::handleAccept(const boost::system::error_code &error) {
  if (!error) {
    session->setWorker(this->watchDog->getFreeWorker());
    session->start();

    if (!acceptConnections()) {
      std::cerr << "Failure during call to accept." << std::endl;
    }
  } else {
    std::cerr << "Very bad error: " << error.message() << std::endl;
  }
}

bool LoadBalancer::acceptConnections() {
  try {
    session = boost::shared_ptr<Session>(new Session(ios));
    acceptor->accept(session->getDownstreamSocket(),
                     boost::bind(&LoadBalancer::handleAccept, this,
                                 boost::asio::placeholders::error));
  } catch (std::exception &e) {
    std::cerr << "acceptor exception: " << e.what() << std::endl;
    return false;
  }

  return true;
}

LoadBalancer::LoadBalancer(const std::shared_ptr<ConfigLoader> &config)
    : config(config) {
  this->watchDog = std::make_shared<WatchDog>(this->ios, this->config);
  this->monitor = std::make_shared<DirectoryMonitor>(this->ios, this->config, this->watchDog);

  if (config->getServerSocketType() == SOCKET_TYPE_TCP) {

    this->acceptor = std::make_shared<TcpAcceptor>(this->ios, this->config);
  } else {
    this->acceptor = std::make_shared<UdsAcceptor>(this->ios, this->config);
  }
}

const std::shared_ptr<WatchDog> &LoadBalancer::getWatchDog() const {
  return watchDog;
}

void LoadBalancer::run() { this->ios.run(); }
}
