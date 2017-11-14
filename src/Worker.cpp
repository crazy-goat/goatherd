#include "Worker.h"

namespace crazygoat::shepherd {

    void Worker::spawn() {
        std::cout << "Spawning process on port:" << this->port << std::endl;

        this->process = std::make_shared<boost::process::child>(
                boost::process::search_path(this->command),
                this->replacePort(
                        this->params, "%%port%%",
                        std::to_string(
                                this->port
                        )
                ), //set the input
                boost::process::std_in.close()
        );
    }

    Worker::Worker(std::string command, std::string params, int port) :
            command(command),
            params(params),
            port(port) {}

    std::vector<std::string> Worker::replacePort(std::string subject, const std::string &search,
                                                 const std::string &replace) {
        boost::algorithm::replace_all(subject, search, replace);
        std::vector<std::string> returnParams;
        boost::split(returnParams, subject, boost::is_any_of("\t "));
        return returnParams;
    }

    bool Worker::isWorking() const {
        return this->process->running();
    }

    unsigned short Worker::getPort() const {
        return port;
    }
}