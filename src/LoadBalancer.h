//
// Created by piotr on 25.02.17.
//

#ifndef PPPM_LOADBALANCER_H
#define PPPM_LOADBALANCER_H

#include <cstdlib>
#include <cstddef>
#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include "ConfigLoader.h"
#include "WatchDog.h"

namespace crazygoat::shepherd {

    class LoadBalancer : public boost::enable_shared_from_this<LoadBalancer> {
    public:

        typedef boost::asio::ip::tcp::socket socket_type;
        typedef boost::shared_ptr<LoadBalancer> ptr_type;

        LoadBalancer(boost::asio::io_service &ios, std::shared_ptr<Worker> worker)
                : downstream_socket_(ios),
                  upstream_socket_(ios),
                  strand_(ios),
                  worker(worker) {};

        socket_type &downstream_socket() {
            // Client socket
            return downstream_socket_;
        }

        socket_type &upstream_socket() {
            // Remote server socket
            return upstream_socket_;
        }

        void start(const std::string &upstream_host, unsigned short upstream_port);

        void handle_upstream_connect(const boost::system::error_code &error);

    private:

        std::shared_ptr<Worker> worker;

        /*
         * Section A: Remote Server --> Proxy --> Client
         * Process data recieved from remote sever then send to client.
        */

        // Read from remote server complete, now send data to client
        void handle_upstream_read(const boost::system::error_code &error, const size_t &bytes_transferred);

        // Write to client complete, Async read from remote server
        void handle_downstream_write(const boost::system::error_code &error);
        // *** End Of Section A ***


        /* Section B: Client --> Proxy --> Remove Server
         * Process data recieved from client then write to remove server.
        */

        // Read from client complete, now send data to remote server
        void handle_downstream_read(const boost::system::error_code &error, const size_t &bytes_transferred);

        // Write to remote server complete, Async read from client
        void handle_upstream_write(const boost::system::error_code &error);
        // *** End Of Section B ***

        void close(const boost::system::error_code &error);

        socket_type downstream_socket_;
        socket_type upstream_socket_;

        enum {
            max_data_length = 8192
        }; //8KB
        unsigned char downstream_data_[max_data_length];
        unsigned char upstream_data_[max_data_length];

        /// Strand to ensure the connection's handlers are not called concurrently.
        boost::asio::io_service::strand strand_;
    public:
        class acceptor {
        public:
            acceptor(
                    boost::asio::io_service &io_service,
                    ConfigLoader &config,
                    WatchDog &watchDog
            ) : io_service_(io_service),
                config_(config),
                localhost_address(boost::asio::ip::address_v4::from_string("0.0.0.0")),
                acceptor_(io_service_, boost::asio::ip::tcp::endpoint(localhost_address, config.getListenPort())),
                upstream_host_("127.0.0.1"),
                watchDog(watchDog) {
                acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
            };

            bool accept_connections();

        private:
            void handle_accept(const boost::system::error_code &error);

            ConfigLoader &config_;
            boost::asio::io_service &io_service_;
            boost::asio::ip::address_v4 localhost_address;
            boost::asio::ip::tcp::acceptor acceptor_;
            ptr_type session_;
            std::string upstream_host_;
            WatchDog &watchDog;
            unsigned short upstream_port_;
        };
    };
}

#endif //PPPM_LOADBALANCER_H
