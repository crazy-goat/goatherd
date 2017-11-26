//
// Created by piotr on 25.02.17.
//

#ifndef PPPM_LOADBALANCER_H
#define PPPM_LOADBALANCER_H

#include <cstdlib>
#include <cstddef>
#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include "ConfigLoader.h"
#include "WatchDog.h"
#include "Acceptor/TcpAcceptor.h"
#include "Session.h"

namespace crazygoat::shepherd {

    class LoadBalancer {
    public:
        explicit LoadBalancer(const std::shared_ptr<ConfigLoader> &config);
        bool acceptConnections();
        const std::shared_ptr<WatchDog> &getWatchDog() const;
        void run();

    private:
        void handleAccept(const boost::system::error_code &error);
        std::shared_ptr<ConfigLoader> config;
        boost::asio::io_service ios;
        boost::shared_ptr<Session> session;
        std::shared_ptr<WatchDog> watchDog;
        std::shared_ptr<TcpAcceptor> acceptor;
    };
}

#endif //PPPM_LOADBALANCER_H
