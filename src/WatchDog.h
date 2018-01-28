//
// Created by piotr on 12.11.17.
//

#ifndef GOATHERD_WATCHDOG_H
#define GOATHERD_WATCHDOG_H

#define BOOST_THREAD_PROVIDES_FUTURE
#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION

#include "Config/ConfigLoader.h"
#include "Worker.h"
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/thread/future.hpp>

namespace crazygoat::goatherd {
class WatchDog {
protected:
  boost::asio::io_service &ios;
  unsigned int requestsCount;
  std::vector<std::shared_ptr<Worker>> workers;
  std::shared_ptr<boost::asio::deadline_timer> timer;
  std::shared_ptr<Worker> workerIterator();
  std::shared_ptr<ConfigLoader> config;
  void watch();
  unsigned int workerCount;

public:
  WatchDog(boost::asio::io_service &ios,
           const std::shared_ptr<ConfigLoader> &config);

  void spawn();
  void restartWorkers();
  boost::future<std::shared_ptr<Worker>> hasFreeWorker();
};
}

#endif // GOATHERD_WATCHDOG_H
