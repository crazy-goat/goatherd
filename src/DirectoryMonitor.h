//
// Created by piotr on 05.12.17.
//

#ifndef SHEPHERD_DIRMONITOR_H
#define SHEPHERD_DIRMONITOR_H
#include "Config/ConfigLoader.h"
#include "WatchDog.h"
#include <dir_monitor/dir_monitor.hpp>

namespace crazygoat::shepherd {

class DirectoryMonitor {
private:
  std::shared_ptr<ConfigLoader> config;
  std::shared_ptr<WatchDog> watchDog;
  std::shared_ptr<boost::asio::dir_monitor> monitor;

  void directoryChangeHandler(const boost::system::error_code &ec,
                              const boost::asio::dir_monitor_event &ev) {
    std::cerr << "Watch dir changed, restarting workers" << std::endl;
    this->watchDog->restartWorkers();
    this->watch();
  }

  void watch() {
    this->monitor->async_monitor(
        boost::bind(&DirectoryMonitor::directoryChangeHandler, this, _1, _2));
  }

public:
  DirectoryMonitor(boost::asio::io_service &ios,
                   std::shared_ptr<ConfigLoader> config,
                   std::shared_ptr<WatchDog> watchDog)
      : config(std::move(config)), watchDog(std::move(watchDog)) {
    this->monitor = std::make_shared<boost::asio::dir_monitor>(ios);

    if (!this->config->getWatchDir().empty()) {
      boost::filesystem::path full_path(
          boost::filesystem::current_path().string() + "/" +
          this->config->getWatchDir());
      this->monitor->add_directory(full_path.string());

      std::cerr << "Watch dir: " << full_path.string() << std::endl;
      this->watch();
    }
  }
};
}
#endif // SHEPHERD_DIRMONITOR_H
