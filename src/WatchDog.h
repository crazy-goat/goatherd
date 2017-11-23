//
// Created by piotr on 12.11.17.
//

#ifndef SHEPHERD_WATCHDOG_H
#define SHEPHERD_WATCHDOG_H

#include <boost/asio/io_service.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/asio/deadline_timer.hpp>
#include "Worker.h"

namespace crazygoat::shepherd {
    class WatchDog {
    protected:
        boost::asio::io_service &ios;
        std::string command, params;
        int count, start_port;
        unsigned int requestsCount;
        std::vector<std::shared_ptr<Worker> > workers;
        std::shared_ptr<boost::asio::deadline_timer> timer;
        void watch();

    public:
        WatchDog(boost::asio::io_service &ios, std::string command, std::string params, int count, int start_port);

        void spawn();

        std::shared_ptr<Worker> getFreeWorker();
    };
}


#endif //SHEPHERD_WATCHDOG_H
