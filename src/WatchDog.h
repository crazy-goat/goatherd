//
// Created by piotr on 12.11.17.
//

#ifndef SHEPHERD_WATCHDOG_H
#define SHEPHERD_WATCHDOG_H

#include <boost/asio/io_service.hpp>

namespace crazygoat::shepherd {
    class WatchDog {
    protected:
        boost::asio::io_service &ios;
    public:
        WatchDog(boost::asio::io_service &ios);
    };
}


#endif //SHEPHERD_WATCHDOG_H
