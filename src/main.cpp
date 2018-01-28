// Distributed under the Boost Software License, Version 1.0.

#include "LoadBalancer.h"

using namespace crazygoat::goatherd;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "usage: goatherd <config_file.json>" << std::endl;
    return 1;
  }

  try {
    LoadBalancer loadBalancer(std::make_shared<ConfigLoader>(argv[1]));
    loadBalancer.getWatchDog()->spawn();
    loadBalancer.acceptConnections();
    loadBalancer.run();
  } catch (std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
