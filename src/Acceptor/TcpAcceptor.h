//
// Created by piotr on 25.11.17.
//

#ifndef GOATHERD_TCPACCEPTOR_H
#define GOATHERD_TCPACCEPTOR_H

#include "../Config/ConfigLoader.h"
#include "../WatchDog.h"
#include "Acceptor.h"
#include <boost/asio.hpp>

namespace crazygoat::goatherd {
class TcpAcceptor : public Acceptor {
public:
  TcpAcceptor(boost::asio::io_service &io_service,
              const std::shared_ptr<ConfigLoader> &config);

  void accept(boost::asio::generic::stream_protocol::socket &socket,
              AsyncAcceptor function) override;

private:
  std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor;
};
}

#endif // GOATHERD_TCPACCEPTOR_H
