//
// Created by piotr on 25.02.17.
//

#ifndef PPPM_CONFIGLOADER_H
#define PPPM_CONFIGLOADER_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace  pppm {

    class ConfigLoader {
    protected:
        boost::property_tree::ptree config;
    public:
        ConfigLoader(const std::string &configFile)
        {
             boost::property_tree::json_parser::read_json(configFile, this->config);
        }

    };

}
#endif //PPPM_CONFIGLOADER_H
