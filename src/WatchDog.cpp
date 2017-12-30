//
// Created by piotr on 12.11.17.
//

#include "WatchDog.h"
#include <boost/asio/steady_timer.hpp>
#include <boost/chrono.hpp>

namespace crazygoat::goatherd {

WatchDog::WatchDog(boost::asio::io_service &ios,
                   const std::shared_ptr<ConfigLoader> &config,
                   std::vector<std::unique_ptr<Worker>> *workers)
    : ios(ios), config(config), workerCount(config->getWorkersCount()) {
  this->workers = workers;
  this->timer = std::make_shared<boost::asio::deadline_timer>(
      this->ios, boost::posix_time::seconds(1));
  this->timer->async_wait(boost::bind(&WatchDog::watch, this));

}

void WatchDog::watch() {
  for (auto const &worker : *this->workers) {
    if (!worker->isProcessRunning()) {
      worker->spawn();
    }
  }

  this->timer->expires_at(this->timer->expires_at() +
                          boost::posix_time::milliseconds(1000));
  this->timer->async_wait(boost::bind(&WatchDog::watch, this));
}

void WatchDog::restartWorkers() {
  for (auto const &worker : *this->workers) {
    worker->setNeedRestart();
  }
}

}