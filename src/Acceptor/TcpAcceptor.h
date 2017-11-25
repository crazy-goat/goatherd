//
// Created by piotr on 25.11.17.
//

#ifndef SHEPHERD_TCPACCEPTOR_H
#define SHEPHERD_TCPACCEPTOR_H

#include <boost/asio.hpp>
#include "../ConfigLoader.h"
#include "../WatchDog.h"

namespace crazygoat::shepherd {
    class TcpAcceptor {
    public:
        TcpAcceptor(
                boost::asio::io_service &io_service,
                const std::shared_ptr<ConfigLoader> &config
        );

        bool accept_connections();

    private:
        void handle_accept(const boost::system::error_code &error);

        std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor;
    };
}

#endif //SHEPHERD_TCPACCEPTOR_H
