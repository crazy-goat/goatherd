//
// Created by piotr on 12.11.17.
//

#ifndef SHEPHERD_WATCHDOG_H
#define SHEPHERD_WATCHDOG_H

#include <boost/asio/io_service.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/asio/deadline_timer.hpp>
#include "Worker.h"
#include "ConfigLoader.h"

namespace crazygoat::shepherd {
    class WatchDog {
    protected:
        boost::asio::io_service &ios;
        std::string command, params, socket_type;
        int count, start_socket;
        unsigned int requestsCount;
        std::vector<std::shared_ptr<Worker> > workers;
        std::shared_ptr<boost::asio::deadline_timer> timer;
        std::shared_ptr<Worker> workerIterator();
        void watch();

    public:
        WatchDog(boost::asio::io_service &ios, const std::shared_ptr<ConfigLoader> &config);

        void spawn();

        std::shared_ptr<Worker> getFreeWorker();
    };
}


#endif //SHEPHERD_WATCHDOG_H
