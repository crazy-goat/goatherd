#include "Worker.h"
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/local/stream_protocol.hpp>

namespace crazygoat::shepherd {

void Worker::spawn() {
  std::cout << "Spawning process on port:" << this->port << std::endl;

  this->process = std::make_shared<boost::process::child>(
      this->workerConfig->getCommand(),
      this->replaceSocket(this->workerConfig->getParams(), "%%socket%%",
                          std::to_string(this->port)), // set the input
      boost::process::std_in.close());
  this->isWorking = false;
}
std::vector<std::string> Worker::replaceSocket(std::string subject,
                                               const std::string &search,
                                               const std::string &replace) {
  boost::algorithm::replace_all(subject, search, replace);
  std::vector<std::string> returnParams;
  boost::split(returnParams, subject, boost::is_any_of("\t "));
  return returnParams;
}

bool Worker::isProcessRunning() const { return this->process->running(); }

unsigned short Worker::getPort() const { return port; }

bool Worker::isIsWorking() const { return isWorking; }

void Worker::setIsWorking(bool isWorking) {
  if (!isWorking && this->needRestart) {
    Worker::isWorking = true;
    this->restartWorker();
  } else {
    Worker::isWorking = isWorking;
  }
}

void Worker::handleUpstreamConnect(
    boost::asio::generic::stream_protocol::socket &socket,
    UpstreamConnect function) {

  if (this->workerConfig->getSocketType() == SOCKET_TYPE_TCP) {
    socket.async_connect(boost::asio::ip::tcp::endpoint(
                             boost::asio::ip::address::from_string(
                                 this->workerConfig->getSocketAddress()),
                             this->getPort()),
                         function);
  } else {
    socket.async_connect(boost::asio::local::stream_protocol::endpoint(
                             this->workerConfig->getSocketPath() + "." +
                             std::to_string(this->getPort())),
                         function);
  }
}
void Worker::restartWorker() {
  if (this->process->valid()) {
    this->process->terminate();
    this->spawn();
  }
  this->needRestart = false;
  this->isWorking = false;
}
void Worker::setNeedRestart() {
  if (!this->isIsWorking()) {
    this->restartWorker();
  } else {
    this->needRestart = true;
  }
}
}