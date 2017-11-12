//
// Created by piotr on 25.02.17.
//

#ifndef PPPM_CONFIGLOADER_H
#define PPPM_CONFIGLOADER_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace crazygoat::shepherd {

    class ConfigLoader {
    protected:
        int listenPort;
        int threads;
        int workersCount;
        int startPort;
        std::string workerCommand;
        std::string workerParams;
    public:
        const std::string &getWorkerCommand() const;

        const std::string &getWorkerParams() const;

    public:
        ConfigLoader(const std::string &configFile) {
            boost::property_tree::ptree config;
            boost::property_tree::json_parser::read_json(configFile, config);
            this->listenPort = config.get<int>("listenPort");
            this->threads = config.get<int>("threads");
            this->workersCount = config.get<int>("worker_count");
            this->startPort = config.get<int>("worker_start_port");
            this->workerCommand = config.get<std::string>("worker_command");
            this->workerParams = config.get<std::string>("worker_params");
        }

        int getListenPort() const;

        int getThreads() const;

        int getWorkersCount() const;

        int getStartPort() const;
    };

}
#endif //PPPM_CONFIGLOADER_H
