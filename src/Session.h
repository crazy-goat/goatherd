//
// Created by piotr on 26.11.17.
//

#ifndef SHEPHERD_SESSION_H
#define SHEPHERD_SESSION_H

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "Worker.h"


namespace crazygoat::shepherd {
    class Session: public boost::enable_shared_from_this<Session> {
    public:

        Session(boost::asio::io_service &ios);

        boost::asio::generic::stream_protocol::socket &getDownstreamSocket();

        void start(const std::string &upstream_host, unsigned short upstream_port);

        void handleUpstreamConnect(const boost::system::error_code &error);

        void setWorker(const std::shared_ptr<Worker> &worker);

    private:
        /*
         * Section A: Remote Server --> Proxy --> Client
         * Process data recieved from remote sever then send to client.
        */

        // Read from remote server complete, now send data to client
        void handleUpstreamRead(const boost::system::error_code &error, const size_t &bytes_transferred);

        // Write to client complete, Async read from remote server
        void handleDownstreamWrite(const boost::system::error_code &error);
        // *** End Of Section A ***


        /* Section B: Client --> Proxy --> Remove Server
         * Process data recieved from client then write to remove server.
        */

        // Read from client complete, now send data to remote server
        void handleDownstreamRead(const boost::system::error_code &error, const size_t &bytes_transferred);

        // Write to remote server complete, Async read from client
        void handleUpstreamWrite(const boost::system::error_code &error);
        // *** End Of Section B ***

        void close(const boost::system::error_code &error);

        std::shared_ptr<Worker> worker;

        boost::asio::generic::stream_protocol::socket downstreamSocket;
        boost::asio::generic::stream_protocol::socket upstreamSocket;

        enum {
            max_data_length = 8192
        }; //8KB
        unsigned char downstream_data[max_data_length];
        unsigned char upstream_data[max_data_length];

        /// Strand to ensure the connection's handlers are not called concurrently.
        boost::asio::io_service::strand strand;
    };
}

#endif //SHEPHERD_SESSION_H
