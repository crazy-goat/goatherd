//
// Created by piotr on 26.02.17.
//

#include "LoadBalancer.h"

namespace crazygoat::shepherd {

    void LoadBalancer::handle_accept(const boost::system::error_code &error) {
        if (!error) {
            auto worker = this->watchDog->getFreeWorker();
            session_->setWorker(worker);
            session_->start(
                    "127.0.0.1",
                    worker->getPort()
            );

            if (!accept_connections()) {
                std::cerr << "Failure during call to accept." << std::endl;
            }
        } else {
            std::cerr << "Very bad error: " << error.message() << std::endl;
        }
    }

    bool LoadBalancer::accept_connections() {
        try {
            session_ = boost::shared_ptr<Session>(new Session(ios));
            acceptor_->async_accept(
                    session_->downstream_socket(),
                    boost::bind(
                            &LoadBalancer::handle_accept,
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
}
