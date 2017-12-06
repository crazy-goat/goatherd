//
// Created by piotr on 12.11.17.
//

#ifndef SHEPHERD_WATCHDOG_H
#define SHEPHERD_WATCHDOG_H

#include "Config/ConfigLoader.h"
#include "Worker.h"
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>

namespace crazygoat::shepherd {
class WatchDog {
protected:
  boost::asio::io_service &ios;
  unsigned int requestsCount;
  std::vector<std::shared_ptr<Worker>> workers;
  std::shared_ptr<boost::asio::deadline_timer> timer;
  std::shared_ptr<Worker> workerIterator();
  std::shared_ptr<ConfigLoader> config;
  void watch();

public:
  WatchDog(boost::asio::io_service &ios,
           const std::shared_ptr<ConfigLoader> &config);

  void spawn();

  std::shared_future<std::shared_ptr<Worker>> getFreeWorker();
};
}

#endif // SHEPHERD_WATCHDOG_H
