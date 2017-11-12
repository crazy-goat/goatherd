//
// Created by piotr on 11.11.17.
//

#ifndef SHEPHERD_WORKER_H
#define SHEPHERD_WORKER_H

#include <iostream>
#include <boost/process.hpp>

namespace crazygoat::shepherd {

    class Worker {
    protected:
        std::string command;
        std::shared_ptr<boost::process::child> process;
    public:
        Worker(std::string command);
        void spawn();
    };
}

#endif //SHEPHERD_WORKER_H
