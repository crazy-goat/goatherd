// Distributed under the Boost Software License, Version 1.0.

#include "LoadBalancer.h"
#include "Worker.h"

using namespace crazygoat::shepherd;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "usage: shepherd <config_file.json>" << std::endl;
        return 1;
    }

    Worker w("/bin/bash");
    w.spawn();
    Worker w2("/bin/bash");
    w2.spawn();

    boost::asio::io_service ios;
    boost::asio::io_service::work work(ios);
    boost::thread_group threadPool;
    try {

        ConfigLoader config(argv[1]);

        LoadBalancer::acceptor acceptor(ios, config);
        acceptor.accept_connections();

        for (int i = 0; i < config.getThreads(); i++) {
            threadPool.create_thread(
                    boost::bind(&boost::asio::io_service::run, &ios)
            );
        }

        threadPool.join_all();
    }
    catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
