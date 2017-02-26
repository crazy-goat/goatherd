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
    boost::asio::io_service::work work(ios);
    boost::thread_group threadpool;
    try {
        boost::asio::ip::address_v4 localhost_address(boost::asio::ip::address_v4::from_string("0.0.0.0"));
        boost::asio::ip::tcp::acceptor acceptor_(ios, boost::asio::ip::tcp::endpoint(localhost_address, local_port));

        for (int i = 0; i < 4; i++) {
            threadpool.create_thread(
                boost::bind(&boost::asio::io_service::run, &ios)
            );
            pppm::LoadBalancer::acceptor *test = new pppm::LoadBalancer::acceptor(ios, acceptor_, forward_host, forward_port);
            ios.post(
                boost::bind(&pppm::LoadBalancer::acceptor::accept_connections, test)
            );
        }

        ios.run();
    }
    catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}