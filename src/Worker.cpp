//
// Created by piotr on 11.11.17.
//

#include "Worker.h"

namespace crazygoat::shepherd {

    Worker::Worker(std::string command) {
        this->command = command;
    }

    void Worker::spawn() {
        this->process = std::make_shared<boost::process::child>(this->command, "", //set the input
                                                                boost::process::std_in.close(),
                                                                boost::process::std_out >
                                                                boost::process::null, //so it can be written without anything
                                                                boost::process::std_err > boost::process::null);
    }
}