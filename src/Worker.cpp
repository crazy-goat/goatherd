#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/local/stream_protocol.hpp>
#include "Worker.h"

namespace crazygoat::shepherd {

    void Worker::spawn() {
        std::cout << "Spawning process on port:" << this->port << std::endl;

        this->process = std::make_shared<boost::process::child>(
                boost::process::search_path(this->command),
                this->replaceSocket(
                    this->params, "%%socket%%",
                    std::to_string(
                        this->port
                    )
                ), //set the input
                boost::process::std_in.close()
        );
        this->isWorking = false;
    }

    Worker::Worker(std::string socket_type, std::string command, std::string params, unsigned short port, std::string socket_path) :
            socket_type(socket_type),
            command(command),
            params(params),
            port(port),
            isWorking(false),
            socket_path(socket_path){}

    std::vector<std::string> Worker::replaceSocket(std::string subject, const std::string &search,
                                                   const std::string &replace) {
        boost::algorithm::replace_all(subject, search, replace);
        std::vector<std::string> returnParams;
        boost::split(returnParams, subject, boost::is_any_of("\t "));
        return returnParams;
    }

    bool Worker::isProcessRunning() const {
        return this->process->running();
    }

    unsigned short Worker::getPort() const {
        return port;
    }

    bool Worker::isIsWorking() const {
        return isWorking;
    }

    void Worker::setIsWorking(bool isWorking) {
        Worker::isWorking = isWorking;
    }

    void Worker::handleUpstreamConnect(boost::asio::generic::stream_protocol::socket &socket, UpstreamConnect function) {
        if (this->socket_type == "tcp") {
            socket.async_connect(
                boost::asio::ip::tcp::endpoint(
                    boost::asio::ip::address::from_string("127.0.0.1"),
                    this->getPort()
                ),
                function
            );
        } else {
            socket.async_connect(
                boost::asio::local::stream_protocol::endpoint(
                    this->socket_path+"."+std::to_string(this->getPort())
                ),
                function
            );
        }
    }
}