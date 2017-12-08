//
// Created by piotr on 27.11.17.
//

#include "UdsAcceptor.h"

namespace crazygoat::shepherd {

void UdsAcceptor::accept(boost::asio::generic::stream_protocol::socket &socket,
                         Acceptor::AsyncAcceptor function) {
  this->acceptor->async_accept(socket, function);
}

UdsAcceptor::UdsAcceptor(boost::asio::io_service &io_service,
                         const std::shared_ptr<ConfigLoader> &config)
    : acceptor(std::make_shared<boost::asio::local::stream_protocol::acceptor>(
          io_service, boost::asio::local::stream_protocol::endpoint(
                          config->getServerSocketPath()))) {}
}