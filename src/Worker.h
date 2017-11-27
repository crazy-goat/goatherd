//
// Created by piotr on 11.11.17.
//

#ifndef SHEPHERD_WORKER_H
#define SHEPHERD_WORKER_H

#include <iostream>
#include <boost/process.hpp>
#include <boost/asio/generic/stream_protocol.hpp>

namespace crazygoat::shepherd {

    class Worker {
    protected:
        std::string command, params, socket_type, socket_path;
        unsigned short port;
        std::shared_ptr<boost::process::child> process;

        std::vector<std::string>
        replaceSocket(std::string subject, const std::string &search, const std::string &replace);

        bool isWorking;

    public:
        typedef boost::function<void(const boost::system::error_code &error)> UpstreamConnect;

        unsigned short getPort() const;

        Worker(std::string socket_type, std::string command, std::string params, unsigned short port, std::string socket_path);

        void spawn();

        bool isProcessRunning() const;

        bool isIsWorking() const;

        void setIsWorking(bool isWorking);

        void handleUpstreamConnect(boost::asio::generic::stream_protocol::socket &socket, UpstreamConnect function);
    };
}

#endif //SHEPHERD_WORKER_H
