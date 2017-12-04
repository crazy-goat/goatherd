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
        int workersCount;
        int startSocket;
        std::string workerSocketType;
        std::string workerCommand;
        std::string workerParams;
        std::string socketPath;
    public:
        const std::string &getSocketPath() const;

    public:
        const std::string &getWorkerCommand() const;
        const std::string &getWorkerParams() const;
        const std::string &getWorkerSocketType() const;

        ConfigLoader(const std::string &configFile) {
            boost::property_tree::ptree config;
            boost::property_tree::json_parser::read_json(configFile, config);
            this->listenPort = config.get<int>("listenPort");
            this->workersCount = config.get<int>("worker_count");
            this->startSocket = config.get<int>("worker_start_socket");
            this->workerCommand = config.get<std::string>("worker_command");
            this->workerParams = config.get<std::string>("worker_params");
            this->workerSocketType = config.get<std::string>("worker_socket_type");
            this->socketPath = config.get_optional<std::string>("worker_socket_path").get_value_or("");
        }

        unsigned short getListenPort() const;

        int getWorkersCount() const;

        int getStartSocket() const;
    };

}
#endif //PPPM_CONFIGLOADER_H
