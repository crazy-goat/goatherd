// Distributed under the Boost Software License, Version 1.0.

#include "LoadBalancer.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "usage: tcpproxy_server <local port> <forward host ip> <forward port>" << std::endl;
        return 1;
    }

    const unsigned short local_port = static_cast<unsigned short>(::atoi(argv[1]));
    const unsigned short forward_port = static_cast<unsigned short>(::atoi(argv[3]));
    const std::string forward_host = argv[2];

    boost::asio::io_service ios;
    boost::asio::io_service pool;
    boost::asio::io_service::work work(pool);
    boost::thread_group threadpool;
    try {
        pppm::LoadBalancer::acceptor acceptor(ios, pool, local_port, forward_host, forward_port);

        for (int i = 0; i < 4; i++) {
            threadpool.create_thread(
                boost::bind(&boost::asio::io_service::run, &pool)
            );
        }

        acceptor.accept_connections();
        ios.run();
    }
    catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}