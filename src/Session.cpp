//
// Created by piotr on 26.11.17.
//

#include "Session.h"

namespace crazygoat::goatherd {
Session::Session(boost::asio::io_service &ios)
    : downstreamSocket(ios), upstreamSocket(ios), strand(ios) {
  this->downstream_data[0] = 0;
  this->upstream_data[0] = 0;
}

void Session::close(const boost::system::error_code &error) {
  if (downstreamSocket.is_open()) {
    if (error.value() != 2 && error.value() != 125) {
      std::cerr << "503: " << error.value() << " : " << error.message()
                << std::endl;
      std::string errorMessage = "HTTP/1.x 503 Service Unavailable\r\n"
                                 "Content-Type' => 'text/plain\r\n"
                                 "\r\n"
                                 "The server is currently unavailable (because "
                                 "it is overloaded or down for maintenance)";

      downstreamSocket.write_some(
          boost::asio::buffer(errorMessage.c_str(), errorMessage.length()));
    }

    downstreamSocket.close();
  }

  if (upstreamSocket.is_open()) {
    this->worker->setIsWorking(false);
    upstreamSocket.close();
  }
}

void Session::handleUpstreamWrite(const boost::system::error_code &error) {
  if (!error) {
    downstreamSocket.async_read_some(
        boost::asio::buffer(downstream_data, max_data_length),
        strand.wrap(boost::bind(&Session::handleDownstreamRead,
                                shared_from_this(),
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred)));
  } else
    close(error);
}

void Session::handleDownstreamRead(const boost::system::error_code &error,
                                   const size_t &bytes_transferred) {
  if (!error) {
    async_write(upstreamSocket,
                boost::asio::buffer(downstream_data, bytes_transferred),
                strand.wrap(boost::bind(&Session::handleUpstreamWrite,
                                        shared_from_this(),
                                        boost::asio::placeholders::error)));
  } else
    close(error);
}

void Session::handleDownstreamWrite(const boost::system::error_code &error) {
  if (!error) {
    upstreamSocket.async_read_some(
        boost::asio::buffer(upstream_data, max_data_length),
        strand.wrap(boost::bind(&Session::handleUpstreamRead,
                                shared_from_this(),
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred)));
  } else
    close(error);
}

void Session::handleUpstreamRead(const boost::system::error_code &error,
                                 const size_t &bytes_transferred) {
  if (!error) {
    async_write(downstreamSocket,
                boost::asio::buffer(upstream_data, bytes_transferred),
                boost::bind(&Session::handleDownstreamWrite, shared_from_this(),
                            boost::asio::placeholders::error));
  } else
    close(error);
}

void Session::handleUpstreamConnect(const boost::system::error_code &error) {
  if (!error) {
    // Setup async read from remote server (upstream)
    upstreamSocket.async_read_some(
        boost::asio::buffer(upstream_data, max_data_length),
        boost::bind(&Session::handleUpstreamRead, shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));

    // Setup async read from client (downstream)
    downstreamSocket.async_read_some(
        boost::asio::buffer(downstream_data, max_data_length),
        boost::bind(&Session::handleDownstreamRead, shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
  } else
    close(error);
}

void Session::start() {
  // Attempt connection to remote server (upstream side)
  this->worker.get()->handleUpstreamConnect(
      upstreamSocket,
      boost::bind(&Session::handleUpstreamConnect, shared_from_this(),
                  boost::asio::placeholders::error));
}

void Session::setWorker(const std::shared_ptr<Worker> &worker) {
  this->worker = worker;
}

boost::asio::generic::stream_protocol::socket &Session::getDownstreamSocket() {
  return downstreamSocket;
}
}