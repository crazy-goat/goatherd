//
// Created by piotr on 27.11.17.
//

#ifndef GOATHERD_UDSACCEPTOR_H
#define GOATHERD_UDSACCEPTOR_H

#include "../Config/ConfigLoader.h"
#include "Acceptor.h"
#include <boost/asio/local/stream_protocol.hpp>

namespace crazygoat::goatherd {
class UdsAcceptor : public Acceptor {
public:
  UdsAcceptor(boost::asio::io_service &io_service,
              const std::shared_ptr<ConfigLoader> &config);
  void accept(boost::asio::generic::stream_protocol::socket &socket,
              AsyncAcceptor function) override;

private:
  std::shared_ptr<boost::asio::local::stream_protocol::acceptor> acceptor;
};
}
#endif // GOATHERD_UDSACCEPTOR_H
