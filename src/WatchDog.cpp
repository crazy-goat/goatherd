//
// Created by piotr on 12.11.17.
//

#include "WatchDog.h"
#include <boost/asio/steady_timer.hpp>
#include <boost/chrono.hpp>

namespace crazygoat::shepherd {

WatchDog::WatchDog(boost::asio::io_service &ios,
                   const std::shared_ptr<ConfigLoader> &config)
    : ios(ios), config(config) {
  this->timer = std::make_shared<boost::asio::deadline_timer>(
      this->ios, boost::posix_time::seconds(1));
  this->requestsCount = 0;
}

void WatchDog::spawn() {
  for (int i = 0; i < this->config->getWorkersCount(); i++) {
    std::shared_ptr<Worker> tmp =
        std::make_shared<Worker>(this->config->getWorkerConfig(), i);
    tmp->spawn();
    this->workers.push_back(std::move(tmp));
  }
  this->timer->async_wait(boost::bind(&WatchDog::watch, this));
}

void WatchDog::watch() {
  for (auto const &worker : this->workers) {
    if (!worker->isProcessRunning()) {
      worker->spawn();
    }
  }

  this->timer->expires_at(this->timer->expires_at() +
                          boost::posix_time::milliseconds(1000));
  this->timer->async_wait(boost::bind(&WatchDog::watch, this));
}

std::shared_ptr<Worker> WatchDog::getFreeWorker() {
  std::future<std::shared_ptr<Worker>> fWorker =
      std::async(&WatchDog::workerIterator, this);

  while (fWorker.wait_for(std::chrono::microseconds(5)) !=
         std::future_status::ready) {
  }

  return fWorker.get();
}

std::shared_ptr<Worker> WatchDog::workerIterator() {
  do {
    auto worker = this->workers[(++this->requestsCount) %
                                this->config->getWorkersCount()];

    if (!worker->isIsWorking()) {
      return worker;
    }
  } while (true);
}
}