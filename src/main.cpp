// Distributed under the Boost Software License, Version 1.0.

#include "LoadBalancer.h"

using namespace crazygoat::shepherd;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "usage: shepherd <config_file.json>" << std::endl;
        return 1;
    }

    boost::asio::io_service ios;
    try {

        auto config = std::make_shared<ConfigLoader>(argv[1]);
        auto watchDog = std::make_shared<WatchDog>(ios, config);
        watchDog->spawn();

        LoadBalancer::acceptor acceptor(ios, config, watchDog);
        acceptor.accept_connections();

        ios.run();
    }
    catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
