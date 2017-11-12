//
// Created by piotr on 12.11.17.
//

#include <boost/bind.hpp>
#include "WatchDog.h"

namespace crazygoat::shepherd {

    WatchDog::WatchDog(boost::asio::io_service &ios, std::string command, std::string params, int count,
                       int start_port) : ios(ios), command(command), params(params), count(count),
                                         start_port(start_port) {
        this->timer = std::make_shared<boost::asio::deadline_timer>(this->ios, boost::posix_time::seconds(1));
    }

    void WatchDog::spawn() {
        int port = this->start_port;
        for (int i = 0; i < this->count; i++) {
            std::shared_ptr<Worker> tmp = std::make_shared<Worker>(
                    this->command,
                    this->params,
                    port + i
            );
            tmp->spawn();
            this->workers.push_back(std::move(tmp));
        }
        this->timer->async_wait(boost::bind(&WatchDog::watch, this));
    }

    void WatchDog::watch() {
        for (auto worker:this->workers) {
            if (!worker->isWorking()) {
                worker->spawn();
            }
        }

        std::cout<<"Check workers"<<std::endl;
        this->timer->expires_at(this->timer->expires_at() + boost::posix_time::milliseconds(100));
        this->timer->async_wait(boost::bind(&WatchDog::watch, this));
    }
}