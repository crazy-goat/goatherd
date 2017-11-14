//
// Created by piotr on 26.02.17.
//

#include "LoadBalancer.h"
namespace crazygoat::shepherd {

    void LoadBalancer::acceptor::handle_accept(const boost::system::error_code &error) {
        if (!error) {
            session_->start(
                    this->upstream_host_,
                    this->upstream_port_
            );

            if (!accept_connections()) {
                std::cerr << "Failure during call to accept." << std::endl;
            }
        } else {
            std::cerr << "Error: " << error.message() << std::endl;
        }
    }

    bool LoadBalancer::acceptor::accept_connections() {
        try {
            std::shared_ptr<Worker> worker= this->watchDog.getFreeWorker();
            this->upstream_port_ = worker->getPort();
            session_ = boost::shared_ptr<LoadBalancer>(new LoadBalancer(io_service_, worker));
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

    void LoadBalancer::close(const boost::system::error_code &error) {
        if (downstream_socket_.is_open()) {
            if (error.value() != 2 || error.value() != 125) {
                std::string errorMessage = "HTTP/1.x 503 Service Unavailable\r\n"
                        "Content-Type' => 'text/plain\r\n"
                        "\r\n"
                        "The server is currently unavailable (because it is overloaded or down for maintenance)";

                downstream_socket_.write_some(
                        boost::asio::buffer(errorMessage.c_str(), errorMessage.length())
                );
            }

            downstream_socket_.close();
        }

        if (upstream_socket_.is_open()) {
            upstream_socket_.close();
        }
    }

    void LoadBalancer::handle_upstream_write(const boost::system::error_code &error) {
        if (!error) {
            downstream_socket_.async_read_some(
                    boost::asio::buffer(downstream_data_, max_data_length),
                    strand_.wrap(
                            boost::bind(
                                    &LoadBalancer::handle_downstream_read,
                                    shared_from_this(),
                                    boost::asio::placeholders::error,
                                    boost::asio::placeholders::bytes_transferred
                            )
                    )
            );
        } else
            close(error);
    }

    void
    LoadBalancer::handle_downstream_read(const boost::system::error_code &error,
                                               const size_t &bytes_transferred) {
        if (!error) {
            async_write(
                    upstream_socket_,
                    boost::asio::buffer(downstream_data_, bytes_transferred),
                    strand_.wrap(
                            boost::bind(&LoadBalancer::handle_upstream_write,
                                        shared_from_this(),
                                        boost::asio::placeholders::error
                            )
                    )
            );
        } else
            close(error);
    }

    void LoadBalancer::handle_downstream_write(const boost::system::error_code &error) {
        if (!error) {
            upstream_socket_.async_read_some(
                    boost::asio::buffer(upstream_data_, max_data_length),
                    strand_.wrap(
                            boost::bind(
                                    &LoadBalancer::handle_upstream_read,
                                    shared_from_this(),
                                    boost::asio::placeholders::error,
                                    boost::asio::placeholders::bytes_transferred
                            )
                    )
            );
        } else
            close(error);
    }

    void
    LoadBalancer::handle_upstream_read(const boost::system::error_code &error, const size_t &bytes_transferred) {
        if (!error) {
            async_write(downstream_socket_,
                        boost::asio::buffer(upstream_data_, bytes_transferred),
                        boost::bind(
                                &LoadBalancer::handle_downstream_write,
                                shared_from_this(),
                                boost::asio::placeholders::error
                        )
            );
        } else
            close(error);
    }

    void LoadBalancer::handle_upstream_connect(const boost::system::error_code &error) {
        if (!error) {
            // Setup async read from remote server (upstream)
            upstream_socket_.async_read_some(
                    boost::asio::buffer(upstream_data_, max_data_length),
                    boost::bind(
                            &LoadBalancer::handle_upstream_read,
                            shared_from_this(),
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred
                    )
            );

            // Setup async read from client (downstream)
            downstream_socket_.async_read_some(
                    boost::asio::buffer(downstream_data_, max_data_length),
                    boost::bind(
                            &LoadBalancer::handle_downstream_read,
                            shared_from_this(),
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred
                    )
            );
        } else
            close(error);
    }

    void LoadBalancer::start(const std::string &upstream_host, unsigned short upstream_port) {
        // Attempt connection to remote server (upstream side)
        upstream_socket_.async_connect(
                boost::asio::ip::tcp::endpoint(
                        boost::asio::ip::address::from_string(upstream_host),
                        upstream_port
                ),
                boost::bind(
                        &LoadBalancer::handle_upstream_connect,
                        shared_from_this(),
                        boost::asio::placeholders::error
                )
        );
    }

}
