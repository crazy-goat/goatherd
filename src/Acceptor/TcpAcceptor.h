//
// Created by piotr on 25.11.17.
//

#ifndef SHEPHERD_TCPACCEPTOR_H
#define SHEPHERD_TCPACCEPTOR_H

#include <boost/asio.hpp>
#include "../Config/ConfigLoader.h"
#include "../WatchDog.h"
#include "Acceptor.h"

namespace crazygoat::shepherd {
    class TcpAcceptor : public Acceptor {
    public:
        TcpAcceptor(
                boost::asio::io_service &io_service,
                const std::shared_ptr<ConfigLoader> &config
        );

        void accept(boost::asio::generic::stream_protocol::socket &socket, AsyncAcceptor function) override;

    private:
        std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor;
    };
}

#endif //SHEPHERD_TCPACCEPTOR_H
