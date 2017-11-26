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

    private:
        std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor;
    public:
        const std::shared_ptr<boost::asio::ip::tcp::acceptor> &getAcceptor() const;
    };
}

#endif //SHEPHERD_TCPACCEPTOR_H
