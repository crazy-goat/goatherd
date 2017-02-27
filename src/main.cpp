// Distributed under the Boost Software License, Version 1.0.

#include "LoadBalancer.h"
#include "ConfigLoader.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "usage: pppm <config_file.json>" << std::endl;
        return 1;
    }

    boost::asio::io_service ios;
    boost::asio::io_service::work work(ios);
    boost::thread_group threadpool;
    try {

        pppm::ConfigLoader config(argv[1]);

        pppm::LoadBalancer::acceptor acceptor(ios, config);
        acceptor.accept_connections();

        for (int i = 0; i < config.getThreads(); i++) {
            threadpool.create_thread(
                boost::bind(&boost::asio::io_service::run, &ios)
            );
        }

        threadpool.join_all();
    }
    catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}