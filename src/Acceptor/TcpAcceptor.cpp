//
// Created by piotr on 25.11.17.
//

#include "TcpAcceptor.h"

namespace crazygoat::shepherd {

TcpAcceptor::TcpAcceptor(boost::asio::io_service &io_service,
                         const std::shared_ptr<ConfigLoader> &config)
    : acceptor(std::make_shared<boost::asio::ip::tcp::acceptor>(
          io_service, boost::asio::ip::tcp::endpoint(
                          boost::asio::ip::address_v4::from_string(
                              config->getServerSocketAddress()),
                          config->getListenPort()))) {
  this->acceptor->set_option(
      boost::asio::ip::tcp::acceptor::reuse_address(true));
}

void TcpAcceptor::accept(boost::asio::generic::stream_protocol::socket &socket,
                         AsyncAcceptor function) {

  this->acceptor->async_accept(socket, function);
}
}