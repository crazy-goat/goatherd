//
// Created by piotr on 26.11.17.
//

#include "Session.h"

namespace crazygoat::shepherd {

    void Session::close(const boost::system::error_code &error) {
        if (downstream_socket_.is_open()) {
            if (error.value() != 2 && error.value() != 125) {
                std::cerr << "503: " << error.value() << " : " <<error.message() << std::endl;
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

        this->worker->setIsWorking(false);
    }

    void Session::handle_upstream_write(const boost::system::error_code &error) {
        if (!error) {
            downstream_socket_.async_read_some(
                    boost::asio::buffer(downstream_data_, max_data_length),
                    strand_.wrap(
                            boost::bind(
                                    &Session::handle_downstream_read,
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
    Session::handle_downstream_read(const boost::system::error_code &error,
                                         const size_t &bytes_transferred) {
        if (!error) {
            async_write(
                    upstream_socket_,
                    boost::asio::buffer(downstream_data_, bytes_transferred),
                    strand_.wrap(
                            boost::bind(&Session::handle_upstream_write,
                                        shared_from_this(),
                                        boost::asio::placeholders::error
                            )
                    )
            );
        } else
            close(error);
    }

    void Session::handle_downstream_write(const boost::system::error_code &error) {
        if (!error) {
            upstream_socket_.async_read_some(
                    boost::asio::buffer(upstream_data_, max_data_length),
                    strand_.wrap(
                            boost::bind(
                                    &Session::handle_upstream_read,
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
    Session::handle_upstream_read(const boost::system::error_code &error, const size_t &bytes_transferred) {
        if (!error) {
            async_write(downstream_socket_,
                        boost::asio::buffer(upstream_data_, bytes_transferred),
                        boost::bind(
                                &Session::handle_downstream_write,
                                shared_from_this(),
                                boost::asio::placeholders::error
                        )
            );
        } else
            close(error);
    }

    void Session::handle_upstream_connect(const boost::system::error_code &error) {
        if (!error) {
            // Setup async read from remote server (upstream)
            upstream_socket_.async_read_some(
                    boost::asio::buffer(upstream_data_, max_data_length),
                    boost::bind(
                            &Session::handle_upstream_read,
                            shared_from_this(),
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred
                    )
            );

            // Setup async read from client (downstream)
            downstream_socket_.async_read_some(
                    boost::asio::buffer(downstream_data_, max_data_length),
                    boost::bind(
                            &Session::handle_downstream_read,
                            shared_from_this(),
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred
                    )
            );
        } else
            close(error);
    }

    void Session::start(const std::string &upstream_host, unsigned short upstream_port) {
        // Attempt connection to remote server (upstream side)
        upstream_socket_.async_connect(
                boost::asio::ip::tcp::endpoint(
                        boost::asio::ip::address::from_string(upstream_host),
                        upstream_port
                ),
                boost::bind(
                        &Session::handle_upstream_connect,
                        shared_from_this(),
                        boost::asio::placeholders::error
                )
        );
    }

    void Session::setWorker(const std::shared_ptr<Worker> &worker) {
        this->worker = worker;
    }
}