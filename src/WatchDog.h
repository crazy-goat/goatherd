//
// Created by piotr on 12.11.17.
//

#ifndef GOATHERD_WATCHDOG_H
#define GOATHERD_WATCHDOG_H

#include "Config/ConfigLoader.h"
#include "Worker.h"
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <queue>

namespace crazygoat::goatherd {
class WatchDog {
protected:
  boost::asio::io_service &ios;
  std::shared_ptr<boost::asio::deadline_timer> timer;

  std::shared_ptr<ConfigLoader> config;
  std::vector<std::unique_ptr<Worker>> *workers;
  void watch();
  unsigned int workerCount;

public:
  WatchDog(boost::asio::io_service &ios,
           const std::shared_ptr<ConfigLoader> &config,
           std::vector<std::unique_ptr<Worker>> *workers);

  void restartWorkers();
};
}

#endif // GOATHERD_WATCHDOG_H
