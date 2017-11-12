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
        std::string hostname;

    public:
        ConfigLoader(const std::string &configFile) {
            boost::property_tree::ptree config;
            boost::property_tree::json_parser::read_json(configFile, config);
            this->listenPort = config.get<int>("listenPort");
            this->threads = config.get<int>("threads");
            this->workersCount = config.get<int>("worker.count");
            this->startPort = config.get<int>("worker.startPort");
            this->hostname = config.get<std::string>("worker.hostname");
        }

        int getListenPort() const;

        int getThreads() const;

        int getWorkersCount() const;

        int getStartPort() const;

        const std::string &getHostname() const;
    };

}
#endif //PPPM_CONFIGLOADER_H
