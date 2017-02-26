//
// Created by piotr on 26.02.17.
//

#include "LoadBalancer.h"

void pppm::LoadBalancer::acceptor::handle_accept(const boost::system::error_code &error) {
    if (!error) {
        session_->start( "127.0.0.1", upstream_port_);

        if (!accept_connections()) {
            std::cerr << "Failure during call to accept." << std::endl;
        }
    } else {
        std::cerr << "Error: " << error.message() << std::endl;
    }
}

bool pppm::LoadBalancer::acceptor::accept_connections() {
    try {
        session_ = boost::shared_ptr<LoadBalancer>(new LoadBalancer(io_service_));
        acceptor_.async_accept(
            session_->downstream_socket(),
            boost::bind(
                &acceptor::handle_accept,
                this,
                boost::asio::placeholders::error
            )
        );
    }
    catch (std::exception &e) {
        std::cerr << "acceptor exception: " << e.what() << std::endl;
        return false;
    }

    return true;
}

void pppm::LoadBalancer::close() {
    boost::mutex::scoped_lock lock(mutex_);

    if (downstream_socket_.is_open()) {
        downstream_socket_.close();
    }

    if (upstream_socket_.is_open()) {
        upstream_socket_.close();
    }
}

void pppm::LoadBalancer::handle_upstream_write(const boost::system::error_code &error) {
    if (!error) {
        downstream_socket_.async_read_some(
            boost::asio::buffer(downstream_data_, max_data_length),
            strand_.wrap(boost::bind(&LoadBalancer::handle_downstream_read,
                        shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred)));
    } else
        close();
}

void
pppm::LoadBalancer::handle_downstream_read(const boost::system::error_code &error, const size_t &bytes_transferred) {
    if (!error) {
        async_write(upstream_socket_,
                    boost::asio::buffer(downstream_data_, bytes_transferred),
                    strand_.wrap(boost::bind(&LoadBalancer::handle_upstream_write,
                                shared_from_this(),
                                boost::asio::placeholders::error)));
    } else
        close();
}

void pppm::LoadBalancer::handle_downstream_write(const boost::system::error_code &error) {
    if (!error) {
        upstream_socket_.async_read_some(
            boost::asio::buffer(upstream_data_, max_data_length),
            strand_.wrap(boost::bind(&LoadBalancer::handle_upstream_read,
                        shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred)));
    } else
        close();
}

void pppm::LoadBalancer::handle_upstream_read(const boost::system::error_code &error, const size_t &bytes_transferred) {
    if (!error) {
        async_write(downstream_socket_,
                    boost::asio::buffer(upstream_data_, bytes_transferred),
                    boost::bind(&LoadBalancer::handle_downstream_write,
                                shared_from_this(),
                                boost::asio::placeholders::error));
    } else
        close();
}

void pppm::LoadBalancer::handle_upstream_connect(const boost::system::error_code &error) {
    if (!error) {
        // Setup async read from remote server (upstream)
        upstream_socket_.async_read_some(
            boost::asio::buffer(upstream_data_, max_data_length),
            boost::bind(&LoadBalancer::handle_upstream_read,
                        shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));

        // Setup async read from client (downstream)
        downstream_socket_.async_read_some(
            boost::asio::buffer(downstream_data_, max_data_length),
            boost::bind(&LoadBalancer::handle_downstream_read,
                        shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    } else
        close();
}

void pppm::LoadBalancer::start(const std::string &upstream_host, unsigned short upstream_port) {
    // Attempt connection to remote server (upstream side)
    upstream_socket_.async_connect(
        boost::asio::ip::tcp::endpoint(
            boost::asio::ip::address::from_string(upstream_host),
            upstream_port),
        boost::bind(&LoadBalancer::handle_upstream_connect,
                    shared_from_this(),
                    boost::asio::placeholders::error));
}
