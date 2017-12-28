//
// Created by piotr on 26.11.17.
//

#ifndef GOATHERD_SESSION_H
#define GOATHERD_SESSION_H

#include "Worker.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

namespace crazygoat::goatherd {

class LoadBalancer;

class Session : public boost::enable_shared_from_this<Session> {
public:
  explicit Session(LoadBalancer *lb);
  ~Session();

  boost::asio::generic::stream_protocol::socket &getDownstreamSocket();

  void start();

  void handleUpstreamConnect(const boost::system::error_code &error);

  void setWorker(const std::shared_ptr<Worker> &worker);

private:
  /*
   * Section A: Remote Server --> Proxy --> Client
   * Process data recieved from remote sever then send to client.
  */

  // Read from remote server complete, now send data to client
  void handleUpstreamRead(const boost::system::error_code &error,
                          const size_t &bytes_transferred);

  // Write to client complete, Async read from remote server
  void handleDownstreamWrite(const boost::system::error_code &error);
  // *** End Of Section A ***

  /* Section B: Client --> Proxy --> Remove Server
   * Process data recieved from client then write to remove server.
  */

  // Read from client complete, now send data to remote server
  void handleDownstreamRead(const boost::system::error_code &error,
                            const size_t &bytes_transferred);

  // Write to remote server complete, Async read from client
  void handleUpstreamWrite(const boost::system::error_code &error);
  // *** End Of Section B ***

  void close(const boost::system::error_code &error);

  std::shared_ptr<Worker> worker;
  LoadBalancer *lb;

  boost::asio::generic::stream_protocol::socket downstreamSocket;
  boost::asio::generic::stream_protocol::socket upstreamSocket;

  enum { max_data_length = 8192 }; // 8KB
  unsigned char downstream_data[max_data_length];
  unsigned char upstream_data[max_data_length];

  /// Strand to ensure the connection's handlers are not called concurrently.
  boost::asio::io_service::strand strand;
};
}

#endif // GOATHERD_SESSION_H
