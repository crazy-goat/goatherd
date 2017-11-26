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
        LoadBalancer(
                boost::asio::io_service &ios,
                const std::shared_ptr<TcpAcceptor> &acceptor,
                const std::shared_ptr<WatchDog> &watchDog
        ) : acceptor_(acceptor->getAcceptor()), ios(ios) {
            this->watchDog = watchDog;
        };

        bool accept_connections();

    private:
        void handle_accept(const boost::system::error_code &error);

        boost::asio::io_service &ios;
        boost::shared_ptr<Session> session_;
        std::shared_ptr<WatchDog> watchDog;
        std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
    };
}

#endif //PPPM_LOADBALANCER_H
