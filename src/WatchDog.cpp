//
// Created by piotr on 12.11.17.
//

#include "WatchDog.h"
#include <boost/asio/steady_timer.hpp>
#include <boost/chrono.hpp>

namespace crazygoat::goatherd {

WatchDog::WatchDog(boost::asio::io_service &ios,
                   const std::shared_ptr<ConfigLoader> &config)
    : ios(ios), config(config), workerCount(config->getWorkersCount()) {
  this->timer = std::make_shared<boost::asio::deadline_timer>(
      this->ios, boost::posix_time::seconds(1));
  this->requestsCount = 0;
}

void WatchDog::spawn() {
  for (int i = 0; i < this->config->getWorkersCount(); i++) {
    std::shared_ptr<Worker> tmp =
        std::make_shared<Worker>(this->config->getWorkerConfig(), i, ios);
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

void WatchDog::restartWorkers() {
  for (const auto &worker : this->workers) {
    worker->setNeedRestart();
  }
}
boost::future<std::shared_ptr<Worker>> WatchDog::hasFreeWorker() {
  return boost::async(boost::launch::async,
                      [this]() -> std::shared_ptr<Worker> {
                        while (1) {
                          auto worker = workers[requestsCount++ % workerCount];
                          if (!worker->isWorking()) {
                            worker->setIsWorking(true);
                            return worker;
                          }
                        };
                      });
}
}