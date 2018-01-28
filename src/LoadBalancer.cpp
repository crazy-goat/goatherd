//
// Created by piotr on 26.02.17.
//
#include "LoadBalancer.h"
#include "Acceptor/TcpAcceptor.h"
#include "Acceptor/UdsAcceptor.h"

namespace crazygoat::goatherd {

void LoadBalancer::handleAccept(const boost::system::error_code &error) {
  if (!error) {
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
    session = boost::shared_ptr<Session>(new Session(this));
    acceptor->accept(session->getDownstreamSocket(),
                     boost::bind(&LoadBalancer::handleAccept, this,
                                 boost::asio::placeholders::error));
  } catch (std::exception &e) {
    std::cerr << "acceptor exception: " << e.what() << std::endl;
    return false;
  }

  return true;
}

LoadBalancer::LoadBalancer(const std::shared_ptr<ConfigLoader> &config) {

  for (short unsigned int i = 0; i < config->getWorkersCount(); i++) {
    auto tmp = std::make_unique<Worker>(config->getWorkerConfig(), i, ios);

    tmp->spawn();
    this->freeWorkers.emplace(tmp.get());
    this->workers.push_back(std::move(tmp));
  }

  this->watchDog = std::make_shared<WatchDog>(this->ios, config, &this->workers);
  this->monitor =
      std::make_unique<DirectoryMonitor>(this->ios, config, this->watchDog);

  if (config->getServerSocketType() == SOCKET_TYPE_TCP) {
    this->acceptor = std::make_unique<TcpAcceptor>(this->ios, config);
  } else {
    this->acceptor = std::make_unique<UdsAcceptor>(this->ios, config);
  }
}

void LoadBalancer::run() { this->ios.run(); }

void LoadBalancer::addFreeWorker(Worker *worker) {
  worker->setIsWorking(false);
  this->freeWorkers.push(worker);
}

Worker *LoadBalancer::getFreeWorker() {
  while (freeWorkers.empty()) ios.poll_one();
  auto worker = freeWorkers.front();
  freeWorkers.pop();
  worker->setIsWorking(true);
  return worker;
}
}
