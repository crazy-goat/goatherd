//
// Created by piotr on 04.12.17.
//

#ifndef GOATHERD_SOCKETPARSER_H
#define GOATHERD_SOCKETPARSER_H

#include <iostream>

namespace crazygoat::goatherd {

const std::string SOCKET_TYPE_TCP = "tcp";
const std::string SOCKET_TYPE_UDS = "uds";

class SocketParser {
private:
  std::string type, path, address;
  unsigned short port;

  void parseSocketString(const std::string &socketString) {
    if (socketString.find("tcp://") == 0) {
      this->type = SOCKET_TYPE_TCP;
      return;
    }

    if (socketString.find("uds://") == 0) {
      this->type = SOCKET_TYPE_UDS;
      return;
    }

    throw "Invalid socket type, use tcp:// or uds://";
  };

  void parseTcpSocket(const std::string &socketString) {
    auto address_port = socketString.substr(6);
    auto pos = address_port.find(':');
    if (pos == std::string::npos) {
      throw "Invalid tcp address, should be address:port (example "
            "127.0.0.0:0) ";
    }
    this->address = address_port.substr(0, pos);
    this->port = (unsigned short)std::stoi(address_port.substr(pos + 1));
  }

  void parseUdsSocket(const std::string &socketString) {
    this->path = socketString.substr(6);
  }

public:
  explicit SocketParser(const std::string &socketString)
      : port(0), type(""), path(""), address("") {
    this->parseSocketString(socketString);
    this->type == "tcp" ? this->parseTcpSocket(socketString)
                        : this->parseUdsSocket(socketString);
  }
  const std::string &getType() const { return type; }
  unsigned short getPort() const { return port; }

  const std::string &getPath() const { return path; }

  const std::string &getAddress() const { return address; }
};
}

#endif // GOATHERD_SOCKETPARSER_H
